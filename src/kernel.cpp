
#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>

#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace smhos;
using namespace smhos::common;
using namespace smhos::drivers;
using namespace smhos::hardware;
void initScreen(){
  
  static uint16_t* VideoMemory = (uint16_t*)0xb8000;
  uint8_t x,y;
  for(y = 0; y < 25; y++)
      for(x = 0; x < 80; x++)
          VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
  x = 0;
  y = 0;
}

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;
    
    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }
        
        if(x >= 80)
        {
            x = 0;
            y++;
        }
        
        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

void printfHex(uint8_t key){
    char* foo = "00";
    char* hex = "01234567890ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0F];
    printf(foo);
}


class PrintfKeyboardEventHandler: public KeyboardEventHandler {
    public: 
        void onKeyDown(char c){
            char* foo = " ";
            foo[0] = c;
            printf(foo);
        }

};

class MouseToConsole: public MouseEventHandler {
    private:
        int8_t x, y;
    public:
        MouseToConsole() {
        }
        virtual void onActivate() {
            static uint16_t* VideoMemory = (uint16_t*)0xb8000;
            x = 40, y = 12;
            VideoMemory[80 * y + x] = 
                ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                ((VideoMemory[80 * y + x] & 0x0F00) << 4) | 
                ((VideoMemory[80 * y + x] & 0x00FF));
        }
        
        virtual void onMouseMove(int xoffset, int yoffset) {

            static uint16_t* VideoMemory = (uint16_t*)0xb8000;
                VideoMemory[80 * y + x] = 
                    ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                    ((VideoMemory[80 * y + x] & 0x0F00) << 4) | 
                    ((VideoMemory[80 * y + x] & 0x00FF));

                x += xoffset;
                if(x < 0) x = 0;
                if(x >= 80) x = 79;

                y -= yoffset;
                if(y < 0) y = 0;
                if(y >= 25) y = 24;

                VideoMemory[80 * y + x] = 
                    ((VideoMemory[80 * y + x] & 0x0F00) << 4) | 
                    ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                    ((VideoMemory[80 * y + x] & 0x00FF));
        }
};

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    initScreen();
    printf("KERNEL MAIN");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    DriverManager drvManager;
    printf("Initialize Hardware 1\n");

    MouseToConsole mouseHandler;
    MouseDriver mouse(&interrupts, &mouseHandler);
    drvManager.AddDriver(&mouse);
    
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);

    printf("Initialize Hardware 2\n");
    drvManager.ActivateAll();
    printf("Initialize Hardware 3\n");
    interrupts.Activate();

    while(1);
}