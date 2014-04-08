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
#include "Enemy.h"
#include "Utilities.h"
#include "ZOrder.h"

#include "Stair.h"


Stair::Stair(int xPos, int yPos, bool d)
{
  posX = xPos;
  posY = yPos;
  dir = d;
}

Stair::Stair()
{
  posX = 0;
  posY = 0;
  dir = false;
}

int Stair::x() const { return posX; }
int Stair::y() const { return posY; }
bool Stair::direction() const { return dir; }
