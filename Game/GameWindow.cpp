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
#include "Item.h"
#include "ItemType.h"
#include "Player.h"
#include "Shot.h"
#include "Spotlight.h"
#include "Stair.h"
#include "Utilities.h"
#include "ZOrder.h"

  
class GameWindow : public Gosu::Window 
{
	std::auto_ptr<Gosu::Image> backgroundImage;
	
	Animation playerAnim;
	Animation enemyAnim;
	Animation shotAnim;
	Animation ammoAnim;
	Animation healthAnim;
  
	Player player;
	std::list<Enemy> enemies;
	std::list<Item> items;
	Environment environment;
	Shot shot;
	Spotlight spotlight;
	Stair up;
	Stair down;
  
    public: 
        GameWindow() 
        :   Window(XRES, YRES, FULLSCREEN), 
            player(playerAnim),
            shot(shotAnim),
			spotlight(graphics())
        { 
          setCaption(L"Fire Mazing");

		  std::wstring playerGraphic = Gosu::resourcePrefix() + L"media/fireman/fireman.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), playerGraphic, 30, 30, false, playerAnim);
          
		  std::wstring enemyGraphic = Gosu::resourcePrefix() + L"media/enemy/fire.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), enemyGraphic, 30, 30, false, enemyAnim);

		  std::wstring shotGraphic = Gosu::resourcePrefix() + L"media/shot/shot.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), shotGraphic, 30, 30, false, shotAnim);

		  std::wstring ammoGraphic = Gosu::resourcePrefix() + L"media/items/ammo.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), ammoGraphic, 30, 30, false, ammoAnim);

		  std::wstring healthGraphic = Gosu::resourcePrefix() + L"media/items/health.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), healthGraphic, 30, 30, false, healthAnim);

		  
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

		  // Initializing items.
		  items.push_back(Item(ammoAnim, AMMO, 45, 45));
		  //items.push_back(Item(healthAnim, HEALTH, 45, 45));

		  loadLevel(2);
        } 
  
        void update() 
        {
		  std::list<Enemy>::iterator cur;

		  // Check for collisions
		  player.checkForEnemyCollisions(enemies);
		  player.checkForItemCollisions(items);
		  
		  if (!shot.active() && !player.isOnFire())
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
				  if (canMoveDirection(player.x(), player.y(), environment, LEFT) && player.getCurrentWalkCycleDirection() == LEFT)
					player.moveLeft();
				  else
					player.turnLeft();
			  }
			  else if (input().down(Gosu::kbRight) || input().down(Gosu::gpRight))
			  {
				  if (canMoveDirection(player.x(), player.y(), environment, RIGHT) && player.getCurrentWalkCycleDirection() == RIGHT)
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
			for (std::list<Item>::const_iterator i = items.begin(); i != items.end(); ++i)
			{
				i->draw();
			}
			spotlight.draw(player, 50);
			backgroundImage->draw(0, 0, zBackground);
		}
  
        void buttonDown(Gosu::Button btn) 
        { 
		  if (btn == Gosu::kbEscape)
		    close();

		  if (!shot.active() && !player.isOnFire())
		  {
			  if (btn == Gosu::kbSpace && player.isStandingStill())
			  {
				  player.shoot(shot, enemies);
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
  
  void loadLevel(int level) {
    std::wstring filename;
    int numWalls = 0;
    wall *walls = NULL;
    
    switch (level) {
      case 1: {
        // Get map graphic
        filename = Gosu::resourcePrefix() + L"media/maps/map1.bmp";

        // Get map walls
        numWalls = 4;
        wall someWalls[] = {
            wall(0, 0, 240, 0),
            wall(0, 0, 0, 210),
            wall(0, 210, 240, 210),
            wall(210, 210, 240, 0)
        };
        walls = (wall *)realloc(walls, numWalls * sizeof(wall));
        memcpy(walls, someWalls, (numWalls * sizeof(wall)));
        
        // Set entrances and exits
        down = Stair(0, 45, false);
        up = Stair(90, 195, true);
        
        break;
      }
      case 2: {
          // Get map graphic
          filename = Gosu::resourcePrefix() + L"media/maps/map2.bmp";

          // Get map walls
          numWalls = 4;
          wall someWalls[] = {
              wall(0, 0, 240, 0),
              wall(0, 0, 0, 210),
              wall(0, 210, 240, 210),
              wall(210, 210, 240, 0)
          };
          walls = (wall *)realloc(walls, numWalls * sizeof(wall));
          memcpy(walls, someWalls, (numWalls * sizeof(wall)));
        
        // Set entrances and exits
        down = Stair(60, 195, false);
        up = Stair(0, 105, true);
        
        break;
      }
      default:
        break;
    }
    
    backgroundImage.reset(new Gosu::Image(graphics(), filename, true));
    environment = Environment(numWalls, walls);
  }
};


int main() 
{ 
    GameWindow window; 
    window.show(); 
} 
