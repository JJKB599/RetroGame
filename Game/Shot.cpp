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

#include "Shot.h"


Shot::Shot(Animation& animation)
:   animation(animation) 
{ 
  posX = posY = 0;
  isActive = false;
}


int Shot::x() const { return posX; }
int Shot::y() const { return posY; }
bool Shot::active() const { return isActive; }


void Shot::activate(int x, int y, int shotDirection, int playerWalkCycleDirection, std::list<Enemy>& enemies)
{
	posX = x;
	posY = y;
	startTime = Gosu::milliseconds();
	isActive = true;
	
	if (shotDirection == LEFT)
		shotCycle = LEFT_SHOT_CYCLE;
	else if (shotDirection == RIGHT)
		shotCycle = RIGHT_SHOT_CYCLE;
	else if (shotDirection == UP)
	{
		if (playerWalkCycleDirection == RIGHT)
			shotCycle = RIGHT_UP_SHOT_CYCLE;
		else // if (playerWalkCycleDirection == LEFT)
			shotCycle = LEFT_UP_SHOT_CYCLE;
	}
	else // if (shotDirection == DOWN)
	{
		if (playerWalkCycleDirection == RIGHT)
			shotCycle = RIGHT_DOWN_SHOT_CYCLE;
		else // if (playerWalkCycleDirection == LEFT)
			shotCycle = LEFT_DOWN_SHOT_CYCLE;
	}

	std::list<Enemy>::iterator cur = enemies.begin();
    while (cur != enemies.end()) {
      if (Gosu::distance((double)posX, (double)posY, (double)cur->x(), (double)cur->y()) < 30)
          cur = enemies.erase(cur);
      else
          ++cur;
    }
}

void Shot::draw()
{ 
	if (isActive) {
		Gosu::Image& image = *animation.at((Gosu::milliseconds() - startTime) / 100 % NUMBER_OF_FRAMES + shotCycle * NUMBER_OF_FRAMES);

		image.draw(posX - image.width() / 2.0, posY - image.height() / 2.0, zShot, 1, 1);

		std::wstring path = Gosu::resourcePrefix() + L"media/sounds/extinguish.wav";
		/*Gosu::Sample sample = Gosu::Sample(path);
		sample.play();*/
		Gosu::Sample(path).play();

		if ((Gosu::milliseconds() - startTime) / 100 == NUMBER_OF_FRAMES - 1)
			isActive = false;
	}
}
