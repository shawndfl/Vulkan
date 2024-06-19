#pragma once
#pragma once
#include<sstream>
#include <source_location>
#include <iostream>

std::string getPath(const std::string& path);

#define LOGI(_StringValue) \
    do \
     std::cout << "   " <<  getPath(__FILE__) << ":" << __LINE__ << " " << _StringValue << std::endl; \
    while(0)

#define LOGE(_StringValue) \
    do \
     std::cout << "E: " <<  getPath(__FILE__) << ":" << __LINE__ << " " << _StringValue << std::endl; \
    while(0)