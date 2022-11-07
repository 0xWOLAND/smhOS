#include <drivers/mouse.h>
using namespace smhos::common;
using namespace smhos::drivers;
using namespace smhos::hardware;

void printf(char*);

MouseEventHandler::MouseEventHandler() {
}

void MouseEventHandler::onActivate(){
}
void MouseEventHandler::onMouseDown(uint8_t button){
}
void MouseEventHandler::onMouseUp(uint8_t button){
}
void MouseEventHandler::onMouseMove(int x, int y){
}
MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler):
    InterruptHandler(0x2C, manager),
    dataport(0x60),
    commandport(0x64)
{
    this->handler = handler;
}

void MouseDriver::Activate() {
    offset = 0;
    buttons = 0;

    if(handler != 0)
        handler->onActivate();
    commandport.Write(0xA8); //activate interrupts
    commandport.Write(0x20); // get current state
    uint8_t status = dataport.Read() | 2;
    commandport.Write(0x60); // set state
    dataport.Write(status);
    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}

MouseDriver::~MouseDriver(){

}


uint32_t MouseDriver::HandleInterrupt(uint32_t esp){
    uint8_t status = commandport.Read();
    if(!(status & 0x20) || handler  == 0)
        return esp;
    

    buffer[offset] = dataport.Read();
    offset = (offset + 1) % 3;

    if(offset == 0){
        if(buffer[1] != 0 || buffer[2] != 0){

            handler->onMouseMove(buffer[1], buffer[2]);

        }
        for(uint8_t i = 0; i < 3; i++){
            if((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i))){
                if(buttons & (0x1 << i))
                    handler->onMouseUp(i+1);
                else
                    handler->onMouseDown(i+1);
            }
        }
        buttons = buffer[0];
    }
    return esp;
}