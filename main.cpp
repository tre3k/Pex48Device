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

    while(enter!="q"){
        std::cin >> enter;
        std::cout << "Current value: " << pex48->getCurrentCounterValue() << "\n";
        sleep(1);
    }


    pex48->stopCounter();

    std::cout << "counter: " << pex48->getCounter1() << "\n";
    std::cout << "value: " << pex48->getCounterValue() << "\n";

    std::cout << "stop counter!\nExiting.\n";
    delete pex48;

}
