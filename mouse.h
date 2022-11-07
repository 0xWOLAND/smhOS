#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "types.h"
#include "driver.h"


class MouseEventHandler {
    public:
        MouseEventHandler();
        virtual void onActivate();
        virtual void onMouseDown(uint8_t button);
        virtual void onMouseUp(uint8_t button);
        virtual void onMouseMove(int x, int y);
};

class MouseDriver : public InterruptHandler, public Driver {
    Port8Bit dataport;
    Port8Bit commandport;

    MouseEventHandler* handler;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
    public:
        MouseDriver(InterruptManager* manager, MouseEventHandler* handler);
        ~MouseDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
        virtual void Activate();
};
#endif