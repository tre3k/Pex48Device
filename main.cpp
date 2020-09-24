#include <iostream>

#include "Pex48Device.h"

int main() {
    std::string enter;

    Pex48Device *pex48 = new Pex48Device("/dev/ixpio1");
    std::cout << "start counter!\n";

    if(pex48->getErrno()!=0){
        std::cout << "Error!\n";
        delete pex48;
        return 0;
    }

    //pex48->startCounter();
    if(pex48->getErrno()!=0){
        std::cout << "Start error!\n";
        delete pex48;
        return 0;
    }

    pex48->startCounter();

    while(1){
        std::cout << "Current value: " << pex48->getCounterValue() << "\n";
        sleep(1);
        if(pex48->getCounterValue()>65535*3) break;
    }

    pex48->stopCounter();

    std::cout << "value: " << pex48->getCounterValue() << "\n";

    std::cout << "stop counter!\nExiting.\n";
    delete pex48;

}
