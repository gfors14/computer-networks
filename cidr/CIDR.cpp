#include "CIDR.h"
#include <cmath>
#include <iostream>

CIDR::CIDR(const std::string& cidr) 
{
    auto pos = cidr.find('/');
    std::string ip = cidr.substr(0, pos);
    prefixLength = std::stoi(cidr.substr(pos + 1));
    netmask = prefixToNetmask(prefixLength);
    networkAddress = IPaddress::toInt(ip) & netmask;
    broadcastAddress = networkAddress | (~netmask);
}

void CIDR::calculateAddresses() 
{

    
}

unsigned int CIDR::getNetworkAddress() const 
{
    return networkAddress;
}

unsigned int CIDR::getBroadcastAddress() const 
{
	
    return broadcastAddress;
}

unsigned int CIDR::getFirstUsableAddress() const 
{
    return networkAddress + 1;
}

unsigned int CIDR::getLastUsableAddress() const 
{
    return broadcastAddress - 1;
}

unsigned int CIDR::getNetmask() const 
{
    return netmask;
}

int CIDR::getNumUsableHosts() const 
{
    if (prefixLength == 32 || prefixLength == 31) return 0;
    return (int)pow(2, 32 - prefixLength) - 2;
}

unsigned int CIDR::prefixToNetmask(int prefix) 
{
    return (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;
}

void CIDR::printAllUsableHostAddresses() const {
    if (getNumUsableHosts() > 0) {  // Only proceed if there are usable hosts
        for (unsigned int addr = getFirstUsableAddress(); addr <= getLastUsableAddress(); ++addr) {
            std::cout << IPaddress::toString(addr) << std::endl;
        }
    } 
	else 
	{
        std::cout << "No usable host addresses available." << std::endl;
    }
}
