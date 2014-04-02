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
#include "Utilities.h"

  
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

		  std::wstring filename = Gosu::resourcePrefix() + L"media/maps/map1.bmp";
		  backgroundImage.reset(new Gosu::Image(graphics(), filename, true));

      std::wstring playerGraphic = Gosu::resourcePrefix() + L"media/fireman/fireman.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), playerGraphic, 30, 30, false, playerAnim);
          
		  std::wstring enemyGraphic = Gosu::resourcePrefix() + L"media/enemy/fire.bmp";
      Gosu::imagesFromTiledBitmap(graphics(), enemyGraphic, 30, 30, false, enemyAnim);

		  std::wstring shotGraphic = Gosu::resourcePrefix() + L"media/shot/shot.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), shotGraphic, 30, 30, false, shotAnim);

		  // Create the walls
          int numWalls = 25;
          wall walls[] = {
              // Outer walls
              wall(0, 0, XRES, 0),
              wall(0, 0, 0, YRES),
              wall(0, YRES, XRES, YRES),
              wall(XRES, YRES, XRES, 0),
              // Environment walls
              // This matches the first map when it is fitted to the top left corner of the screen
              // Really should come up with a dynamic way of generating this...
              wall(30, 30, 30, 120),
              wall(30, 30, 90, 30),
              wall(90, 30, 90, 60),
              wall(90, 60, 60, 60),
              wall(30, 90, 120, 90),
              wall(120, 0, 120, 90),
              wall(30, 120, 60, 120),
              wall(60, 120, 60, 180),
              wall(0, 180, 30, 180),
              wall(30, 180, 30, 150),
              wall(90, 180, 210, 180),
              wall(120, 180, 120, 210),
              wall(90, 180, 90, 120),
              wall(90, 120, 150, 120),
              wall(150, 120, 150, 30),
              wall(150, 30, 180, 30),
              wall(210, 30, 210, 180),
              wall(210, 60, 180, 60),
              wall(150, 90, 180, 90),
              wall(210, 120, 180, 120),
              wall(210, 150, 120, 150)
			  /*wall(16, 29, 256, 29),
			  wall(256, 29, 256, 239),
			  wall(16, 29, 16, 239),
			  wall(16, 239, 256, 239),

			  wall(46, 59, 46, 149),
              wall(46, 59, 106, 59),
              wall(106, 59, 106, 89),
              wall(106, 89, 76, 89),
              wall(46, 119, 136, 119),
              wall(136, 29, 136, 119),
              wall(46, 149, 76, 149),
              wall(76, 149, 76, 209),
              wall(16, 209, 46, 209),
              wall(46, 209, 46, 179),
              wall(106, 209, 226, 209),
              wall(136, 209, 136, 239),
              wall(106, 209, 106, 149),
              wall(106, 149, 166, 149),
              wall(166, 149, 166, 59),
              wall(166, 59, 196, 59),
              wall(226, 59, 226, 209),
              wall(226, 89, 196, 89),
              wall(166, 119, 196, 119),
              wall(226, 149, 196, 149),
              wall(226, 179, 136, 179)*/
            };
          environment = Environment(numWalls, walls);

		  
          /* Set initial positions */
		  
          player.warp(135, 105);

		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));
		  enemies.push_back(Enemy(enemyAnim));

		  // Initializing enemy list.
		  std::list<Enemy>::iterator cur = enemies.begin();
		  cur->warp(135,75);
		  ++cur;
		  cur->warp(165,165);
		  ++cur;
		  cur->warp(225,195);
		  ++cur;
		  cur->warp(195,195);
		  ++cur;
		  cur->warp(45,45);
		  ++cur;
        } 
  
        void update() 
        {
		  std::list<Enemy>::iterator cur;

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
				  else
					player.turnLeft();
			  }
			  else if (input().down(Gosu::kbRight) || input().down(Gosu::gpRight))
			  {
				  if (canMoveDirection(player.x(), player.y(), environment, RIGHT))
					player.moveRight();
				  else
					player.turnRight();
			  }
		  }

		  // Move the player
		  player.move();
		  
		  // Move the enemy
		  cur = enemies.begin();
		  while (cur != enemies.end())
		  {
			  cur->move(environment);
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
			  if (btn == Gosu::kbSpace && player.isStandingStill())
			  {
				  int shotDirection = player.shotDirection();
				  if (shotDirection == UP)
					  shot.activate(player.x(), player.y() - 30, UP, player.getCurrentWalkCycleDirection(), enemies);
				  if (shotDirection == DOWN)
					  shot.activate(player.x(), player.y() + 30, DOWN, player.getCurrentWalkCycleDirection(), enemies);
				  if (shotDirection == LEFT)
					  shot.activate(player.x() - 30, player.y(), LEFT, player.getCurrentWalkCycleDirection(), enemies);
				  if (shotDirection == RIGHT)
					  shot.activate(player.x() + 30, player.y(), RIGHT, player.getCurrentWalkCycleDirection(), enemies);
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
};


int main() 
{ 
    GameWindow window; 
    window.show(); 
} 
