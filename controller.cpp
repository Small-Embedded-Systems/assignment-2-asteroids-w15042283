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
* Asteroids Controller
*
* controller.cpp
* This file contains the source code for handling user input controls.
*/

/*******************************************
 *                                         *
 *               HEADER FILES              *
 *                                         *
 ******************************************/

/* C Libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Hardware Platform Libraries */
#include <mbed.h>

/* Main Game Elements */
#include "asteroids.h"
#include "model.h"

/*******************************************
 *                                         *
 *       DECLARATIONS AND VARIABLES        *
 *                                         *
 ******************************************/

/* Initialise joystick */
typedef enum { left,
    down,
    right,
    up,
    centre } btnId_t;

/* Declare input object for joystick */
DigitalIn joystick[] = { P5_0, P5_1, P5_4, P5_2, P5_3 }; /* left, down, right, up, center */
static bool jsPrsdAndRlsd(btnId_t b);

/*******************************************
 *                                         *
 *           USER INPUT CONTROLS           *
 *                                         *
 ******************************************/

/* Turn, move, or fire a missile from the ship in a specific direction when the specific joystick button is pressed. */
void controls(void)
{
    if (running) {
        if (jsPrsdAndRlsd(left)) {
            shipTurnRight();
        }
        else if (jsPrsdAndRlsd(down)) {
            shipMoveBackward();
        }
        else if (jsPrsdAndRlsd(right)) {
            shipTurnLeft();
        }
        else if (jsPrsdAndRlsd(up)) {
            shipMoveForward();
        }
        else if (jsPrsdAndRlsd(centre)) {
            shipFire();
        }
    }
}

/* Read the states of the joystick buttons. While a button is pressed, set the pressed button value to 0, which causes the button press to be recognized, 
and execute a specific action in the method above while the game is in "running" state. */
bool jsPrsdAndRlsd(btnId_t b)
{
    bool result = false;
    uint32_t state;
    /* Initially all 1's: nothing pressed */
    static uint32_t savedState[5] = { 1, 1, 1, 1, 1 };
    state = joystick[b].read();
    if (savedState[b] == 0) {
        result = true;
    }
    savedState[b] = state;
    return result;
}
