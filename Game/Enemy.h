#ifndef ENEMY_H
#define ENEMY_H
  
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

#include "Animation.h" 
#include "Environment.h"

class Enemy
{
	Animation& animation;
	int roundNumber;
    int posX, posY;
    int direction;
  
    public: 
        Enemy(Animation& animation, int round);
  
        int x() const;
        int y() const;
  
        void warp(int x, int y);
  
        void move(Environment env);
        void moveDirection(int dir);
          
        void draw() const; 
}; 
  
#endif 
