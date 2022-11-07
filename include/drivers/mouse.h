#ifndef __SMHOS__DRIVERS__MOUSE_H
#define __SMHOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <hardware/port.h>


namespace smhos {
    namespace drivers {
        class MouseEventHandler {
            public:
                MouseEventHandler();
                virtual void onActivate();
                virtual void onMouseDown(smhos::common::uint8_t button);
                virtual void onMouseUp(smhos::common::uint8_t button);
                virtual void onMouseMove(int x, int y);
        };

        class MouseDriver : public InterruptHandler, public Driver {
            Port8Bit dataport;
            Port8Bit commandport;

            MouseEventHandler* handler;
            smhos::common::uint8_t buffer[3];
            smhos::common::uint8_t offset;
            smhos::common::uint8_t buttons;
            public:
                MouseDriver(smhos::hardware::InterruptManager* manager, MouseEventHandler* handler);
                ~MouseDriver();
                virtual uint32_t HandleInterrupt(uint32_t esp);
                virtual void Activate();
        };
    }
}
#endif