#pragma once

#include "spdlog/spdlog.h"

// Macros
#define logInfo(...) SPDLOG_INFO(__VA_ARGS__);
#define logDebug(...) SPDLOG_DEBUG(__VA_ARGS__);
#define logError(...) SPDLOG_ERROR(__VA_ARGS__);