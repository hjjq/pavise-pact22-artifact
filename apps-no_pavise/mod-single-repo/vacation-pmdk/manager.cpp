#include <iostream>
#include <cassert>
#include <stdlib.h>

#include <util.hpp>
#include <manager.hpp>
#include <constants.hpp>

struct MapStats {
    long num_used = 0;
    long num_free = 0;
    long num_total = 0;
};

int MapAggregator(uint64_t key, PMEMoid value_ptr, 
        void* arg) {
    auto reservation = *(persistent_ptr<Reservation>*) pmemobj_direct(value_ptr);
    struct MapStats* stats = (struct MapStats*) arg;
    stats->num_used += reservation->num_used;
    stats->num_free += reservation->num_free;
    stats->num_total += reservation->num_total;
    return 0;
}

int PrintCustomerVisitor(uint64_t key, PMEMoid value_ptr, 
        void* arg) {
    auto customer = *(persistent_ptr<Customer>*) pmemobj_direct(value_ptr);
    std::cout << "Customer Id:" << key << " Ptr:" << customer << std::endl;
    return 0;
}

void PrintMapContents(pool_base& pop, const persistent_ptr<MAP_T> map,
        std::string map_name) {
    std::cout << map_name << " Size:" << map->size(pop.handle());
    map->print();
    std::cout << std::endl;
    struct MapStats stats;
    map->ForEach(pop.handle(), &MapAggregator, (void*)&stats);
    std::cout << "Free: " << stats.num_free
              << " Used: " << stats.num_used
              << " Total: " << stats.num_total
              << std::endl;
    /*
    auto iterator = map->begin();
    long num_used = 0;
    long num_free = 0;
    long num_total = 0;
    while (iterator != map->end()) {
        auto reservation = (*iterator).second;
        LOG("Id:%ld Reservation:%s", (*iterator).first,
                                     ToString(reservation).c_str());
        
        num_used += reservation->num_used;
        num_free += reservation->num_free;
        num_total += reservation->num_total;;
        iterator++;
    }
    std::cout << " Free: " << num_free << " Used: " << num_used
              << " Total: " << num_total << std::endl;
              */
}

persistent_ptr<Manager> AllocManager(pool_base &pop,
        persistent_ptr<MAP_T> car_table_ptr,
        persistent_ptr<MAP_T> room_table_ptr,
        persistent_ptr<MAP_T> flight_table_ptr,
        persistent_ptr<CUSTOMER_MAP_T> customer_table_ptr) {
    // Allocate each of the reservation tables, if NULL.
    if (car_table_ptr == nullptr) {
        transaction::run(pop, [&] {
                car_table_ptr = make_persistent<MAP_T>();
                car_table_ptr->init(pop);
        });
    } 
    if (room_table_ptr == nullptr) {
        transaction::run(pop, [&] {
                room_table_ptr = make_persistent<MAP_T>();
                room_table_ptr->init(pop);
        });
    }
    if (flight_table_ptr == nullptr) {
        transaction::run(pop, [&] {
                flight_table_ptr = make_persistent<MAP_T>();
                flight_table_ptr->init(pop);
        });
    } 
    if (customer_table_ptr == nullptr) {
        make_persistent_atomic<CUSTOMER_MAP_T>(pop, customer_table_ptr);
        customer_table_ptr->init(pop);
    }

    //void *mem = nvm_reserve(sizeof(Manager));
    // TODO-PMDK: Allocate persistently
    persistent_ptr<Manager> manager_ptr;
    make_persistent_atomic<Manager>(pop, manager_ptr, car_table_ptr,
           room_table_ptr, flight_table_ptr, customer_table_ptr);
    
    // TODO: maybe I should use nvm_activate here for all table ptrs?
    // I do not want to as manager_ptr itself is not activated yet.
    // NVM_PERSIST(manager_ptr, sizeof(Manager));
    return manager_ptr;
}

bool DeleteReservation(pool_base& pop, persistent_ptr<MAP_T> table_ptr, long id) {
    table_ptr->erase(pop.handle(), id);
    return true;
}

bool CancelReservation(pool_base& pop, persistent_ptr<MAP_T> table_ptr, long id) {
    // Find if table contains the id:
    auto old_reservation = table_ptr->find(pop.handle(), id);
    if (old_reservation == nullptr || old_reservation->num_used == 0) {
        std::cerr << "No such reservation found:" << std::endl;
        return false;
    }
    old_reservation->num_used -= 1;
    old_reservation->num_free += 1; return true;
}

bool AddReservation(pool_base &pop, persistent_ptr<MAP_T> table_ptr, long id,
        long num, long price) {

    // Find if table contains the id:
    auto old_reservation = table_ptr->find(pop.handle(), id);
    if (old_reservation == nullptr) {
        if (num < 1 || price < 0) 
            // Cannot delete a reservation that does not exist.
            return false;
        // We create new reservation after the if-else block.
    }
    else {
        // Add number to total
        if (old_reservation->num_free + num < 0) {
            // Can't delete more than available.
            return false;
        }
        old_reservation->num_free += num;
        old_reservation->num_total += num;
        if (old_reservation->num_total == 0) {
            // TODO: Delete resource even if it may be in use?
            return DeleteReservation(pop, table_ptr, id);
        }
        return true;
    }

    // Allocate reservation in persistent memory with updated num and price.
    auto reservation = AllocReservation(pop, id, 0, num, num, price);
    table_ptr->insert(pop.handle(), id, reservation);
    return true;
}

bool AddCustomer (pool_base &pop, persistent_ptr<CUSTOMER_MAP_T>& customer_table_ptr,
        long id) {
    auto customer = customer_table_ptr->find(pop.handle(), id);

    if (customer != nullptr) 
        return false;

    // TODO-PMDK Allocate customer in persistent memory.
    auto new_customer = AllocCustomer(pop, id, nullptr);

    // Insert into the map
    customer_table_ptr->insert(pop.handle(), id, new_customer);
    return true;
}

bool Manager::AddCar(long id, long num, long price) {
    return AddReservation(pop_, car_table_ptr_, id, num, price);
}

bool Manager::AddRoom(long id, long num, long price) {
    return AddReservation(pop_, room_table_ptr_, id, num, price);
}

bool Manager::AddFlight(long id, long num, long price) {
    return AddReservation(pop_, flight_table_ptr_, id, num, price);
}

bool Manager::DeleteCar(long id, long num) {
    return AddReservation(pop_, car_table_ptr_, id, -num ,-1);
}

bool Manager::DeleteRoom(long id, long num) {
    return AddReservation(pop_, room_table_ptr_, id, -num, -1);
}

bool Manager::DeleteFlight(long id) {
    auto old_reservation = flight_table_ptr_->find(pop_.handle(), id);
    if (old_reservation == nullptr || old_reservation->num_used > 0) {
        return false;
    }
    return DeleteReservation(pop_, flight_table_ptr_, id);
}

bool Manager::CancelCar(long id) {
    return CancelReservation(pop_, car_table_ptr_, id);
}

bool Manager::CancelRoom(long id) {
    return CancelReservation(pop_, room_table_ptr_, id);
}

bool Manager::CancelFlight(long id) {
    return CancelReservation(pop_, flight_table_ptr_, id);
}

bool Manager::Reserve(persistent_ptr<MAP_T> table_ptr, long customer_id,
        long id, ReservationType type) {
    auto customer = customer_table_ptr_->find(pop_.handle(), customer_id);
    if (customer == nullptr) {
        return false;
    }

    auto reservation = table_ptr->find(pop_.handle(), id);
    if (reservation == nullptr)
        return false;
    
    if (!MakeReservation(reservation))
        return false;

    customer->AddReservationInfo(pop_, type, id, reservation->price);
    return true;
} 

bool Manager::ReserveCar(long customer_id, long id) {
    return Reserve(car_table_ptr_, customer_id, id, RESERVATION_CAR);
}

bool Manager::ReserveRoom(long customer_id, long id) {
    return Reserve(room_table_ptr_, customer_id, id, RESERVATION_ROOM);
}

bool Manager::ReserveFlight(long customer_id, long id) {
    return Reserve(flight_table_ptr_, customer_id, id, RESERVATION_FLIGHT);
}

bool Manager::AddCustomer(long id) {
    return ::AddCustomer(pop_, customer_table_ptr_, id);
}

bool Manager::DeleteCustomer(long id) {
    auto customer = customer_table_ptr_->find(pop_.handle(), id);
    if (customer == nullptr) 
        return false;

    assert (num_tables_ == 3); // Failsafe to make sure I add code here.

    customer->CancelAllReservations(pop_, this);
    
    // Original vacation deletes customer from customer table here, 
    // but WHISPER version doesn't.
    return true;
}

long Manager::QueryCustomerBill(long id) {
    long bill = -1;
    auto customer = customer_table_ptr_->find(pop_.handle(), id);
    if (customer != nullptr) 
        bill = customer->GetBill(pop_);
    return bill;
}

void Manager::PrintTables() {
    PrintMapContents(pop_, car_table_ptr_, "car table");
    PrintMapContents(pop_, room_table_ptr_, "room table");
    PrintMapContents(pop_, flight_table_ptr_, "flight table");
}
