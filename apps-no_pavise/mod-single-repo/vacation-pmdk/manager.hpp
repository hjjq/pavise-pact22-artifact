#pragma once

#include <iostream>
#include <vector>

#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/make_persistent_atomic.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/transaction.hpp>

#include <reservation.hpp>
#include <customer.hpp>
#ifndef NO_TX
#include <map.hpp>
#else 
#include <map_notx.hpp>
#endif

using pmem::obj::make_persistent;
using pmem::obj::make_persistent_atomic;
using pmem::obj::p;
using pmem::obj::persistent_ptr;
using pmem::obj::pool_base;
using pmem::obj::transaction;

//TODO-PMDK: Make reservation persistent.
//using MAP_T = std::map<long, Reservation*, std::less<long>,
//      pmem::obj::allocator<std::pair<const long, Reservation*>>>;
using MAP_T = PMDKMap<uint64_t, persistent_ptr<Reservation>>;
using CUSTOMER_MAP_T = PMDKMap<uint64_t, persistent_ptr<Customer>>;

class Manager {
  public:
    Manager(persistent_ptr<MAP_T> car_table_ptr,
            persistent_ptr<MAP_T> room_table_ptr,
            persistent_ptr<MAP_T> flight_table_ptr,
            persistent_ptr<CUSTOMER_MAP_T> customer_table_ptr)
      : car_table_ptr_ (car_table_ptr),
        room_table_ptr_ (room_table_ptr),
        flight_table_ptr_ (flight_table_ptr),
        customer_table_ptr_ (customer_table_ptr)
        {} 

    // Manager(Manager *other) : car_table_ptr_(other->car_table_ptr_) {}
    void UpdatePool(pool_base &pop) {
        pop_ = pop;
    }

    bool AddCar(long id, long num, long price);
    bool AddRoom(long id, long num, long price);
    bool AddFlight(long id, long num, long price);

    bool DeleteCar(long id, long num);
    bool DeleteRoom(long id, long num);
    bool DeleteFlight(long id);
    
    // Cancel* removes one customers reservation.
    bool CancelCar(long id);
    bool CancelRoom(long id);
    bool CancelFlight(long id);

    // Add/DeleteCustomer is not marked const as we may return this 
    // if no customer is deleted. Using a clever combination of const 
    // in the return path, we may be able to make it work.
    // TODO: Mark as const.
    bool AddCustomer(long id);   

    bool DeleteCustomer(long id);

    long QueryCustomerBill(long id);

    long QueryCar (long id) {
      auto reservation = car_table_ptr_->find(pop_.handle(), id);
      if (reservation == nullptr)
        return -1;
      return reservation->num_free;
    }

    // Returns -1 if does not exist
    long QueryCarPrice (long id) {
      auto reservation = car_table_ptr_->find(pop_.handle(), id);
      if (reservation == nullptr)
        return -1;
      return reservation->price;
    }

    long QueryRoom (long id) {
      auto reservation = room_table_ptr_->find(pop_.handle(), id);
      if (reservation == nullptr)
        return -1;
      return reservation->num_free;
    }

    // Returns -1 if does not exist
    long QueryRoomPrice (long id) {
      auto reservation = room_table_ptr_->find(pop_.handle(), id);
      if (reservation == nullptr)
        return -1;
      return reservation->price;
    }

    long QueryFlight (long id) {
      auto reservation = flight_table_ptr_->find(pop_.handle(), id);
      if (reservation == nullptr)
        return -1;
      return reservation->num_free;
    }

    // Returns -1 if does not exist
    long QueryFlightPrice (long id) {
      auto reservation = flight_table_ptr_->find(pop_.handle(), id);
      if (reservation == nullptr)
        return -1;
      return reservation->price;
    }

    bool Reserve(persistent_ptr<MAP_T> table_ptr, long customerId, long id,
        ReservationType type);

    bool ReserveCar(long customer_id, long id);
    
    bool ReserveRoom(long customer_id, long id);
    
    bool ReserveFlight(long customer_id, long id);

    static long NumReservationTypes() { return num_tables_; }  

    void PrintTables();

    void AssertInnerTablesNotNull() const {
        assert (car_table_ptr_ != nullptr);
        assert (room_table_ptr_ != nullptr);
        assert (flight_table_ptr_ != nullptr);
        assert (customer_table_ptr_ != nullptr);
    }


  private:

    static const int num_tables_ = 3;
    persistent_ptr<MAP_T> car_table_ptr_;
    persistent_ptr<MAP_T> room_table_ptr_;
    persistent_ptr<MAP_T> flight_table_ptr_;
    persistent_ptr<CUSTOMER_MAP_T> customer_table_ptr_;

    // Stored in volatile memory, updated every time.
    pool_base pop_;
};

persistent_ptr<Manager> AllocManager(pool_base &pop,
        persistent_ptr<MAP_T> car_table_ptr,
        persistent_ptr<MAP_T> room_table_ptr,
        persistent_ptr<MAP_T> flight_table_ptr,
        persistent_ptr<CUSTOMER_MAP_T> customer_table_ptr);

bool DeleteReservation(persistent_ptr<MAP_T> table_ptr, long id);

bool AddReservation(pool_base &pop, persistent_ptr<MAP_T> table_ptr, long id,
        long num, long price);

bool AddCustomer (pool_base &pop, persistent_ptr<CUSTOMER_MAP_T>& customer_table_ptr,
        long id);
