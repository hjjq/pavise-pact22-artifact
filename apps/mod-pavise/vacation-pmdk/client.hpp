#pragma once

// Unlike original vacation, we do not store manager_ptr
// with the Client as we keep copying it!
struct Client {
    Client(long id_in, long num_operations_in,
           long num_query_per_transaction_in, long query_range_in,
           long percent_user_queries_in, pool_base &pop_in) 
           : id(id_in), 
             num_operations(num_operations_in),
             num_query_per_transaction(num_query_per_transaction_in), 
             query_range(query_range_in), 
             percent_user_queries(percent_user_queries_in),
             pop (pop_in)
             {} 

    long id;
    long num_operations;
    long num_query_per_transaction;
    long query_range;
    long percent_user_queries;
    pool_base& pop;
};

// TODO: Move client and Action to separate types.hpp?
enum Action {
  ACTION_MAKE_RESERVATION = 0,
  ACTION_DELETE_CUSTOMER = 1,
  ACTION_UPDATE_TABLES = 2,
  NUM_ACTION
};

