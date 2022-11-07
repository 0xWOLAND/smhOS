#ifndef __SMHOS__DRIVERS__KEYBOARD_H
#define __SMHOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <driver/driver.h>
#include <hardware/port.h>

namespace smhos {
    namespace drivers {
        class KeyboardEventHandler {
            public:
                KeyboardEventHandler();

                virtual void onKeyDown(char);
                virtual void onKeyUp(char);
        };

        class KeyboardDriver : public smhos::hardware::InterruptHandler, public Driver {
            smhos::hardware::Port8Bit dataport;
            smhos::hardware::Port8Bit commandport;

            KeyboardEventHandler* handler;
            public:
                KeyboardDriver(smhos::hardware::InterruptManager* manager, KeyboardEventHandler* handler);
                ~KeyboardDriver();
                virtual uint32_t HandleInterrupt(uint32_t esp);
                virtual void Activate();
        };
    }
}
#endif