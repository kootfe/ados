#include "../helpers/helper.h"
#include "../shell/shell.h"
#include <stdint.h>

static const char scancode_map[128] = {
    0,   0,  '1','2','3','4','5','6','7','8','9','0','-','=', '\b',   0,
  'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,  'a','s',
  'd','f','g','h','j','k','l',';','\'','`', 0, '\\','z','x','c','v',
  'b','n','m',',','.','/', 0,  '*', 0,  ' '
};

void keyboard_handler()
{
    uint8_t sc = inb(0x60);
    if (sc & 0x80) return;
    char c = scancode_map[sc];
    if (c) shell_feed(c);
}
