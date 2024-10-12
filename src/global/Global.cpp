#include "global/Global.h"

std::mutex guard_1;
std::mutex guard_2;
std::mutex guard_3;
std::mutex guard_4;
std::mutex logGuard;
std::condition_variable database_cv;
bool packetWindowInitialized{false};