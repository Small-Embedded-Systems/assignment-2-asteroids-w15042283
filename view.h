/**
* Faculty of Engineering & Environment
* CM0506 - Small Embedded Systems
* Assignment 2 of 2 - Asteroids
*
* Authors
* Alain Dobeli / W15039869
* Tom Batchelor / W15042283
* 
* Version
* 08.03.2017
*
*
*
* Asteroids View
*
* view.h
* This file contains declarations to be shared between several source files, for handling the displayed content of the Asteroids game.
*/

/* Asteroids View */
/* support double buffering */
void init_DBuffer(void);
void swap_DBuffer(void);

void draw(void);
extern bool inPlay;

/* Heart Bitmap */
#define heart_width 7
#define heart_height 8
const unsigned char heart_bits[] = {
    0x36, 0x7f, 0x7f, 0x7f, 0x7f, 0x3e, 0x1c, 0x08,
};

/* Rock Bitmap */
#define rock_width 40
#define rock_height 40
const unsigned char rock_bits[] = {
    0x00, 0xC0, 0xFF, 0x01, 0x00, 0x00, 0x78, 0x00, 0x0F, 0x00, 0x00, 0x0C, 
    0x00, 0x18, 0x00, 0x00, 0x06, 0x00, 0x30, 0x00, 0x00, 0x02, 0x00, 0x20, 
    0x00, 0x00, 0x03, 0x00, 0x60, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x80, 
    0x01, 0x00, 0xC0, 0x00, 0x80, 0x00, 0x00, 0x80, 0x07, 0x80, 0x00, 0x00, 
    0x00, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x30, 
    0xC0, 0x00, 0x00, 0x00, 0x60, 0x70, 0x00, 0x00, 0x00, 0x40, 0x1C, 0x00, 
    0x00, 0x00, 0x40, 0x06, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 
    0x60, 0x03, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00, 0x18, 0x03, 
    0x00, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x0C, 0x06, 0x00, 0x00, 
    0x00, 0x04, 0x04, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x00, 0x00, 0x00, 0x04, 
    0x08, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x18, 0x10, 0x00, 
    0x00, 0x00, 0x30, 0x10, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 
    0x40, 0x20, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x00, 0x00, 0x00, 0x80, 0x30, 
    0x00, 0x00, 0x00, 0x80, 0x18, 0x00, 0x00, 0x00, 0x80, 0x0C, 0x00, 0x00, 
    0x00, 0xBE, 0x06, 0xE0, 0x03, 0x00, 0xE3, 0x02, 0x30, 0x0E, 0x80, 0x01, 
    0x02, 0x18, 0x38, 0x80, 0x00, 0x06, 0x0E, 0xE0, 0xC0, 0x00, 0x8C, 0x03, 
    0x80, 0x7F, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00,
};
