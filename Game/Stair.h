#ifndef STAIR_H
#define STAIR_H

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

class Stair
{
	/* CONSTANTS */

	int posX, posY;
	bool dir;

	public:
		Stair(int xPos, int yPos, bool d);
    Stair();
		int x() const;
		int y() const;
    bool direction() const;
};

#endif
