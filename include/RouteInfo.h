#pragma once

#include <regex>
#include <vector>
#include <string>
#include "RequestHandler.h"

struct RouteInfo {
    std::string method;
    std::regex pattern;
    RequestHandler* handler;
    std::vector<std::string> paramNames;
};