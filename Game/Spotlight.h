#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

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
#include "Player.h"

class Spotlight
{
	std::auto_ptr<Gosu::Image> spotlightImage;
	Gosu::Bitmap *spotlightBitmap;

	public:
		Spotlight(Gosu::Graphics &graphics);
		void draw(Player& player, int magnitude);
};

#endif
