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
* view.cpp
* This file contains the source code for handling the displayed content of the Asteroids game.
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
#include <string.h>

/* Hardware Platform Libraries */
#include <display.h>
#include <mbed.h>

/* Main Game Elements */
#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "view.h"

/* An assembly function within the assembly file bitflip.s, used to flip image bit patterns */
extern "C" uint8_t bitflip(uint8_t);

/*******************************************
 *                                         *
 *       DECLARATIONS AND VARIABLES        *
 *                                         *
 ******************************************/

/* Call a static method of class Display and return an object that manages the display screen */
Display* graphics = Display::theDisplay();

/* Method prototypes belonging to this class - used for managing the displayed content */
static void drawSideBar(int score, float elapsed_time, int lives);
static void drawShip(struct ship);
static void transformShipShape();
static void drawMissiles(struct missile* list);
static void drawRocks(struct rock* list);

/* Assembly function, methods, and variables used to flip image bit patterns */
uint8_t bitflip(uint8_t);
static void heartBitFlip();
static unsigned char flippedHeart[8];
static void rockBitFlip();
static unsigned char flippedRock[200];

/* Define the colors of the in-game elements (background, ship, missile, and rocks) */
static const colour_t background = rgb(0, 0, 0); /* Background Colour - Black */
static const colour_t shipColour = rgb(255, 255, 255); /* Ship Colour - White */
static const colour_t missileColour = rgb(255, 255, 255); /* Missile Colour - White */
static const colour_t rockColour = rgb(255, 255, 255); /* Rock Colour - White */

/* Coordinates used for creating the ship shape */
static const coordinate_t shape[] = {
    { 0.0f, -10.0f }, { 5.0f, 5.0f }, { -5.0f, 5.0f }
};
static coordinate_t transformedShipShape[3];

/*******************************************
 *                                         *
 *             DOUBLE-BUFFERING            *
 *                                         *
 ******************************************/

/* Initialise the double buffer */
void init_DBuffer(void)
{ /* Initialise the LCD driver to use the second frame in the buffer */
    uint16_t* bufferbase = graphics->getFb();
    uint16_t* nextbuffer = bufferbase + (480 * 272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

/* Swaps frames used by the LCD driver and the graphics object */
void swap_DBuffer(void)
{
    uint16_t* buffer = graphics->getFb();
    graphics->setFb((uint16_t*)LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

/*******************************************
 *                                         *
 *    GENERAL SCREEN ELEMENTS HANDLING     *
 *                                         *
 ******************************************/

/* This is the main method of this class, responsible for integrating all other methods with each other in this class and
displaying appropriate content. */
void draw(void)
{
    graphics->fillScreen(background);

    drawSideBar(score, elapsed_time, lives);
    drawMissiles(shots);
    drawRocks(asteroids);
    transformShipShape();
    drawShip(player);

    /* Define the title and supporting text (instructions) to display while a game is not running */
    if (!running) {
        switch (lives) {
        case 3:
        case 2:
        case 1:
            graphics->setTextSize(5);
            graphics->setCursor(65, 50);
            graphics->printf("ASTEROIDS");
            graphics->setTextSize(1);
            graphics->setCursor(60, 155);
            graphics->printf("Click the user pushbutton on the back to start.");
            break;
        case 0:
            graphics->setTextSize(5);
            graphics->setCursor(70, 50);
            graphics->printf("GAME OVER");
            graphics->setTextSize(2);
            graphics->setCursor(110, 100);
            graphics->printf("Final Score: %d", score);
            graphics->setTextSize(1);
            graphics->setCursor(50, 155);
            graphics->printf("Click the user reset button on the back to restart.");
            break;
        }
    }
    swap_DBuffer();
}

/* Draws a side bar to the right of the screen with information on the game (such as score, time played, lives left, and shield HP) */
void drawSideBar(int score, float elapsed_time, int lives)
{
    graphics->fillRect(400, 0, 80, 272, BLACK);
    graphics->setCursor(425, 5);
    graphics->printf("CM0506");
    graphics->setCursor(405, 15);
    graphics->printf("Assignment 2");
    graphics->setCursor(415, 25);
    graphics->printf("ASTEROIDS");
    graphics->setCursor(405, 101);
    graphics->printf("Score: %d", score);
    graphics->setCursor(405, 121);
    graphics->printf("Time : %.2f", elapsed_time);
    graphics->setCursor(405, 141);
    graphics->printf("Lives:");
    switch (lives) {
    case 3:
        heartBitFlip();
        graphics->drawBitmap(467, 141, flippedHeart, heart_width, heart_height, WHITE);
        graphics->drawBitmap(457, 141, flippedHeart, heart_width, heart_height, WHITE);
        graphics->drawBitmap(447, 141, flippedHeart, heart_width, heart_height, WHITE);
        break;
    case 2:
        graphics->drawBitmap(457, 141, flippedHeart, heart_width, heart_height, WHITE);
        graphics->drawBitmap(447, 141, flippedHeart, heart_width, heart_height, WHITE);
        break;
    case 1:
        graphics->drawBitmap(447, 141, flippedHeart, heart_width, heart_height, WHITE);
        break;
    case 0:
        break;
    }
    graphics->setCursor(405, 161);
    graphics->printf("Shield HP: ");
    graphics->setCursor(405, 165);
    switch (player.shield) {
    case 3:
        graphics->fillRect(405, 171, 70, 6, WHITE);
        graphics->fillRect(453, 172, 20, 4, WHITE);
        graphics->fillRect(430, 172, 20, 4, WHITE);
        graphics->fillRect(407, 172, 20, 4, WHITE);
        break;
    case 2:
        graphics->fillRect(405, 171, 70, 6, WHITE);
        graphics->fillRect(430, 172, 20, 4, WHITE);
        graphics->fillRect(407, 172, 20, 4, WHITE);
        break;
    case 1:
        graphics->fillRect(405, 171, 70, 6, WHITE);
        graphics->fillRect(407, 172, 20, 4, WHITE);
        break;
    case 0:
        graphics->fillRect(405, 171, 70, 6, WHITE);
        break;
    }
    graphics->setCursor(405, 240);
    graphics->printf("Authors");
    graphics->setCursor(405, 250);
    graphics->printf("A. Dobeli");
    graphics->setCursor(405, 260);
    graphics->printf("T. Batchelor");
}

/* Draw the ship in the game and the shield, if available */
void drawShip(struct ship)
{
    /* Define the colours of the shield around the ship */
    switch (player.shield) {
    case 3:
        graphics->drawCircle(player.p.x, player.p.y, 10, WHITE);
        break;
    case 2:
        graphics->drawCircle(player.p.x, player.p.y, 10, WHITE);
        break;
    case 1:
        graphics->drawCircle(player.p.x, player.p.y, 10, WHITE);
        break;
    case 0:
        //do nothing
        break;
    }
    /* Create the triangular shape of the ship */
    graphics->drawLine(player.p.x + transformedShipShape[0].x, player.p.y + transformedShipShape[0].y, player.p.x + transformedShipShape[1].x, player.p.y + transformedShipShape[1].y, shipColour);
    graphics->drawLine(player.p.x + transformedShipShape[1].x, player.p.y + transformedShipShape[1].y, player.p.x + transformedShipShape[2].x, player.p.y + transformedShipShape[2].y, shipColour);
    graphics->drawLine(player.p.x + transformedShipShape[2].x, player.p.y + transformedShipShape[2].y, player.p.x + transformedShipShape[0].x, player.p.y + transformedShipShape[0].y, shipColour);
}

/* Responsible for the transformation of the ship shape */
void transformShipShape()
{
    int i;
    for (i = 0; i < 3; i++) {
        transformedShipShape[i].x = (shape[i].x * cos(radians(player.heading))) - (shape[i].y * sin(radians(player.heading)));
        transformedShipShape[i].y = (shape[i].x * sin(radians(player.heading))) + (shape[i].y * cos(radians(player.heading)));
    }
}

/* Draw the missiles in the game */
void drawMissiles(struct missile* list)
{
    while (list) {
        graphics->fillCircle(list->p.x, list->p.y, 1, missileColour);
        list = list->next;
    }
}

/* Draw the rocks (asteroids) in the game */
void drawRocks(struct rock* list)
{
    rockBitFlip();
    while (list) {
        graphics->drawBitmap(list->p.x - 20, list->p.y - 20, flippedRock, rock_width, rock_height, rockColour);
        list = list->next;
    }
}

/*******************************************
 *                                         *
 *         IMAGE BIT FLIP HANDLING         *
 *                                         *
 ******************************************/

/* Flip image bit pattern of the heart image */
void heartBitFlip()
{
    int p;
    for (p = 0; p < 8; p++) {
        flippedHeart[p] = bitflip(heart_bits[p]);
    }
}

/* Flip image bit pattern of the rock image */
void rockBitFlip()
{
    int p;
    for (p = 0; p < 200; p++) {
        flippedRock[p] = bitflip(rock_bits[p]);
    }
}
