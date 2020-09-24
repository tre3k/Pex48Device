//
// Created by Kirill Pshenichnyi pshcyrilL@mail.ru a on 24.09.2020.
//

#include "Pex48Device.h"

Pex48Device::Pex48Device(std::string path_to_device) {
    _fd = open(path_to_device.c_str(),O_RDWR);
}

Pex48Device::~Pex48Device() {
    close(_fd);
}

void Pex48Device::sig_handler(int isig) {
    counter_accamulate += 65536;

    std::cout << isig << " Handler!\n";
}