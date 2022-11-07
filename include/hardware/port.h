#ifndef __SMHOS__HARDWARE__PORT_H
#define __SMHOS__HARDWARE__PORT_H

#include <common/types.h>

namespace smhos {
  namespace hardware {
    class Port {
      protected:
        smhos::common::uint16_t portnumber;
        Port(smhos::common::uint16_t portnumber);
        ~Port();
    };

    class Port8Bit : public Port {
      public:
        Port8Bit(smhos::common::uint16_t portnumber);
        ~Port8Bit();
        virtual void Write(smhos::common::uint8_t data);
        virtual smhos::common::uint8_t Read();
    };

    class Port8BitSlow : public Port8Bit{
      public:
        Port8BitSlow(smhos::common::uint16_t portnumber);
        ~Port8BitSlow();
        virtual void Write(smhos::common::uint8_t data);
    };

    class Port16Bit : public Port {
      public:
        Port16Bit(smhos::common::uint16_t portnumber);
        ~Port16Bit();
        virtual void Write(smhos::common::uint16_t data);
        virtual smhos::common::uint16_t Read();
    };

    class Port32Bit : public Port {
      public:
        Port32Bit(smhos::common::uint16_t portnumber);
        ~Port32Bit();
        virtual void Write(smhos::common::uint32_t data);
        virtual smhos::common::uint32_t Read();
    };
  }
}
#endif
