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
* asteroids.h
* This file contains declarations to be shared between several source files, for handling the main initialisation of the Asteroids game.
*/

/* Variables defining the game state */
extern const float Dt; /* Time step for physics, needed for consistent motion */
extern float elapsed_time; /* time this ship has been active */
extern int score; /* total score so far */
extern int lives; /* lives remaining */
extern bool running; /* status of the game */

/* Data structures used to set the game settings and state */
extern struct ship player; /* the ship in-game object */
extern struct missile* shots; /* array of missiles / pointer to linked-list */
extern struct rock* asteroids; /* array of rocks / pointer to linked-list */
