#ifndef Utilities_h
#define Utilities_h

#include "Direction.h"
#include "Enemy.h"
#include "Environment.h"
#include "Player.h"

// Detect whether a collision occurred between the player and an enemy
bool collisionDidOccur(Player player, Enemy enemy);

// Check whether a player or enemy can move in a given direction
bool canMoveDirection(int x, int y, Environment environment, int direction);

#endif
