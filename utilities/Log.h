#pragma once
#include<sstream>
#include <source_location>
#include <iostream>
#include <glm/glm.hpp>

std::string getPath(const std::string& path);

#define LOGI(_StringValue) \
    do \
     std::cout << "   " <<  getPath(__FILE__) << ":" << __LINE__ << " " << _StringValue << std::endl; \
    while(0)

#define LOGE(_StringValue) \
    do \
     std::cout << "E: " <<  getPath(__FILE__) << ":" << __LINE__ << " " << _StringValue << std::endl; \
    while(0)


std::ostream& operator<<(std::ostream & out, const glm::vec3 & v);
std::ostream& operator<<(std::ostream & out, const glm::vec2 & v);