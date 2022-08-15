#include <cassert>
#include <sstream>

#include <reservation.hpp>

// Here id is car/flight/room id depending on type
persistent_ptr<Reservation> AllocReservation(pool_base &pop,
        long id, long num_used, long num_free, long num_total, long price) {
    // nvm_utils overloads new operator.
    persistent_ptr<Reservation> reservation;
    make_persistent_atomic<Reservation>(pop, reservation, id, num_used,
            num_free, num_total, price);
    return reservation;
}

// Here id is car/flight/room id depending on type
persistent_ptr<ReservationInfo> AllocReservationInfo(pool_base &pop,
        ReservationType type, long id, long price) {
    persistent_ptr<ReservationInfo> reservation_info;
    // nvm_utils overloads new operator.
    make_persistent_atomic<ReservationInfo>(pop, reservation_info,
            type, id, price);
    //NVM_PERSIST(reservation_info, sizeof(ReservationInfo));
    return reservation_info;
}

std::string ToString(persistent_ptr<Reservation> reservation) {
    std::stringstream ss;
    ss << "id:" << reservation->id 
       << " used:" << reservation->num_used
       << " free:" << reservation->num_free
       << " total:" << reservation->num_total
       << " price:" << reservation->price;
    return ss.str();
}

bool MakeReservation(persistent_ptr<Reservation> reservation) {
    if (reservation->num_free < 1)
        return false;
    reservation->num_used++;
    reservation->num_free--;
    return true;
}
