#ifndef GLOBAL_H
#define GLOBAL_H
#include <mutex>
#include <condition_variable>

extern std::mutex guard_1;
extern std::mutex guard_2;
extern std::mutex guard_3;
extern std::mutex guard_4;
extern std::mutex logGuard;
extern std::condition_variable database_cv;

extern bool packetWindowInitialized;

#endif // GLOBAL_H
