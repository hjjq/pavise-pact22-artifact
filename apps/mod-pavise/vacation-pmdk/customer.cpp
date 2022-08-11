#include <string>
#include <sstream>
#include <util.hpp>
#include <customer.hpp>
#include <constants.hpp>
#include <manager.hpp>

uint64_t reservationToKey (ReservationType type, long id) {
    return kPrepopulatedRelations*type+id;
}

int SumVisitor(uint64_t key, PMEMoid value_ptr, 
        void* arg) {
    auto price = *(long*) pmemobj_direct(value_ptr);
    auto bill = (long*) arg;
    *bill += price;
    return 0;
}

int CancelVisitor(uint64_t key, PMEMoid value_ptr, 
        void* arg) {
    auto manager = (Manager*) arg;
    const auto reservation_type = key/kPrepopulatedRelations;
    const auto id = key % kPrepopulatedRelations; 
    switch (reservation_type) {
        case 0: 
            { 
                manager->CancelCar(id);
            }
            break;
        case 1:
            {
                manager->CancelFlight(id);
            }
            break;
        case 2:
            {
                manager->CancelRoom(id);
            }
            break;
        default:
            std::cerr << "Reservation type not supported:" 
                << reservation_type << std::endl;
            return 1;
    }
    return 0;
}

persistent_ptr<Customer> AllocCustomer(pool_base &pop, long id, persistent_ptr<LIST_T> reservation_info_list) {
    // Allocate the reservation list, if NULL.
    if (reservation_info_list == nullptr) {
        make_persistent_atomic<LIST_T>(pop, reservation_info_list);
        reservation_info_list->init(pop);
    }

    persistent_ptr<Customer> customer;
    make_persistent_atomic<Customer>(pop, customer, id, reservation_info_list);

    return customer;
}

// Returns nullptr if duplicate info.
void Customer::AddReservationInfo(pool_base& pop, ReservationType type,
                                       long id, long price)
{
    // Create new Reservation Info.
    /*
    auto *reservation_info = AllocReservationInfo(type, id, price);
    assert(reservation_info != nullptr);
    */
    auto key = reservationToKey(type, id);
    
    // Ensure not a duplicate entry
    auto reservation_info = reservation_info_list_->find(pop.handle(), key);
    if (reservation_info != 0)
        return;

    // Insert into list.
    reservation_info_list_->insert(pop.handle(), key, price);

    return;
}

void Customer::RemoveReservationInfo(pool_base& pop, ReservationType type,
                                          long id)
{
    reservation_info_list_->erase(pop.handle(), reservationToKey(type, id));
    return;
}

void Customer::CancelAllReservations(pool_base& pop, void* manager) {
    NOTE("Customer:%lu has %lu reservations\n", id_, reservation_info_list_->size(pop.handle()));
    reservation_info_list_->ForEach(pop.handle(), &CancelVisitor, manager);
    // No way to clear entire map
    delete_persistent_atomic<LIST_T>( reservation_info_list_);
    make_persistent_atomic<LIST_T>(pop, reservation_info_list_);
    reservation_info_list_->init(pop);
}

long Customer::GetBill(pool_base& pop) {
    long bill = 0;
    reservation_info_list_->ForEach(pop.handle(), &SumVisitor, (void*)&bill);
    return bill;
}
