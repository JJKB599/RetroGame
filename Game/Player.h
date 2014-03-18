#ifndef PLAYER_H
#define PLAYER_H

// All of Gosu.
#include <Gosu/Gosu.hpp>
// To safely include std::tr1::shared_ptr
#include <Gosu/TR1.hpp> 
// Makes life easier for Windows users compiling this.
#include <Gosu/AutoLink.hpp>

#include <cmath>
#include <cstdlib>
#include <list>
#include <sstream> // For int <-> string conversion
#include <vector>

#include "Enemy.h"
#include "Shot.h"

class Player
{
    /* CONSTANTS */

	static const int NUMBER_OF_FRAMES = 3;			// number of frames per walk cycle

	static const int LEFT_GUN_DOWN_CYCLE = 0;		// starting position of left gun down walk cycle = 0 * NUMBER_OF_FRAMES
	static const int RIGHT_GUN_DOWN_CYCLE = 1;		// starting position of right gun down walk cycle = 1 * NUMBER_OF_FRAMES
	static const int LEFT_GUN_FORWARD_CYCLE = 2;	// starting position of left gun forward walk cycle = 2 * NUMBER_OF_FRAMES
	static const int RIGHT_GUN_FORWARD_CYCLE = 3;	// starting position of right gun forward walk cycle = 3 * NUMBER_OF_FRAMES
	static const int LEFT_GUN_UP_CYCLE = 4;			// starting position of left gun up walk cycle = 4 * NUMBER_OF_FRAMES
	static const int RIGHT_GUN_UP_CYCLE = 5;		// starting position of right gun up walk cycle = 5 * NUMBER_OF_FRAMES
	static const int DYING_CYCLE = 6;				// starting position of dying cycle = 6 * NUMBER_OF_FRAMES

	static const int GUN_DOWN = 0;
	static const int GUN_FORWARD = 1;
	static const int GUN_UP = 2;

	
	Animation& animation;
	int posX, posY;

	int currentWalkCycle;
	int currentDirection;
	int currentWalkCycleDirection;
	int gunPosition;
	bool standingStill;

	bool dying;

    public:
		Player(Animation& animation);

		int x() const;
		int y() const;
		bool isDying() const;

		int shotDirection() const;

        void warp(int x, int y);
        void moveLeft();
        void moveRight();
		void moveUp();
        void moveDown();

		void moveGunUp();
		void moveGunDown();

		void shoot(Shot& shot);

		void checkForEnemyCollisions(std::list<Enemy>& enemies);

        void draw();

	private:
		void changeWalkCycle();
};

#endif
