// All of Gosu. 
#include <Gosu/Gosu.hpp> 
// To safely include std::tr1::shared_ptr 
#include <Gosu/TR1.hpp>  
// Makes life easier for Windows users compiling this. 
#include <Gosu/AutoLink.hpp> 
  
#include <Gosu/Color.hpp> 
  
#include <cmath> 
#include <cstdlib>
#include <ctime>
#include <list> 
#include <sstream> // For int <-> string conversion 
#include <vector> 
  
#include "Animation.h" 
#include "Environment.h"
#include "Direction.h"
#include "GameConstants.h" 
#include "Utilities.h"
#include "ZOrder.h" 
  
#include "Enemy.h"
  
  
Enemy::Enemy(Animation& animation)
:   animation(animation) 
{ 
    srand(time(NULL));
	
	posX = posY = 0;
    direction = LEFT;
}


int Enemy::x() const { return posX; }
int Enemy::y() const { return posY; }


void Enemy::warp(int x, int y)
{ 
    posX = x; 
    posY = y; 
} 


void Enemy::move(Environment env)
{
	if (isInCentreOfCell(posX - 15, posY - 15))
	{
		int dir = rand() % 4;
		int i;
		
		if (canMoveDirection(posX, posY, env, direction))
		{
			// Randomly select a direction to move OTHER than the opposite of current direction.
			for (i = 0; i < 4; i++)
			{
				if (canMoveDirection(posX, posY, env, (dir + i) % 4))
				{
					if (!(direction == RIGHT && (dir + i) % 4 == LEFT) &&
						!(direction == LEFT && (dir + i) % 4 == RIGHT) &&
						!(direction == UP && (dir + i) % 4 == DOWN) &&
						!(direction == DOWN && (dir + i) % 4 == UP))
					{
						direction = (dir + i) % 4;
						break;
					}
				}
			}
		}
		else
		{
			// Randomly select a direction to move from ALL possible directions.
			for (i = 0; i < 4; i++)
			{
				if (canMoveDirection(posX, posY, env, (dir + i) % 4))
				{
					direction = (dir + i) % 4;
					break;
				}
			}
		}
	}

	moveDirection(direction);
}


void Enemy::moveDirection(int dir)
{
  switch (direction) {
    case LEFT:
      posX -= 1;
      break;
    case RIGHT:
      posX += 1;
      break;
    case UP:
      posY -= 1;
      break;
    case DOWN:
      posY += 1;
      break;
  }
}


void Enemy::draw() const
{ 
    Gosu::Image& image = *animation.at(Gosu::milliseconds() / 100 % animation.size()); 
  
    image.draw(mapXToScreenX(posX) - image.width() / 2.0, mapYToScreenY(posY) - image.height() / 2.0, zPlayer, 1, 1); 
} 
