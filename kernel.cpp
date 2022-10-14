#include "types.h"
#include "gdt.h"

void printf(char* str){
  uint16_t* video_memory = (uint16_t*) 0xb8000;
  static uint8_t x = 0, y = 0;
  for(int i = 0; str[i] != '\0'; i++){
    switch (str[i])
    {
      case '\n':
          y++;
          x=0;
        break;
      default:
        video_memory[80 * y + x] = (video_memory[80 * y + x] & 0xFF00) | str[i];
        x++;
        break;
    }
    if(x >= 80){
      y++;
      x=0;
    }
    if(y >= 25){
      for(y = 0; y < 25; y++){
        for(x = 0; x < 80; x++){
          video_memory[80 * y + x] = (video_memory[80 * y + x] & 0xFF00) | ' ';
        }
      }
      y = 0;
      x = 0;
    }
  }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors() {
  for(constructor* i = &start_ctors; i != &end_ctors; i++){
    (*i)();
  }
}


extern "C" void kernel_main(void* multiboot_struct, uint32_t magic_number) {

  printf("THIS WORKS\n");
  printf("THIS WORKS\n");
  GlobalDescriptorTable gdt;
  while(1);
}
