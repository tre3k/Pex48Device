//
// Created by Kirill Pshenichnyi pshcyrilL@mail.ru a on 24.09.2020.
//

#include "Pex48Device.h"

/* static members */
unsigned int Pex48Device::overflow {0};
ixpio_signal_t Pex48Device::sig;
struct sigaction Pex48Device::act;
struct sigaction Pex48Device::act_old;

void Pex48Device::writeRegister(unsigned int regID, unsigned int value){
    if(_errno!=Errors::ERR_OK) return;
    ixpio_reg_t reg;
    reg.id = regID;
    reg.value = value;

    if(ioctl(_fd, IXPIO_WRITE_REG, &reg)){
        sigaction(SIGALRM, &act_old, NULL);
        close(_fd);
        _errno = Errors::ERR_WRITE_REG;
    }
    return;
}

unsigned int Pex48Device::readRegister(unsigned int regID){
    if(_errno!=Errors::ERR_OK) return 0xfffffffe;
    ixpio_reg_t reg;
    reg.id = regID;
    reg.value = 0;

    if(ioctl(_fd, IXPIO_READ_REG, &reg)){
        sigaction(SIGALRM, &act_old, NULL);
        close(_fd);
        _errno = Errors::ERR_READ_REG;
        return -1;
    }

    return reg.value;
}

/* Interrupt handler */
void Pex48Device::sig_handler(int isig) {
    overflow += 1;
    std::cout << "overflow: " << overflow << "\n";
}

/* constructor */
Pex48Device::Pex48Device(std::string path_to_device) {
    _fd = open(path_to_device.c_str(),O_RDWR);
    std::cout << "FD: " << _fd << "\n";
    if(_fd < 0){
        _errno = Errors::ERR_OPEN;
        return;
    }
}

Pex48Device::~Pex48Device() {
    sigaction(SIGALRM, &act_old, NULL);
    close(_fd);
}

void Pex48Device::setCounter1(uint16_t value){
    writeRegister(COUNTER0_REG,value & 0x00ff);
    writeRegister(COUNTER0_REG,(value & 0xff00)>>8);
    return;
}

uint16_t Pex48Device::getCounter1() {
    uint16_t retval {0};
    uint8_t c_l {0}, c_h{0};

    c_l = readRegister(COUNTER0_REG)&0xff;
    c_h = readRegister(COUNTER0_REG)&0xff;

    retval = 65535-((c_h<<8)|c_l);
    return retval;
}

void Pex48Device::startCounter() {
    counter_accamulate = 0;
    overflow = 0;
    /* set Signal action */
    act.sa_handler = Pex48Device::sig_handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGALRM);
    sigaction(SIGALRM,&act,&act_old);

    /* enable INT_CHAN_2 interrupt */
    writeRegister(CLOCK_INT_CW,0x15);          // select 32kHz for timer and disable PC3 interrupts
    writeRegister(INT_MASK_REG,0x04);          // Enable INT_CHAN_2 interrupt

    /* set Signal */
    sig.sid = SIGALRM;
    sig.pid = getpid();
    sig.is = 0x04; //INT_CHAN_2
    sig.edge = 0x04;
    sig.bedge = 0;
    if(ioctl(_fd,IXPIO_SIG,&sig)){
        sigaction(SIGALRM, &act_old, NULL);
        close(_fd);
        _errno = Errors::ERR_SIG;
    }

    writeRegister(CW_8254,0x36);               // Binary count counter0 mode 3
    setCounter1(0xffff);
}

void Pex48Device::stopCounter() {
    writeRegister(CW_8254,0x30);
    sigaction(SIGALRM, &act_old, NULL);
    counter_accamulate = (overflow<<16|getCounter1());
    setCounter1(0xffff);
}