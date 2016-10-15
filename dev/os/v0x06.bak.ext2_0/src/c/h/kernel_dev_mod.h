#ifndef _KERNEL_DEV_MOD_H_
#define _KERNEL_DEV_MOD_H_

#include "kernel.h"
// ====================================================================
// ALT, one for CONTROL, and one for SHIFT. It's also a good
// idea to have one for CAPSLOCK, NUMLOCK, and SCROLLLOCK
int32_t _inportb_input;

void kb_install();

void kb_handler( REG *r);










// ====================================================================
#endif



