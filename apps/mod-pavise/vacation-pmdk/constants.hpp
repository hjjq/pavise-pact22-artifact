#pragma once

constexpr auto kManagerId = "manager";
// Expected use: customer_1, customer_2.
constexpr auto kCustomerId = "customer_";

// This is referred to as numRelations in original vacation.
constexpr auto kPrepopulatedRelations = 100000;
constexpr auto kNumClients = 1;
constexpr auto kNumTransactions = 1000;
constexpr auto kTransactionsPerClient = kNumTransactions/kNumClients;

// Max number of queries that can be performed in a transaction.
// TODO: Does this make sense for our approach?
constexpr auto kQueriesPerTransaction = 2;
// Dictates what percent of the initial flights/cars/hotels are booked
// by customers. If this number is low, contention is high.
constexpr auto kPercentQueryRange = 90;

// 3 types of queries: 
// a. user makes reservations
// b. user gets removed from system
// c. flights/cars/hotels are added/removed.
constexpr auto kPercentUserQueries = 80;

// All prices are randomly generated up to this value.
constexpr auto kMaxPrice = 100000;
