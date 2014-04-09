// All of Gosu.
#include <Gosu/Gosu.hpp>
// To safely include std::tr1::shared_ptr
#include <Gosu/TR1.hpp> 
// Makes life easier for Windows users compiling this.
#include <Gosu/AutoLink.hpp>

#include <Gosu/Color.hpp>

#include <cmath>
#include <cstdlib>
#include <list>
#include <sstream> // For int <-> string conversion
#include <vector>

#include "Animation.h"
#include "ZOrder.h"

#include "Item.h"


Item::Item(Animation& animation, int itemType, int xPos, int yPos)
:   animation(animation)
{
	posX = xPos;
	posY = yPos;
	type = itemType;
}

int Item::x() const { return posX; }
int Item::y() const { return posY; }

int Item::getType() const { return type; }

void Item::draw() const
{ 
    Gosu::Image& image = *animation.at(Gosu::milliseconds() / 100 % animation.size()); 
  
    image.draw(posX - image.width() / 2.0, posY - image.height() / 2.0, zItem, 1, 1);
}
