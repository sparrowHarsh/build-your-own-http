#pragma once

#include <regex>
#include <vector>
#include <string>
#include "RequestHandler.h"
#include "HttpMethod.h"

struct RouteInfo {
    HttpMethod method;
    std::regex pattern;
    RequestHandler* handler;
    std::vector<std::string> paramNames;
};