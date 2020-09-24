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
    int _fd;            // file descriptor for device
    int _errno {0};

    /* define const RegID */
    // interrupt registers
    static constexpr unsigned int INT_MASK_REG =                 0x03;
    static constexpr unsigned int AUX_PIN_STAT_REG =             0x04;
    static constexpr unsigned int INT_POL_REG =                  0x05;

    // Read/Write Port ports and Control worlds wBase+0xc0 ... +0xdc
    static constexpr unsigned int PORTA_REG =                     0x0d;
    static constexpr unsigned int PORTB_REG =                     0x0e;
    static constexpr unsigned int PORTC_REG =                     0x0f;
    static constexpr unsigned int CONTROL_WORD_IO =               0x10;

    // Counters register
    static constexpr int COUNTER0_REG =                           0x15;
    static constexpr int COUNTER1_REG =                           0x16;
    static constexpr int COUNTER2_REG =                           0x17;
    static constexpr int CW_8254 =                                0x18;

    static constexpr int CLOCK_INT_CW =                           0x1d;

    enum Errors{
        ERR_OK,
        ERR_OPEN,
        ERR_WRITE_REG,
        ERR_READ_REG,
        ERR_SIG
    };


protected:
    /* structuries for signal and action */
    static ixpio_signal_t sig;
    static struct sigaction act, act_old;

    /* counter accamulator */
    unsigned long counter_accamulate {0};
    static unsigned int overflow;

    static void sig_handler(int isig);

    void setCounter1(uint16_t value);
    uint16_t getCounter1(void);

    /* low level */
    void writeRegister(unsigned int regID, unsigned int value);
    unsigned int readRegister(unsigned int regID);

public:
    Pex48Device(std::string path_to_device);
    ~Pex48Device(void);

    void startCounter(void);
    void stopCounter(void);

    unsigned long getCounterValue(void){
        return (overflow<<16|getCounter1());
    }


    int getErrno(void){return _errno;}

};


#endif //PEX48DEVICE_PEX48DEVICE_H
