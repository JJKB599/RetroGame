#include "Direction.h"
#include "GameConstants.h"
#include "Utilities.h"


bool collisionDidOccur(Player player, Enemy enemy)
{
  if (abs(player.x() - enemy.x()) <= 30 && abs(player.y() - enemy.y()) <= 30)
    return true;
  return false;
}


bool canMoveDirection(int x, int y, Environment environment, int direction)
{
  wall *walls = environment.getWalls();
  for (int i = 0; i < environment.getNumWalls(); ++i) {
    wall wall = walls[i];

    // Either the wall is a vertical wall
    if (wall.x1 == wall.x2) {
      // If the player is between the 2 ends of the wall
      if ((y < wall.y1 && y > wall.y2) || (y > wall.y1 && y < wall.y2)) {
        // Then determine if he is standing right next to the wall
        if (abs(x - wall.x1) <= 15) {
          // Then either he is to the right of the wall
          if (x > wall.x1 && direction == LEFT)
            return false;
          // Or he is to the left of the wall
          else if (x < wall.x1 && direction == RIGHT)
            return false;
        }
      }
      // If the player is parallel(ish) with the wall
      if (abs(x - wall.x1) < 15) {
        // Then determine if the player is standing above or below the wall
        if (abs(y - wall.y1) < 15 || abs(y - wall.y2) < 15) {
          if (wall.y1 > wall.y2) {
            if (y > wall.y1 && direction == UP)
              return false;
            else if (y < wall.y2 && direction == DOWN)
              return false;
          }
          else {
            if (y > wall.y2 && direction == UP)
              return false;
            else if (y < wall.y1 && direction == DOWN)
              return false;
          }
        }
      }
    }
    // Or its a horizontal wall
    else if (wall.y1 == wall.y2) {
      // If the player is between the 2 ends of the wall
      if ((x < wall.x1 && x > wall.x2) || (x > wall.x1 && x < wall.x2)) {
        // Then determine if he is standing right next to the wall
        if (abs(y - wall.y1) <= 15) {
          // Then either he is below the wall
          if (y > wall.y1 && direction == UP)
            return false;
          // Or he is above the wall
          else if (y < wall.y1 && direction == DOWN)
            return false;
        }
      }
      // If the player is parallel(ish) with the wall
      if (abs(y - wall.y1) < 15) {
        // Then determine if the player is standing above or below the wall
        if (abs(x - wall.x1) < 15 || abs(x - wall.x2) < 15) {
          if (wall.x1 > wall.x2) {
            if (x > wall.x1 && direction == UP)
              return false;
            else if (x < wall.x2 && direction == DOWN)
              return false;
          }
          else {
            if (x > wall.x2 && direction == UP)
              return false;
            else if (x < wall.x1 && direction == DOWN)
              return false;
          }
        }
      }
    }
  }
  return true;
}


int screenXToMapX(int screenX)
{
	return screenX - 16;
}

int screenYToMapY(int screenY)
{
	return screenY - 29;
}

int mapXToScreenX(int mapX)
{
	return mapX + 16;
}

int mapYToScreenY(int mapY)
{
	return mapY + 29;
}

bool isInCentreOfCell(int x, int y)
{
	return (x % TILE_SIZE == 0 && y % TILE_SIZE == 0);
}
