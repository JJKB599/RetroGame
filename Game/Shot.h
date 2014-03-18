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

	static const int LEFT_SHOT_CYCLE = LEFT;	// starting position of left shot cycle = LEFT * NUMBER_OF_FRAMES
	static const int RIGHT_SHOT_CYCLE = RIGHT;	// starting position of right shot cycle = RIGHT * NUMBER_OF_FRAMES
	static const int UP_SHOT_CYCLE = UP;		// starting position of up shot cycle = UP * NUMBER_OF_FRAMES
	static const int DOWN_SHOT_CYCLE = DOWN;	// starting position of down shot cycle = DOWN * NUMBER_OF_FRAMES
	
	
	Animation& animation;
	int posX, posY;
	bool isActive;
	unsigned long startTime;
	int shotDirection;

	public:
		Shot(Animation& animation);
		int x() const;
		int y() const;
		bool active() const;
		void activate(int x, int y, int direction, std::list<Enemy>& enemies);
		void draw();
};

#endif
