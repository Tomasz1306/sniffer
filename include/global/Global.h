#ifndef GLOBAL_H
#define GLOBAL_H
#include <mutex>
#include <condition_variable>
#include <chrono>
extern std::mutex guard_1;
extern std::mutex guard_2;
extern std::mutex guard_3;
extern std::mutex guard_4;
extern std::mutex logGuard;
extern std::mutex warningGuard;
extern std::mutex analyzerGuard;
extern std::condition_variable database_cv;
extern std::mutex getDatabasesMutex;
extern std::chrono::steady_clock::time_point last_time;
extern bool packetWindowInitialized;

#endif // GLOBAL_H
