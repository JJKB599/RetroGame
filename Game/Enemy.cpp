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
#include "GameConstants.h" 
#include "ZOrder.h" 
  
#include "Enemy.h"
  
  
Enemy::Enemy(Animation& animation)
:   animation(animation) 
{ 
    posX = posY = 0;
}
  
int Enemy::x() const { return posX; }
int Enemy::y() const { return posY; }
  
void Enemy::warp(int x, int y)
{ 
    posX = x; 
    posY = y; 
} 
  
void Enemy::moveUp()
{ 
    posY -= 1; 
} 
  
void Enemy::moveDown()
{ 
    posY += 1; 
} 
  
void Enemy::moveLeft()
{ 
    posX -= 1; 
} 
  
void Enemy::moveRight()
{ 
    posX += 1; 
} 
  
void Enemy::draw() const
{ 
    Gosu::Image& image = *animation.at(Gosu::milliseconds() / 100 % animation.size()); 
  
    image.draw(posX - image.width() / 2.0, posY - image.height() / 2.0, zEnemy, 1, 1); 
}
