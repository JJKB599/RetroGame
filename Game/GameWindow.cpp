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
#include "Direction.h"
#include "Enemy.h"
#include "Environment.h"
#include "GameConstants.h"
#include "Player.h"
#include "Shot.h"

  
class GameWindow : public Gosu::Window 
{
	std::auto_ptr<Gosu::Image> backgroundImage;
	
	Animation playerAnim; 
    Animation enemyAnim;
	Animation shotAnim;
  
    Player player;
	std::list<Enemy> enemies;
	Environment environment;
	Shot shot;
  
    public: 
        GameWindow() 
        :   Window(XRES, YRES, FULLSCREEN), 
            player(playerAnim),
			shot(shotAnim)
        { 
          setCaption(L"Fire Mazing");

		  std::wstring filename = Gosu::resourcePrefix() + L"media/maps/map1.png";
		  backgroundImage.reset(new Gosu::Image(graphics(), filename, true));

          std::wstring playerGraphic = Gosu::resourcePrefix() + L"media/fireman/fireman.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), playerGraphic, 30, 30, false, playerAnim);
          
		  std::wstring enemyGraphic = Gosu::resourcePrefix() + L"media/enemy/fire.bmp";
          Gosu::imagesFromTiledBitmap(graphics(), enemyGraphic, 30, 30, false, enemyAnim);

		  std::wstring shotGraphic = Gosu::resourcePrefix() + L"media/shot/shot.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), shotGraphic, 30, 30, false, shotAnim);

		  // Create the walls
          int numWalls = 4;
          wall walls[] = {
              // Outer walls
              wall(0, 0, XRES, 0),
              wall(0, 0, 0, YRES),
              wall(0, YRES, XRES, YRES),
              wall(XRES, YRES, XRES, 0),
              // Environment walls
              // This matches the first map when it is fitted to the top left corner of the screen
              // Really should come up with a dynamic way of generating this...
//              wall(30, 30, 30, 120),
//              wall(30, 30, 90, 30),
//              wall(90, 30, 90, 60),
//              wall(90, 60, 60, 60),
//              wall(30, 90, 120, 90),
//              wall(120, 0, 120, 90),
//              wall(30, 120, 60, 120),
//              wall(60, 120, 60, 180),
//              wall(0, 180, 30, 180),
//              wall(30, 180, 30, 150),
//              wall(90, 180, 210, 180),
//              wall(120, 180, 120, 210),
//              wall(90, 180, 90, 120),
//              wall(90, 120, 150, 120),
//              wall(150, 120, 150, 30),
//              wall(150, 30, 180, 30),
//              wall(210, 30, 210, 180),
//              wall(210, 60, 180, 60),
//              wall(150, 90, 180, 90),
//              wall(210, 120, 180, 120),
//              wall(210, 150, 120, 150)
            };
          environment = Environment(numWalls, walls);

		  
          /* Set initial positions */
		  
          player.warp(XRES / 2, YRES / 2);

		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));

		  // Initializing enemy list.
		  std::list<Enemy>::iterator cur = enemies.begin();
		  cur->warp(120,60);
		  ++cur;
		  cur->warp(150,150);
		  ++cur;
		  cur->warp(210,180);
		  ++cur;
		  cur->warp(180,180);
		  ++cur;
		  cur->warp(30,30);
		  ++cur;
        } 
  
        void update() 
        {
		  std::list<Enemy>::iterator cur;
		  int dir;


		  // Check for a collision
		  player.checkForEnemyCollisions(enemies);
		  
		  if (!shot.active() && !player.isDying())
		  {
			  // Move the player
			  if (input().down(Gosu::kbUp) || input().down(Gosu::gpUp))
			  {
				  if (canMoveDirection(player.x(), player.y(), environment, UP))
					player.moveUp();
			  }
			  else if (input().down(Gosu::kbDown) || input().down(Gosu::gpDown))
			  {
				  if (canMoveDirection(player.x(), player.y(), environment, DOWN))
					player.moveDown();
			  }
			  else if (input().down(Gosu::kbLeft) || input().down(Gosu::gpLeft))
			  {
				  if (canMoveDirection(player.x(), player.y(), environment, LEFT))
					player.moveLeft();
			  }
			  else if (input().down(Gosu::kbRight) || input().down(Gosu::gpRight))
			  {
				  if (canMoveDirection(player.x(), player.y(), environment, RIGHT))
					player.moveRight();
			  }
		  }
		  
		  // Move the enemy
		  cur = enemies.begin();
		  while (cur != enemies.end())
		  {
			  dir = rand() % 4;
			  switch (dir) {
				case LEFT:
				  if (canMoveDirection(cur->x(), cur->y(), environment, LEFT))
					cur->moveLeft();
				  break;
				case RIGHT:
				  if (canMoveDirection(cur->x(), cur->y(), environment, RIGHT))
					cur->moveRight();
				  break;
				case UP:
				  if (canMoveDirection(cur->x(), cur->y(), environment, UP))
					cur->moveUp();
				  break;
				case DOWN:
				  if (canMoveDirection(cur->x(), cur->y(), environment, DOWN))
					cur->moveDown();
				  break;
			  }
			  
			  ++cur;
		  }
        } 
  
        void draw() 
        { 
			player.draw();
			shot.draw();
			for (std::list<Enemy>::const_iterator i = enemies.begin(); i != enemies.end(); ++i)
			{
				i->draw();
			}
			backgroundImage->draw(0,0,0);
        } 
  
        void buttonDown(Gosu::Button btn) 
        { 
		  if (btn == Gosu::kbEscape)
		    close();

		  if (!shot.active() && !player.isDying())
		  {
			  if (btn == Gosu::kbSpace)
			  {
				  int shotDirection = player.shotDirection();
				  if (shotDirection == UP)
					  shot.activate(player.x(), player.y() - 30, UP, enemies);
				  if (shotDirection == DOWN)
					  shot.activate(player.x(), player.y() + 30, DOWN, enemies);
				  if (shotDirection == LEFT)
					  shot.activate(player.x() - 30, player.y(), LEFT, enemies);
				  if (shotDirection == RIGHT)
					  shot.activate(player.x() + 30, player.y(), RIGHT, enemies);
			  }
			  else if (btn == Gosu::kbW)
			  {
				  player.moveGunUp();
			  }
			  else if (btn == Gosu::kbS)
			  {
				  player.moveGunDown();
			  }
		  }
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
            }
          }
          return true;
        }
};


int main() 
{ 
    GameWindow window; 
    window.show(); 
} 
