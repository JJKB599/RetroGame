#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
  
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

struct wall {
  int x1, y1, x2, y2;
  wall(int x1, int y1, int x2, int y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
  }
};

class Environment
{
  int numWalls;
  wall *walls;

  public:
    Environment();
    Environment(int numWalls, wall* walls);
    int getNumWalls();
    wall *getWalls();

};

#endif 
