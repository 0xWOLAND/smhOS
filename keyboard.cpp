#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager):
    InterruptHandler(0x21, manager),
    dataport(0x60),
    commandport(0x64)
{
    while(commandport.Read() & 0x1){
        dataport.Read();
    }
    commandport.Write(0xAE); // Tells PIC to start sending keyboard interrupts
    commandport.Write(0x20); // Give us your current state
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // Tells PIC to change current state
    dataport.Write(status);

    dataport.Write(0xF4); // Activates keyboard

}
KeyboardDriver::~KeyboardDriver(){

}

void printf(char*);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp){
    uint8_t key = dataport.Read();

    if(key < 0x80)

    switch(key){
        case 0xFA: break;
        case 0x45: case 0xC5: break;
        default:
            char* foo = " KEYBOARD 0x00";
            char* hex = "01234567890ABCDEF";
            foo[11] = hex[(key >> 4) & 0x0F];
            foo[12] = hex[key & 0x0F];
            printf(foo);
            break;
    }
    return esp;
}