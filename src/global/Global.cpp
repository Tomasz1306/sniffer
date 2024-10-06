#include "global/Global.h"

std::mutex guard_1;
std::mutex guard_2;
std::mutex guard_3;
bool packetWindowInitialized{false};