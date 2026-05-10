#ifndef PIC_H
#define PIC_H

void pic_eoi(unsigned char irq);
void enable_keyboard();
void pic_remap();

#endif
