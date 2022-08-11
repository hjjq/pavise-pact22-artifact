#pragma once

#include <libpmemobj++/make_persistent_atomic.hpp>

#include <reservation.hpp>
#ifndef NO_TX
#include <map.hpp>
#else 
#include <map_notx.hpp>
#endif
using pmem::obj::pool_base;
using pmem::obj::delete_persistent_atomic;

// TODO: change to a immer list?
//using LIST_T = immer::vector<ReservationInfo*>;
// Unlike original vacation, we store reservation info in a map
// ReservationType_id : price (eg: reservationtype*kPrepopulatedRelations+id: 10000).
using LIST_T = PMDKMap<uint64_t, long>;

class Customer {
  public:
    Customer (long id, persistent_ptr<LIST_T> reservation_info_list)
      : id_(id), reservation_info_list_(reservation_info_list) {}

    long GetId() const { return id_; }

    void AddReservationInfo(pool_base& pop, ReservationType type,
                                 long id, long price);

    void RemoveReservationInfo(pool_base& pop, ReservationType type,
                                    long id);

    void CancelAllReservations(pool_base& pop, void* manager);

    long GetBill(pool_base& pop);

    const persistent_ptr<LIST_T> GetReservationInfoList() const {
      return reservation_info_list_;
    }

  private:
    // TODO: should this be p<long>
    long id_;
    persistent_ptr<LIST_T> reservation_info_list_;
};

persistent_ptr<Customer> AllocCustomer(pool_base &pop, long id,
        persistent_ptr<LIST_T> reservation_info_list);

// TODO: No idea how this should work. Maybe return nullptr?
Customer* FreeCustomer(Customer *customer);
