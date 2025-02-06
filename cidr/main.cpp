#include <iostream>
#include "CIDR.h"

int main() 
{	
	std::string cidrInput;
    std::cout << "Enter CIDR IP (e.g., 192.168.1.0/24): ";
    std::cin >> cidrInput;
	
	CIDR network(cidrInput);
    std::cout << "Network Address: " << IPaddress::toString(network.getNetworkAddress()) << std::endl;
    std::cout << "Broadcast Address: " << IPaddress::toString(network.getBroadcastAddress()) << std::endl;
    std::cout << "First Usable Address: " << IPaddress::toString(network.getFirstUsableAddress()) << std::endl;
    std::cout << "Last Usable Address: " << IPaddress::toString(network.getLastUsableAddress()) << std::endl;
    std::cout << "Number of Usable Hosts: " << network.getNumUsableHosts() << std::endl;
	
	char choice;
    std::cout << "Do you want to list all usable host addresses? (y/n): ";
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') 
	{
        network.printAllUsableHostAddresses();
    }


    return 0;
}
