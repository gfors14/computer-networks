#ifndef CIDRNETWORK_H
#define CIDRNETWORK_H

#include <string>
#include "IPaddress.h"

class CIDR 
{
private:
    unsigned int networkAddress;
    unsigned int broadcastAddress;
    unsigned int netmask;
    int prefixLength;

public:
    CIDR(const std::string& cidr);
    void calculateAddresses();

    unsigned int getNetworkAddress() const;
    unsigned int getBroadcastAddress() const;
    unsigned int getFirstUsableAddress() const;
    unsigned int getLastUsableAddress() const;
    unsigned int getNetmask() const;
    int getNumUsableHosts() const;
	 void printAllUsableHostAddresses() const;

    static unsigned int prefixToNetmask(int prefix);
};

#endif 
