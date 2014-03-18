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
#include "Environment.h"
#include "Direction.h"
#include "GameConstants.h" 
#include "Utilities.h"
#include "ZOrder.h" 
  
#include "Enemy.h"
  
  
Enemy::Enemy(Animation& animation)
:   animation(animation) 
{ 
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
  // If we can continue moving in the direction, don't change directions
  if (canMoveDirection(x(), y(), env, direction) == false) {
    
    // Otherwise, we need to select a new direction
    do {
      direction = (direction + 1) % 4;
    }
    while (canMoveDirection(x(), y(), env, direction) == false);
  }

  // Then move in that direction
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
  
    image.draw(posX - image.width() / 2.0, posY - image.height() / 2.0, zPlayer, 1, 1); 
} 
