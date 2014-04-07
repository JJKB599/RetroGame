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

#include "Enemy.h"
#include "Player.h"
#include "ZOrder.h"

#include "Spotlight.h"


Spotlight::Spotlight(Gosu::Graphics &graphics)
{
	std::wstring filename = Gosu::resourcePrefix() + L"media/maps/map1.bmp";
	spotlightImage.reset(new Gosu::Image(graphics, filename, true));
	spotlightBitmap = new Gosu::Bitmap(spotlightImage->width(), spotlightImage->height());
}

void Spotlight::draw(Player& player, int magnitude)
{
	int x, y;
	double distanceSquared, distSq;

	for (y = 0; y < static_cast<int>(spotlightImage->height()); y++)
	{
		for (x = 0; x < static_cast<int>(spotlightImage->width()); x++)
		{
			// Check distance from player.
			distanceSquared = (x - player.x()) * (x - player.x()) + (y - player.y()) * (y - player.y());

			// Illuminate the pixel based on the distance to the closest object.
			if (distanceSquared < magnitude * magnitude)
			{
				spotlightBitmap->setPixel(x, y, Gosu::Color::NONE);
			}
			else
			{
				spotlightBitmap->setPixel(x, y, Gosu::Color::BLACK);
			}
		}
	}

	spotlightImage->getData().insert(*spotlightBitmap, 0, 0);
	spotlightImage->draw(0, 0, zSpotlight);
}
