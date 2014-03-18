#include "Environment.h"

// Empty constructor
Environment::Environment()
{
  numWalls = 0;
  walls = NULL;
}


// Use this constructor
Environment::Environment(int nWalls, wall* someWalls)
{
  numWalls = nWalls;
  walls = (wall *)malloc(numWalls * sizeof(wall));
  memcpy(walls, someWalls, (numWalls * sizeof(wall)));
}


wall* Environment::getWalls()
{
  return walls;
}


int Environment::getNumWalls()
{
  return numWalls;
}