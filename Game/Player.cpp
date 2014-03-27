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
#include "Direction.h"
#include "Enemy.h"
#include "Utilities.h"
#include "ZOrder.h"

#include "Player.h"


Player::Player(Animation& animation)
:	animation(animation)
{
	posX = posY = 0;
	currentWalkCycle = LEFT_GUN_DOWN_CYCLE;
	currentDirection = LEFT;
	currentWalkCycleDirection = LEFT;
	gunPosition = GUN_DOWN;
	standingStill = true;
	dying = false;
}


int Player::x() const { return posX; }
int Player::y() const { return posY; }

bool Player::isDying() const { return dying; }
bool Player::isStandingStill() const { return standingStill; }


int Player::shotDirection() const
{
	if (currentWalkCycle == LEFT_GUN_UP_CYCLE || currentWalkCycle == RIGHT_GUN_UP_CYCLE)
	{
		return UP;
	}
	if (currentWalkCycle == LEFT_GUN_DOWN_CYCLE || currentWalkCycle == RIGHT_GUN_DOWN_CYCLE)
	{
		return DOWN;
	}
	else if (currentWalkCycle == LEFT_GUN_FORWARD_CYCLE)
	{
		return LEFT;
	}
	else // if (currentWalkCycle == RIGHT_GUN_FORWARD_CYCLE)
	{
		return RIGHT;
	}
}


int Player::getCurrentWalkCycleDirection() const
{
	return currentWalkCycleDirection;
}


void Player::warp(int x, int y)
{
	posX = x;
	posY = y;
}

void Player::move()
{
	if (!standingStill && !dying)
	{
		if (currentDirection == LEFT)
			posX -= 1;
		else if (currentDirection == RIGHT)
			posX += 1;
		else if (currentDirection == UP)
			posY -= 1;
		else // if (currentDirection == DOWN)
			posY += 1;

		if (isInCentreOfCell(posX - 15, posY - 15))
			standingStill = true;
	}
}

void Player::moveLeft()
{
	if (!dying && standingStill)
	{
		//posX -= 1;
		currentDirection = LEFT;
		currentWalkCycleDirection = LEFT;
		standingStill = false;
		changeWalkCycle();
	}
}

void Player::moveRight()
{
	if (!dying && standingStill)
	{
		//posX += 1;
		currentDirection = RIGHT;
		currentWalkCycleDirection = RIGHT;
		standingStill = false;
		changeWalkCycle();
	}
}
void Player::moveUp()
{
	if (!dying && standingStill)
	{
		//posY -= 1;
		currentDirection = UP;
		standingStill = false;
		changeWalkCycle();
	}
}

void Player::moveDown()
{
	if (!dying && standingStill)
	{
		//posY += 1;
		currentDirection = DOWN;
		standingStill = false;
		changeWalkCycle();
	}
}

void Player::turnLeft()
{
	if (!dying && standingStill)
	{
		currentDirection = LEFT;
		currentWalkCycleDirection = LEFT;
		changeWalkCycle();
	}
}

void Player::turnRight()
{
	if (!dying && standingStill)
	{
		currentDirection = RIGHT;
		currentWalkCycleDirection = RIGHT;
		changeWalkCycle();
	}
}


void Player::moveGunUp()
{
	if (!dying)
	{
		if (gunPosition == GUN_DOWN)
			gunPosition = GUN_FORWARD;
		else if (gunPosition == GUN_FORWARD)
			gunPosition = GUN_UP;
		changeWalkCycle();
	}
}

void Player::moveGunDown()
{
	if (!dying)
	{
		if (gunPosition == GUN_UP)
			gunPosition = GUN_FORWARD;
		else if (gunPosition == GUN_FORWARD)
			gunPosition = GUN_DOWN;
		changeWalkCycle();
	}
}


void Player::checkForEnemyCollisions(std::list<Enemy>& enemies)
{
	std::list<Enemy>::iterator cur = enemies.begin();
    while (cur != enemies.end())
    {
        if (Gosu::distance(posX, posY, cur->x(), cur->y()) < 30)
		{
            dying = true;
			break;
		}
        else
            ++cur;
    }
}


void Player::draw()
{
	if (dying)
	{
		Gosu::Image& image = *animation.at(Gosu::milliseconds() / 100 % 2 /* number of frames in dying animation cycle */ + DYING_CYCLE * NUMBER_OF_FRAMES);
		image.draw(mapXToScreenX(posX) - image.width() / 2.0, mapYToScreenY(posY) - image.height() / 2.0, zPlayer, 1, 1);
	}
	else if (standingStill)
	{
		Gosu::Image& image = *animation.at(currentWalkCycle * NUMBER_OF_FRAMES);
		image.draw(mapXToScreenX(posX) - image.width() / 2.0, mapYToScreenY(posY) - image.height() / 2.0, zPlayer, 1, 1);
	}
	else
	{
		Gosu::Image& image = *animation.at(Gosu::milliseconds() / 100 % NUMBER_OF_FRAMES + currentWalkCycle * NUMBER_OF_FRAMES);
		image.draw(mapXToScreenX(posX) - image.width() / 2.0, mapYToScreenY(posY) - image.height() / 2.0, zPlayer, 1, 1);
	}

	//standingStill = true;
}


void Player::changeWalkCycle()
{
	if (currentDirection == LEFT)
	{
		if (gunPosition == GUN_DOWN)
			currentWalkCycle = LEFT_GUN_DOWN_CYCLE;
		else if (gunPosition == GUN_FORWARD)
			currentWalkCycle = LEFT_GUN_FORWARD_CYCLE;
		else if (gunPosition == GUN_UP)
			currentWalkCycle = LEFT_GUN_UP_CYCLE;
	}
	else if (currentDirection == RIGHT)
	{
		if (gunPosition == GUN_DOWN)
			currentWalkCycle = RIGHT_GUN_DOWN_CYCLE;
		else if (gunPosition == GUN_FORWARD)
			currentWalkCycle = RIGHT_GUN_FORWARD_CYCLE;
		else if (gunPosition == GUN_UP)
			currentWalkCycle = RIGHT_GUN_UP_CYCLE;
	}
	else if (currentDirection == UP || currentDirection == DOWN)
	{
		if (gunPosition == GUN_DOWN)
		{
			if (currentWalkCycleDirection == LEFT)
				currentWalkCycle = LEFT_GUN_DOWN_CYCLE;
			else
				currentWalkCycle = RIGHT_GUN_DOWN_CYCLE;
		}
		else if (gunPosition == GUN_FORWARD)
		{
			if (currentWalkCycleDirection == LEFT)
				currentWalkCycle = LEFT_GUN_FORWARD_CYCLE;
			else
				currentWalkCycle = RIGHT_GUN_FORWARD_CYCLE;
		}
		else if (gunPosition == GUN_UP)
		{
			if (currentWalkCycleDirection == LEFT)
				currentWalkCycle = LEFT_GUN_UP_CYCLE;
			else
				currentWalkCycle = RIGHT_GUN_UP_CYCLE;
		}
	}
}
