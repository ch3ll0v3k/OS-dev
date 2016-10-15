#ifndef _KERNEL_ISR_H_
#define _KERNEL_ISR_H_

#include "kernel.h"

// ====================================================================
void isr0();  void isr1();  void isr2();  void isr3();  void isr4();
void isr5();  void isr6();  void isr7();  void isr8();  void isr9();
void isr10(); void isr11(); void isr12(); void isr13(); void isr14();
void isr15(); void isr16(); void isr17(); void isr18(); void isr19();
void isr20(); void isr21(); void isr22(); void isr23(); void isr24();
void isr25(); void isr26(); void isr27(); void isr28(); void isr29();
void isr30(); void isr31();

extern void irq0();  // IRQ 0  => System timer          This interrupt is reserved for the internal system timer. It is never available to peripherals or other devices.
extern void irq1();  // IRQ 1  => Keyboard              This interrupt is reserved for the keyboard controller. Even on devices without a keyboard, this interrupt is exclusively for keyboard input.
extern void irq2();  // IRQ 2  => Cascade  IRQs 8-15    This interrupt cascades the second interrupt controller to the first.
extern void irq3();  // IRQ 3  => 2d serial port (COM2) The interrupt for the second serial port and often the default interrupt for the fourth serial port (COM4).
extern void irq4();  // IRQ 4  => 1t serial port (COM1) This interrupt is normally used for the first serial port. On devices that do not use a PS/2 mouse, this interrupt is almost always used by the serial mouse. This is also the default interrupt for the third serial port (COM3).
extern void irq5();  // IRQ 5  => Sound card            This interrupt is the first choice that most sound cards make when looking for an IRQ setting.
extern void irq6();  // IRQ 6  => Floppy controller     This interrupt is reserved for the floppy disk controller.
extern void irq7();  // IRQ 7  => First parallel port   This interrupt is normally reserved for the use of the printer. If a printer is not being used, this interrupt can be used for other devices that use parallel ports.
extern void irq8();  // IRQ 8  => Real-time clock       This interrupt is reserved for the system's real-time clock timer and can not be used for any other purpose.
extern void irq9();  // IRQ 9  => Open interrupt        This interrupt is typically left open on devices for the use of peripherals.
extern void irq10(); // IRQ 10 => Open interrupt        This interrupt is typically left open on devices for the use of peripherals.
extern void irq11(); // IRQ 11 => Open interrupt        This interrupt is typically left open on devices for the use of peripherals.
extern void irq12(); // IRQ 12 => PS/2 mouse            This interrupt is reserved for the PS/2 mouse on machines that use one. If a PS/2 mouse is not used, the interrupt can be used for other peripherals, such as network card.
extern void irq13(); // IRQ 13 => FLOAT unit/coprocessor This interrupt is reserved for the integrated floating point unit. It is never available to peripherals or other devices as it is used exclusively for internal signaling.
extern void irq14(); // IRQ 14 => Primary IDE channel   This interrupt is reserved for use by the primary IDE controller. On systems that do not use IDE devices, the IRQ can be used for another purpose.
extern void irq15(); // IRQ 15 => Secondary IDE channel This interrupt is reserved for use by the secondary IDE controller.

// ====================================================================
#endif