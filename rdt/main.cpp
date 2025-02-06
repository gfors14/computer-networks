#include "rdt.h"
#include <iostream>

int main() 
{
   
    rdt client("thomas.butler.edu", 43539);
    
   
    client.rdt_send("test");
    client.rdt_send("test23232");

    return 0;
}
