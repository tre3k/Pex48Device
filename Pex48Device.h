//
// Created by Kirill Pshenichnyi pshcyrilL@mail.ru a on 24.09.2020.
//

#ifndef PEX48DEVICE_PEX48DEVICE_H
#define PEX48DEVICE_PEX48DEVICE_H

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


#include <ixpio.h>
#include <signal.h>


class Pex48Device {
private:
    int _fd;

protected:
    unsigned long counter_accamulate {0};

    void sig_handler(int isig);
public:
    Pex48Device(std::string path_to_device);
    ~Pex48Device(void);

};


#endif //PEX48DEVICE_PEX48DEVICE_H
