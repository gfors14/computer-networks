#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <string>

class IPaddress 
{
	public:
		static unsigned int toInt(const std::string &ip);
		static std::string toString(unsigned int ip);
};

#endif 
