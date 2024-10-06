#ifndef GLOBAL_H
#define GLOBAL_H
#include <mutex>

extern std::mutex guard_1;
extern std::mutex guard_2;
extern std::mutex guard_3;

extern bool packetWindowInitialized;

#endif // GLOBAL_H
