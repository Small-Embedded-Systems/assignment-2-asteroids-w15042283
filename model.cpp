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
* model.cpp
* This file contains the source code for handling in-game elements and their movement.
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

/* Main Game Elements */
#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "view.h"

/*******************************************
 *                                         *
 *       DECLARATIONS AND VARIABLES        *
 *                                         *
 ******************************************/

/* Define the ship properties (ship drag factor, ship acceleration factor, maximum number of missiles and unused (free) missile nodes,
maximum number of rocks and unused (free) rock nodes) */
static const float shipDrag = 0.02;
static const float shipAcceleration = 1.0;
static const size_t MAXSize = 10;

static missile_t missile_data[MAXSize];
static missile_t* freeMissileNodes;
static rock_t rock_data[MAXSize];
static rock_t* freeRockNodes;

/* Method prototypes belonging to this class - used for managing the appearance and movement of models */
static void move_ship(ship, float x);
static void new_missile(struct missile *m);
static void update_missile_list(struct missile* list);
static void move_missiles(struct missile* list);
static struct rock* new_rocks();
static void update_rock_list(struct rock* list);
static void move_rocks(struct rock* list, float x);
static bool ship_hits(struct rock* list);
static void missiles_hit_rocks(struct missile* list, struct rock* r);

/*******************************************
 *                                         *
 *    GENERAL IN-GAME ELEMENTS HANDLING    *
 *                                         *
 ******************************************/

/* This is the main method of this class, responsible for integrating all other methods with each other in this class and
for the handling of in-game elements, elapsed time, scoring, and shield/lives on hit. */
void physics(void)
{
    if (running) {
        elapsed_time += Dt;
        move_ship(player, Dt);
        move_missiles(shots);
        asteroids = new_rocks();
        move_rocks(asteroids, Dt);
        missiles_hit_rocks(shots, asteroids);
        if (ship_hits(asteroids)) {
            if (player.shield <= 0) {
                inPlay = false;
                score += (int)elapsed_time; /* Convert elapsed time to integer to add it to the score, since it is actually a float. */
                lives--;
            } else {
                player.shield--;
            }
        }
        update_missile_list(shots);
        update_rock_list(asteroids);
    }
}

/*******************************************
 *                                         *
 *    SHIP NAVIGATION AND FIRE HANDLING    *
 *                                         *
 ******************************************/

/* Turn the ship's heading left */
void shipTurnLeft()
{
    player.heading -= 10.0f;
}

/* Turn the ship's heading right */
void shipTurnRight()
{
    player.heading += 10.0f;
}

/* Move the ship backward */
void shipMoveBackward()
{
    /* A normalised vector in travel direction is created */
    float xNormalised = sin(radians(player.heading));
    float yNormalised = cos(radians(player.heading));

    /* Make the x-velocity negative, adding to the velocity, causing the ship to move backward */
    player.v.x -= xNormalised * shipAcceleration;
    player.v.y += yNormalised * shipAcceleration;
}

/* Move the ship forward */
void shipMoveForward()
{
    /* A normalised vector in travel direction is created */
    float xNormalised = sin(radians(player.heading));
    float yNormalised = cos(radians(player.heading));

    /* Make the y-velocity negative, adding to the velocity, causing the ship to move forward */
    player.v.x += xNormalised * shipAcceleration;
    player.v.y -= yNormalised * shipAcceleration;
}

/* Fire a missile from the ship */
void shipFire()
{
    struct missile* newMissile = allocate_missile_node();
    if (newMissile) {
        newMissile->next = shots;
        shots = newMissile;
        /* Construct a new missile */
        new_missile(newMissile);
    }
    update_missile_list(shots);
}

/* Handle the movement of the ship, including drag and velocity */
void move_ship(ship, float x)
{
    /* The position of the ship is updated based on the velocity (speed of movement) */
    player.p.x += player.v.x;
    player.p.y += player.v.y;

    /* Add drag to the ship based on velocity */
    player.v.x = player.v.x - (player.v.x * shipDrag);
    player.v.y = player.v.y - (player.v.y * shipDrag);

    /* Make the ship wrap-around, keeping it in the game world */
    if (player.p.x < 0) {
        player.p.x += 400;
        }
    if (player.p.x >= 400) {
        player.p.x -= 400;
        }
    if (player.p.y < 0) {
        player.p.y += 272;
        }
    if (player.p.y >= 272) {
        player.p.y -= 272;
    }
}

/*******************************************
 *                                         *
 *          SHIP MISSILE HANDLING          *
 *                                         *
 ******************************************/

/* Initialise the ship's missiles using linked-list */
void initialise_missiles()
{
    int c;
    for (c = 0; c < (MAXSize - 1); c++) {
        missile_data[c].next = &missile_data[c + 1];
    }
    missile_data[c].next = NULL;
    freeMissileNodes = &missile_data[0];
}

/* Constructs new missiles, sets their position, velocity, and lifetime */
void new_missile(struct missile* m)
{
    m->heading = player.heading;
    m->p.x = player.p.x;
    m->p.y = player.p.y;
    m->v.x = sin(radians(m->heading));
    m->v.y = -cos(radians(m->heading));
    m->p.x += m->v.x * 20;
    m->p.y += m->v.y * 20;
    m->v.x *= 50;
    m->v.y *= 50;
    m->timeToLive = 20;
}

/* Frees nodes in the rock linked-list that contain expired rocks */
void free_missile_node(missile_t* node)
{
    node->next = freeMissileNodes;
    freeMissileNodes = node;
}

/* Allocate nodes in the missile linked-list for new missiles */
missile_t* allocate_missile_node(void)
{
    missile_t* node = NULL;
    if (freeMissileNodes) {
        node = freeMissileNodes;
        freeMissileNodes = freeMissileNodes->next;
    }
    return node;
}

/* Updates the missile held in the missile linked-list */
void update_missile_list(struct missile* list)
{
    for (; list; list = list->next) {
        if (list->next->timeToLive <= 0) {
            struct missile* expiredMissile = list->next;
            list->next = list->next->next;
            /* Free nodes with expired missiles */
            free_missile_node(expiredMissile);
        }
    }
}

/* Handle the movement and velocity of the missiles */
void move_missiles(struct missile* list)
{
    for (; list; list = list->next) {
        list->p.x += list->v.x * Dt;
        list->p.y += list->v.y * Dt;

        /* Make the missiles not wrap-around, they will not enter the game world again */
        if (list->p.y >= 272) {
            list->timeToLive = 0;
        }
        if (list->p.y < 0) {
            list->timeToLive = 0;
        }
        if (list->p.x >= 400) {
            list->timeToLive = 0;
        }
        if (list->p.x < 0) {
            list->timeToLive = 0;
        }

        list->timeToLive -= Dt;
    }
}

/*******************************************
 *                                         *
 *       ROCKS (ASTEROIDS) HANDLING        *
 *                                         *
 ******************************************/

/* Initialise the game's rocks using linked-list */
void initialise_rocks()
{
    int c;
    for (c = 0; c < (MAXSize - 1); c++) {
        rock_data[c].next = &rock_data[c + 1];
    }
    rock_data[c].next = NULL;
    freeRockNodes = &rock_data[0];
}

/* Constructs new rocks, sets their lifetime, and adds them to the rock linked-list */
struct rock* new_rocks()
{
    int c;
    for (c = 0; c < MAXSize - 1; c++) {
        struct rock* addRock = allocate_rock_node();
        if (addRock) {
            addRock->next = asteroids;
            asteroids = addRock;

            /* Define the initial asteroid values */
            int rockPosition = randrange(1, 5);
            switch (rockPosition) {
            case 1:
                /* Rock - From Left Side of Screen */
                addRock->p.x = randrange(0, 100);
                addRock->p.y = randrange(0, 272);
                addRock->v.x = 1;
                if (addRock->p.y > 136) {
                    addRock->v.y = -10;
                }
                if (addRock->p.y <= 136) {
                    addRock->v.y = 10;
                }
                break;
            case 2:
                /* Rock - From Right Side of Screen */
                addRock->p.x = randrange(280, 380);
                addRock->p.y = randrange(0, 272);
                addRock->v.x = -1;
                if (addRock->p.y > 136) {
                    addRock->v.y = -10;
                }
                if (addRock->p.y <= 136) {
                    addRock->v.y = 10;
                }
                break;
            case 3:
                /* Rock - From Top of Screen */
                addRock->p.x = randrange(0, 380);
                addRock->p.y = randrange(0, 100);
                addRock->v.y = 1;
                if (addRock->p.x > 200) {
                    addRock->v.x = -10;
                }
                if (addRock->p.y <= 200) {
                    addRock->v.x = 10;
                }
                break;
            case 4:
                /* Rock - From Bottom of Screen */
                addRock->p.x = randrange(0, 380);
                addRock->p.y = randrange(172, 272);
                addRock->v.y = 1;
                if (addRock->p.x > 200) {
                    addRock->v.x = -10;
                }
                if (addRock->p.y <= 200) {
                    addRock->v.x = 10;
                }
                break;
            }
            addRock->timeToLive = 2000;
        }
        update_rock_list(asteroids);
    }
    return asteroids;
}

/* Frees nodes in the rock linked-list that contain expired rocks */
void free_rock_node(rock_t* node)
{
    node->next = freeRockNodes;
    freeRockNodes = node;
}

/* Allocate nodes in the rock linked-list for new rocks */
rock_t* allocate_rock_node(void)
{
    rock_t* node = NULL;
    if (freeRockNodes) {
        node = freeRockNodes;
        freeRockNodes = freeRockNodes->next;
    }
    return node;
}

/* Updates the rocks held in the rock linked-list */
void update_rock_list(struct rock* list)
{
    for (; list; list = list->next) {
        if (list->next->timeToLive <= 0) {
            struct rock* expiredRock = list->next;
            list->next = list->next->next;
            /* Free nodes with expired rocks */
            free_rock_node(expiredRock);
        }
    }
}

/* Handle the movement and velocity of the rocks */
void move_rocks(struct rock* list, float x)
{
    for (; list; list = list->next) {
        list->p.x += list->v.x * x;
        list->p.y += list->v.y * x;

        /* Make the rocks wrap-around, keeping them in the game world */
        if (list->p.y >= 272) {
            list->p.y -= 272;
        }
        if (list->p.y < 0) {
            list->p.y += 272;
        }
        if (list->p.x >= 380) {
            list->p.x -= 380;
        }
        if (list->p.x < 0) {
            list->p.x += 380;
        }

        list->timeToLive -= Dt;
    }
}

/*******************************************
 *                                         *
 *            COLLISION HANDLING           *
 *                                         *
 ******************************************/

/* Check if a collision between the ship and a rock has occurred. If there is a collision, remove the rock responsible for the collision.*/
bool ship_hits(struct rock* list)
{
    for (; list; list = list->next) {
        float rShip = 7.9; /* This is an approximate value of the radius of the circle around the ship. */
        float rRock = 20; /* This is a value of the radius of the circle around the rock. */
        float dx = list->p.x - player.p.x;
        float dy = list->p.y - player.p.y;
        float distance = sqrt(dx * dx + dy * dy);
        /* If collision detected, remove rock */
        if (distance < (rShip + rRock)) {
            list->timeToLive = 0;
            return true;
        }
    }
    return false;
}

/* Check if a collision between a missle and a rock has occurred. If there is a collision, remove the missile and the rock responsible for thr collision. */
void missiles_hit_rocks(struct missile* list, struct rock* r)
{
    for (; list; list = list->next) {
        for (; r; r = r->next) {
            float rMissile = 1; /* This is a value of the radius of the circle around the missile. */
            float rRock = 20; /* This is a value of the radius of the circle around the rock. */
            float dx = list->p.x - r->p.x;
            float dy = list->p.y - r->p.y;
            float distance = sqrt(dx * dx + dy * dy);
            /* If collision detected, remove missile and rock */
            if (distance < (rMissile + rRock)) {
                list->timeToLive = 0;
                r->timeToLive = 0;
            }
        }
    }
}
