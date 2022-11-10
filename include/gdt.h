#ifndef __SMHOS__GDT_H
#define __SMHOS__GDT_H

#include <common/types.h>
namespace smhos {
    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor
            {
                private:
                    smhos::common::uint16_t limit_lo;
                   smhos::common::uint16_t base_lo;
                   smhos::common::uint8_t base_hi;
                   smhos::common::uint8_t type;
                   smhos::common::uint8_t limit_hi;
                   smhos::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(smhos::common::uint32_t base, smhos::common::uint32_t limit, smhos::common::uint8_t type);
                   smhos::common::uint32_t Base();
                   smhos::common::uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

           smhos::common::uint16_t CodeSegmentSelector();
           smhos::common::uint16_t DataSegmentSelector();
    };
}  
#endif