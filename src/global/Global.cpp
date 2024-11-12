#include "global/Global.h"

std::mutex guard_1;
std::mutex guard_2;
std::mutex guard_3;
std::mutex guard_4;
std::mutex logGuard;
std::mutex warningGuard;
std::mutex analyzerGuard;
std::condition_variable database_cv;
std::mutex getDatabasesMutex;
std::chrono::steady_clock::time_point last_time;
bool packetWindowInitialized{false};