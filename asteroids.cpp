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
* Asteroids
*
* asteroids.cpp
* This file contains the source code for handling the main initialisation of the Asteroids game.
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
#include "controller.h"
#include "model.h"
#include "view.h"

/*******************************************
 *                                         *
 *       DECLARATIONS AND VARIABLES        *
 *                                         *
 ******************************************/

/* Global variables, data structures, and function prototypes that are used to set the initial game settings and state */
float Dt = 0.01f;
float elapsed_time = 0.0f;
int score = 0;
int lives = 3;
bool inPlay;
bool running = false;
struct ship player;
struct missile* shots = NULL;
struct rock* asteroids = NULL;
void clickUserButtonToStart();
void initialiseGame();

/* Ticker objects to manage background tasks alongside the main-loop, following a Model-View-Controller pattern */
Ticker model, view, controller;

/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10, PullUp);

/*******************************************
 *                                         *
 *             CORE GAME LOGIC             *
 *                                         *
 ******************************************/

/* This is the main method which directly and indirectly integrates all functions and methods with each other in
an appropriate sequence, making the game playable. */
int main()
{
    /* Initialize the double buffer for smooth animation */
    init_DBuffer();
    view.attach(draw, 0.055);
    model.attach(physics, Dt);
    controller.attach(controls, 0.1);
		
    /* A single while() loop within the single main() function, as required */
    while (true) {
			initialiseGame();
        while (lives > 0) {
            clickUserButtonToStart();
            inPlay = true;
            while (inPlay) {
              /* Do nothing here; game in progress */
            }
        }
        /* Stop the game when all lives are lost */
        running = false;
    }
}

/* This method will initially set up the ship's position (center on x-axis and y-axis), set the ship's velocity
on the x-axis and y-axis to 0 (not moving), set the ship's heading north, and also apply a full shield that can
absorb 3 hits. In addition, the ship's missiles are initialised aswell as the rocks (asteroids). */
void initialiseGame()
{
    player.p.x = 200; /* The ship's initial x-axis position */
    player.p.y = 136; /* The ship's initial y-axis position */
    player.v.x = 0; /* The ship's initial x-axis velocity */
    player.v.y = 0; /* The ship's initial y-axis velocity */
    player.heading = 0.0; /* The ship's initial heading direction */
    player.shield = 3; /* The ship's initial shield HP */
    initialise_missiles();
    initialise_rocks();
}

/* Method that checks whether the user clicks the user button on the back of the board, upon which a game is started/continued. */
void clickUserButtonToStart()
{
    while (userbutton.read()) { /* Remember: 1 is not pressed */
        running = false;
        wait_ms(100);
    }
    running = true;
}
