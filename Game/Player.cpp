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
#include "Item.h"
#include "ItemType.h"
#include "Utilities.h"
#include "ZOrder.h"

#include "Player.h"


Player::Player(Animation& animation)
:	animation(animation)
{
	posX = posY = 0;
	currentWalkCycle = LEFT_GUN_FORWARD_CYCLE;
	currentDirection = LEFT;
	currentWalkCycleDirection = LEFT;
	gunPosition = GUN_FORWARD;
	standingStill = true;
	lastTurnTime = Gosu::milliseconds() / 100;
	onFire = false;
	dead = false;
	recovering = false;
	ammo = startAmmo = 20;
	health = startHealth = 10;
}


int Player::x() const { return posX; }
int Player::y() const { return posY; }

bool Player::isOnFire() const { return onFire; }
bool Player::isDead() const { return dead; }
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
	if (!standingStill && !onFire)
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
	if (!onFire && standingStill && Gosu::milliseconds() / 100 - lastTurnTime > 1)
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
	if (!onFire && standingStill && Gosu::milliseconds() / 100 - lastTurnTime > 1)
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
	if (!onFire && standingStill)
	{
		//posY -= 1;
		currentDirection = UP;
		standingStill = false;
		changeWalkCycle();
	}
}

void Player::moveDown()
{
	if (!onFire && standingStill)
	{
		//posY += 1;
		currentDirection = DOWN;
		standingStill = false;
		changeWalkCycle();
	}
}

void Player::turnLeft()
{
	if (!onFire && standingStill)
	{
		currentDirection = LEFT;
		currentWalkCycleDirection = LEFT;
		changeWalkCycle();
		lastTurnTime = Gosu::milliseconds() / 100;
	}
}

void Player::turnRight()
{
	if (!onFire && standingStill)
	{
		currentDirection = RIGHT;
		currentWalkCycleDirection = RIGHT;
		changeWalkCycle();
		lastTurnTime = Gosu::milliseconds() / 100;
	}
}


void Player::moveGunUp()
{
	if (!onFire)
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
	if (!onFire)
	{
		if (gunPosition == GUN_UP)
			gunPosition = GUN_FORWARD;
		else if (gunPosition == GUN_FORWARD)
			gunPosition = GUN_DOWN;
		changeWalkCycle();
	}
}


void Player::shoot(Shot& shot, std::list<Enemy>& enemies)
{
	if (ammo > 0)
	{
		//int shotDirection = this.shotDirection();
		if (shotDirection() == UP)
			shot.activate(posX, posY - 30, UP, getCurrentWalkCycleDirection(), enemies);
		else if (shotDirection() == DOWN)
			shot.activate(posX, posY + 30, DOWN, getCurrentWalkCycleDirection(), enemies);
		else if (shotDirection() == LEFT)
			shot.activate(posX - 30, posY, LEFT, getCurrentWalkCycleDirection(), enemies);
		else // if (shotDirection() == RIGHT)
			shot.activate(posX + 30, posY, RIGHT, getCurrentWalkCycleDirection(), enemies);

		ammo--;
	}
}


void Player::checkForEnemyCollisions(std::list<Enemy>& enemies)
{
	std::list<Enemy>::iterator cur = enemies.begin();
    while (cur != enemies.end())
    {
        if (Gosu::distance(posX, posY, cur->x(), cur->y()) < 30)
		{
            if (!onFire && !recovering)
			{
				onFire = true;
				health--;
				if (health <= 0)
					dead = true;
				onFireFrameCount = 0;
			}
			break;
		}
        else
            ++cur;
    }
}

void Player::checkForItemCollisions(std::list<Item>& items)
{
	std::list<Item>::iterator cur = items.begin();
    while (cur != items.end())
    {
		if (Gosu::distance(posX, posY, cur->x(), cur->y()) < 30)
		{
			if (cur->getType() == AMMO)
				ammo = startAmmo;
			else if (cur->getType() == HEALTH)
				health = startHealth;

			cur = items.erase(cur);

			break;
		}
		else
			++cur;
	}
}


void Player::draw()
{
	if (recovering)
	{
		if (Gosu::milliseconds() / 100 - recoveringStartTime > 40)
			recovering = false;
		if (Gosu::milliseconds() / 100 % 2 == 0) // don't draw every fourth frame if recovering; to give flashing effect
			return;
	}

	if (onFire)
	{
		int startFrame;
		if (currentWalkCycleDirection == LEFT)
			startFrame = ON_FIRE_LEFT_CYCLE * NUMBER_OF_FRAMES;
		else
			startFrame = ON_FIRE_RIGHT_CYCLE * NUMBER_OF_FRAMES;

		Gosu::Image& image = *animation.at(onFireFrameCount + startFrame);
		image.draw(posX - image.width() / 2.0, posY - image.height() / 2.0, zPlayer, 1, 1);

		if (onFireFrameCount < 8)
			onFireFrameCount++;
		if (onFireFrameCount == 7 && !dead)
		{
			onFire = false;
			recovering = true;
			recoveringStartTime = Gosu::milliseconds() / 100;
		}
	}
	else if (standingStill)
	{
		Gosu::Image& image = *animation.at(currentWalkCycle * NUMBER_OF_FRAMES);
		image.draw(posX - image.width() / 2.0, posY - image.height() / 2.0, zPlayer, 1, 1);
	}
	else
	{
		Gosu::Image& image = *animation.at(Gosu::milliseconds() / 100 % NUMBER_OF_FRAMES + currentWalkCycle * NUMBER_OF_FRAMES);
		image.draw(posX - image.width() / 2.0, posY - image.height() / 2.0, zPlayer, 1, 1);
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
