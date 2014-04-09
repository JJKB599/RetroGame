#ifndef ITEM_H
#define ITEM_H

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

#include "Animation.h"

class Item
{
	int posX, posY;

	Animation& animation;

	int type;

	public:
		Item(Animation& animation, int itemType, int xPos, int yPos);
		int x() const;
		int y() const;
		int getType() const;
		void draw() const;
};

#endif
