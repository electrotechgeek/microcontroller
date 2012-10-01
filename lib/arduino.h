// Jason Dreisbach
// Jason Descenzo
// Arduino Library

#ifndef ARDUINO_H
#define ARDUINO_H

//Define new datatypes as easier shorthand
typedef unsigned char u08; //!< Unsigned 8-bit integer, range: 0 to +255
typedef signed char   s08; //!< Signed 8-bit integer, range: -128 to +127
typedef unsigned int  u16; //!< Unsigned 16-bit integer, range: 0 to +65,535
typedef signed int    s16; //!< Signed 16-bit integer, range: -32,768 to +32,767
typedef unsigned long u32; //!< Unsigned 32-bit integer, range: 0 to +4,294,967,295
typedef signed long   s32; //!< Signed 32-bit integer, range: -2,147,483,648 to +2,147,483,647

//Bit manipulation macros
#define sbi(a, b) ((a) |= 1 << (b))       //!< Sets bit b in variable a.
#define cbi(a, b) ((a) &= ~(1 << (b)))    //!< Clears bit b in variable a.
#define tbi(a, b) ((a) ^= 1 << (b))       //!< Toggles bit b in variable a.
#define gbi(a, b) ((a) & (1 << (b)))      //!< Gets bit b in variable a (masks out everything else).
#define gbis(a, b) (gbi((a), (b)) >> (b)) //!< Gets bit b in variable a and shifts it to the LSB.

#include <avr/io.h>
#include <util/delay.h>
#include <lcd.h>

#endif /* ARDUINO_H */