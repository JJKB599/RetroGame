#ifndef SHOT_H
#define SHOT_H

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

#include "Direction.h"
#include "Enemy.h"

class Shot
{
	/* CONSTANTS */

	static const int NUMBER_OF_FRAMES = 4;	// number of frames per cycle

	static const int RIGHT_SHOT_CYCLE = 0;		// starting position of right shot cycle = 0 * NUMBER_OF_FRAMES
	static const int RIGHT_UP_SHOT_CYCLE = 1;	// starting position of right-up shot cycle = 1 * NUMBER_OF_FRAMES
	static const int RIGHT_DOWN_SHOT_CYCLE = 2;	// starting position of right-down shot cycle = 2 * NUMBER_OF_FRAMES
	static const int LEFT_SHOT_CYCLE = 3;		// starting position of left shot cycle = 3 * NUMBER_OF_FRAMES
	static const int LEFT_UP_SHOT_CYCLE = 4;	// starting position of left-up shot cycle = 4 * NUMBER_OF_FRAMES
	static const int LEFT_DOWN_SHOT_CYCLE = 5;	// starting position of left-down shot cycle = 5 * NUMBER_OF_FRAMES
	
	
	Animation& animation;
	int posX, posY;
	bool isActive;
	unsigned long startTime;
	int shotCycle;

	public:
		Shot(Animation& animation);
		int x() const;
		int y() const;
		bool active() const;
		void activate(int x, int y, int shotDirection, int playerWalkCycleDirection, std::list<Enemy>& enemies, int& score);
		void draw();
};

#endif
