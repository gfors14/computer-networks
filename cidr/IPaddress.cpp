#include "IPaddress.h"
#include <sstream>
#include <bitset>

unsigned int IPaddress::toInt(const std::string &ip) 
{
    unsigned int bytes[4];
    std::sscanf(ip.c_str(), "%u.%u.%u.%u", &bytes[3], &bytes[2], &bytes[1], &bytes[0]);
    return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}

std::string IPaddress::toString(unsigned int ip) 
{
    return std::to_string(ip >> 24) + '.' +
           std::to_string((ip >> 16) & 0xFF) + '.' +
           std::to_string((ip >> 8) & 0xFF) + '.' +
           std::to_string(ip & 0xFF);
}
