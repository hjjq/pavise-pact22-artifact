#pragma once

#include <string>

#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/make_persistent_atomic.hpp>
#include <libpmemobj++/pool.hpp>

using pmem::obj::make_persistent;
using pmem::obj::make_persistent_atomic;
using pmem::obj::p;
using pmem::obj::persistent_ptr;
using pmem::obj::pool_base;

struct Reservation {

    Reservation(long id_in, long num_used_in, long num_free_in,
                long num_total_in, long price_in)
        : id(id_in),
          num_used(num_used_in),
          num_free(num_free_in),
          num_total(num_total_in),
          price(price_in)
    {}

    // TODO: id is in key as well as value.
    long id;
    long num_used;
    long num_free;
    // TODO: numTotal is just numUsed + numFree.
    long num_total;
    long price;
};

// We do not use enum class here as we generate a random long
// to select between these types.
enum ReservationType {
  RESERVATION_CAR = 0,
  RESERVATION_FLIGHT = 1,
  RESERVATION_ROOM = 2,
  NUM_RESERVATION_TYPES
};

struct ReservationInfo {
    ReservationInfo (ReservationType type_in, long id_in, long price_in)
        : type (type_in), id (id_in), price (price_in) {}

    p<ReservationType> type;
    p<long> id;
    p<long> price; /* holds price at time reservation was made */
};


persistent_ptr<Reservation> AllocReservation(pool_base &pop, long id,
        long num_used, long num_free, long num_total, long price);

// Here id is car/flight/room id depending on type
persistent_ptr<ReservationInfo> AllocReservationInfo(pool_base &pop, ReservationType type,
        long id, long price);

std::string ToString(persistent_ptr<Reservation> reservation);

bool MakeReservation(persistent_ptr<Reservation> reservation);
