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
* Asteroids Model
*
* model.h
* This file contains declarations to be shared between several source files, for handling in-game elements and their movement.
*/

/* Asteroids Model */
struct point {
    float x, y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Initial struct types */
struct ship {
    float heading;
    coordinate_t p;
    vector_t v;
    int shield;
    int score;
};

/* Initial structs for building linked lists */
typedef struct missile {
    float heading;
    coordinate_t p;
    vector_t v;
    float timeToLive;
    struct missile* next;
} missile_t;

typedef struct rock {
    float heading;
    coordinate_t p;
    vector_t v;
    float timeToLive;
    struct rock* next;
} rock_t;

void physics(void);
void shipTurnLeft();
void shipTurnRight();
void shipMoveBackward();
void shipMoveForward();
void shipFire();

void initialise_missiles();
void free_missile_node(missile_t* node);
missile_t* allocate_missile_node(void);
void initialise_rocks();
void free_rock_node(rock_t* node);
rock_t* allocate_rock_node(void);
