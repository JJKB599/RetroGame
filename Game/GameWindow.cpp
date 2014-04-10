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

#include <ctime>

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
	Animation kittensAnim;
	Animation ammoBarAnim;
	Animation healthBarAnim;
	Animation numbersAnim;

	std::auto_ptr<Gosu::Image> scoreLabelImage;
	std::auto_ptr<Gosu::Image> timeLabelImage;
  
	Player player;
	std::list<Enemy> enemies;
	std::list<Item> items;
	Environment environment;
	Shot shot;
	Spotlight spotlight;
	Stair up;
	Stair down;
	int level;
	int round;
    bool transitioning;
	time_t startTime;

	bool paused;
	bool onStartScreen;

    public: 
        GameWindow() 
        :   Window(XRES, YRES, FULLSCREEN), 
            player(playerAnim),
            shot(shotAnim),
			spotlight(graphics())
        { 
          setCaption(L"Fire Mazing");

		  paused = false;
		  onStartScreen = false;
		  transitioning = false;

		  level = 0;
		  round = 1;

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

		  std::wstring kittensGraphic = Gosu::resourcePrefix() + L"media/items/kittens.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), kittensGraphic, 30, 30, false, kittensAnim);

		  std::wstring ammoBarGraphic = Gosu::resourcePrefix() + L"media/ui/ammoBar.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), ammoBarGraphic, 16, 94, false, ammoBarAnim);

		  std::wstring healthBarGraphic = Gosu::resourcePrefix() + L"media/ui/healthBar.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), healthBarGraphic, 16, 87, false, healthBarAnim);

		  std::wstring numbersGraphic = Gosu::resourcePrefix() + L"media/ui/numbers.bmp";
		  Gosu::imagesFromTiledBitmap(graphics(), numbersGraphic, 12, 10, false, numbersAnim);

		  scoreLabelImage.reset(new Gosu::Image(graphics(), Gosu::resourcePrefix() + L"media/ui/score.bmp", true));
		  timeLabelImage.reset(new Gosu::Image(graphics(), Gosu::resourcePrefix() + L"media/ui/time.bmp", true));

		  loadLevel(level, true);
        } 
  
        void update() {
        
          if (transitioning) {
            if (difftime(time(NULL), startTime) >= 5) {
              transitioning = false;
              player.setAscending(true);
			  player.setStandingStill();
              round += 1;
              level = 1;
              loadLevel(1, true);
            }
          }
          else if (!onStartScreen) {
            player.songUpdate();
            std::list<Enemy>::iterator cur;

            // Check if the player is on a set of stairs
            if (playerOnStairs(player, up) && player.isAscending()) {
              if (level < 3)
                level += 1;
              loadLevel(level, true);
            }

            if (playerOnStairs(player, down) && (player.isAscending() == false)) {
              if (level > 1)
                level -= 1;
              else {
                time(&startTime);
                transitioning = true;
                level = -1;
              }
              
              loadLevel(level, false);
            }


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
        }
  
        void draw() {
          if (transitioning || onStartScreen) {
            backgroundImage->draw(0, 0, zBackground);
            Gosu::Image& ammoBarImage = *ammoBarAnim.at(20 - player.getAmmo());
            ammoBarImage.draw(240, 92, zUI);

            Gosu::Image& healthBarImage = *healthBarAnim.at(10 - player.getHealth());
            healthBarImage.draw(240, 3, zUI);

            scoreLabelImage->draw(5, 215, zUI);
            Gosu::Image& scoreDigit3Image = *numbersAnim.at(player.getScore() / 1000 % 10);
            scoreDigit3Image.draw(68, 215, zUI);
            Gosu::Image& scoreDigit2Image = *numbersAnim.at(player.getScore() / 100 % 10);
            scoreDigit2Image.draw(80, 215, zUI);
            Gosu::Image& scoreDigit1Image = *numbersAnim.at(player.getScore() / 10 % 10);
            scoreDigit1Image.draw(92, 215, zUI);
            Gosu::Image& scoreDigit0Image = *numbersAnim.at(player.getScore() % 10);
            scoreDigit0Image.draw(104, 215, zUI);

			for (std::list<Item>::const_iterator i = items.begin(); i != items.end(); ++i)
            {
              i->draw();
            }
          }
          else {
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

			int spotlightRadius;
			if (round == 1)
				spotlightRadius = 50;
			else if (round == 2)
				spotlightRadius = 42;
			else if (round == 3)
				spotlightRadius = 33;
			else
				spotlightRadius = 25;
            spotlight.draw(player, spotlightRadius);
            backgroundImage->draw(0, 0, zBackground);

            Gosu::Image& ammoBarImage = *ammoBarAnim.at(20 - player.getAmmo());
            ammoBarImage.draw(240, 92, zUI);

            Gosu::Image& healthBarImage = *healthBarAnim.at(10 - player.getHealth());
            healthBarImage.draw(240, 3, zUI);

            scoreLabelImage->draw(5, 215, zUI);
            Gosu::Image& scoreDigit3Image = *numbersAnim.at(player.getScore() / 1000 % 10);
            scoreDigit3Image.draw(68, 215, zUI);
            Gosu::Image& scoreDigit2Image = *numbersAnim.at(player.getScore() / 100 % 10);
            scoreDigit2Image.draw(80, 215, zUI);
            Gosu::Image& scoreDigit1Image = *numbersAnim.at(player.getScore() / 10 % 10);
            scoreDigit1Image.draw(92, 215, zUI);
            Gosu::Image& scoreDigit0Image = *numbersAnim.at(player.getScore() % 10);
            scoreDigit0Image.draw(104, 215, zUI);
          }
        }
  
        void buttonDown(Gosu::Button btn) 
        { 
		  if (onStartScreen)
		  {
			  onStartScreen = false;
			  level = 1;
			  player.resetPlayer();
			  loadLevel(level, true);
		  }
		  else if (player.isDead())
		  {
			  onStartScreen = true;
			  level = 0;
			  loadLevel(level, true);
		  }
		  else
		  {			
			  if (btn == Gosu::kbEscape)
				close();
			  else if (btn == Gosu::kbReturn || btn == Gosu::kbEnter)
			  {
				  if (paused)
					paused = false;
				  else
					paused = true;
			  }

			  if (!paused && !shot.active() && !player.isOnFire())
			  {
				  if (btn == Gosu::kbSpace && player.isStandingStill())
				  {
					  player.shoot(shot, enemies, environment);
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
        }
  
  
  void despawnEnemies() {
	std::list<Enemy>::iterator cur = enemies.begin();
    while (cur != enemies.end())
    {
			cur = enemies.erase(cur);
		}
  }


  void despawnItems() {
	std::list<Item>::iterator cur = items.begin();
    while (cur != items.end())
    {
			cur = items.erase(cur);
		}
  }

  
  void spawnEnemies() {
  
    // Remove any remaining enemies
    despawnEnemies();
  
    // Generate new enemies
    enemies.push_back(Enemy(enemyAnim, round));
    enemies.push_back(Enemy(enemyAnim, round));
    enemies.push_back(Enemy(enemyAnim, round));
    enemies.push_back(Enemy(enemyAnim, round));
    enemies.push_back(Enemy(enemyAnim, round));

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

  
  void loadLevel(int level, bool ascending) {
    std::wstring filename;
    int numWalls = 0;
    wall *walls = NULL;
    
    switch (level) {
      // transition
      case -1: {
        filename = Gosu::resourcePrefix() + L"media/ui/transitionScreen.bmp";
        
        despawnItems();
        despawnEnemies();

		items.push_back(Item(kittensAnim, KITTENS, 135, 105));
        
        break;
      }

      // start screen
	  case 0: {
		  onStartScreen = true;
		  filename = Gosu::resourcePrefix() + L"media/ui/startScreen.bmp";

		  despawnItems();
          despawnEnemies();

		  // Set entrances and exits
          down = Stair(15, 45, false);
          up = Stair(105, 195, true);
        
          // Set player location
          if (ascending) {
            player.warp(down.x(), down.y());
          }
          else {
            player.warp(up.x(), up.y());
          }

		  break;
	  }
    
      case 1: {
        // Get map graphic
        filename = Gosu::resourcePrefix() + L"media/maps/map1.bmp";

        // Get map walls
        numWalls = 25;
        wall someWalls[] = {
           wall(0, 0, 240, 0),
           wall(0, 0, 0, 210),
           wall(0, 210, 240, 210),
           wall(240, 210, 240, 0),
           wall(0, 180, 30, 180),
           wall(30, 30, 30, 120),
           wall(30, 150, 30, 180),
           wall(30, 30, 90, 30),
           wall(90, 30, 90, 60),
           wall(90, 60, 60, 60),
           wall(30, 120, 60, 120),
           wall(60, 120, 60, 150),
           wall(30, 90, 120, 90),
           wall(120, 90, 120, 0),
           wall(90, 210, 90, 180),
           wall(60, 180, 210, 180),
           wall(210, 180, 210, 30),
           wall(210, 60, 180, 60),
           wall(210, 120, 180, 120),
           wall(210, 150, 90, 150),
           wall(90, 150, 90, 120),
           wall(90, 120, 150, 120),
           wall(150, 120, 150, 30),
           wall(150, 30, 180, 30),
           wall(150, 90, 180, 90)
        };
        walls = (wall *)realloc(walls, numWalls * sizeof(wall));
        memcpy(walls, someWalls, (numWalls * sizeof(wall)));
        
        // Set entrances and exits
        down = Stair(15, 45, false);
        up = Stair(105, 195, true);
        
        // Set player location
        if (ascending) {
          player.warp(down.x(), down.y());
        }
        else {
          player.warp(up.x(), up.y());
        }
        
        // Set items
        despawnItems();
        items.push_back(Item(ammoAnim, AMMO, 195, 75));
        items.push_back(Item(healthAnim, HEALTH, 195, 165));

        // Spawn enemies
        spawnEnemies();

        break;
      }
      case 2: {
          // Get map graphic
          filename = Gosu::resourcePrefix() + L"media/maps/map2.bmp";

          // Get map walls
          numWalls = 29;
          wall someWalls[] = {
              wall(0, 0, 240, 0),
              wall(0, 0, 0, 210),
              wall(0, 210, 240, 210),
              wall(240, 210, 240, 0),
              wall(0, 60, 30, 60),
              wall(30, 60, 30, 30),
              wall(30, 30, 90, 30),
              wall(90, 30, 90, 90),
              wall(0, 120, 90, 120),
              wall(30, 120, 30, 90),
              wall(30, 90, 60, 90),
              wall(60, 90, 60, 60),
              wall(30, 210, 30, 150),
              wall(30, 150, 120, 150),
              wall(120, 150, 120, 90),
              wall(120, 90, 150, 90),
              wall(150, 60, 150, 180),
              wall(120, 60, 180, 60),
              wall(120, 30, 120, 60),
              wall(180, 30, 180, 60),
              wall(180, 30, 210, 30),
              wall(210, 60, 240, 60),
              wall(210, 150, 240, 150),
              wall(180, 90, 180, 210),
              wall(180, 90, 210, 90),
              wall(210, 90, 210, 120),
              wall(60, 180, 120, 180),
              wall(90, 180, 90, 210),
              wall(150, 0, 150, 30)
          };
          walls = (wall *)realloc(walls, numWalls * sizeof(wall));
          memcpy(walls, someWalls, (numWalls * sizeof(wall)));
        
        // Set entrances and exits
        down = Stair(75, 195, false);
        up = Stair(15, 105, true);
        
        // Set player location
        if (ascending) {
          player.warp(down.x(), down.y());
        }
        else {
          player.warp(up.x(), up.y());
        }
        
        // Set items
        despawnItems();
        items.push_back(Item(ammoAnim, AMMO, 225, 195));
        items.push_back(Item(healthAnim, HEALTH, 15, 195));
        
        // Spawn enemies
        spawnEnemies();

        break;
      }
      case 3: {
          // Get map graphic
          filename = Gosu::resourcePrefix() + L"media/maps/map3.bmp";

          // Get map walls
          numWalls = 25;
          wall someWalls[] = {
             wall(0, 0, 240, 0),
             wall(0, 0, 0, 210),
             wall(0, 210, 240, 210),
             wall(240, 210, 240, 0),
             wall(0, 60, 30, 60),
             wall(30, 60, 30, 90),
             wall(30, 90, 60, 90),
             wall(60, 60, 60, 180),
             wall(30, 30, 90, 30),
             wall(90, 30, 90, 150),
             wall(30, 180, 180, 180),
             wall(180, 180, 180, 90),
             wall(210, 90, 210, 180),
             wall(210, 180, 240, 180),
             wall(210, 60, 210, 30),
             wall(210, 30, 120, 30),
             wall(120, 30, 120, 0),
             wall(90, 60, 180, 60),
             wall(150, 60, 150, 90),
             wall(120, 90, 180, 90),
             wall(90, 120, 150, 120),
             wall(150, 120, 150, 150),
             wall(150, 150, 120, 150),
             wall(0, 120, 30, 120),
             wall(30, 120, 30, 150)
          };
          walls = (wall *)realloc(walls, numWalls * sizeof(wall));
          memcpy(walls, someWalls, (numWalls * sizeof(wall)));
        
        // Set entrances and exits
        down = Stair(15, 135, false);
        up = Stair(-15, -15, true);
        
        // Set player location
        if (ascending) {
          player.warp(down.x(), down.y());
        }
        else {
          player.warp(up.x(), up.y());
        }
        
        // Set items
        despawnItems();
        items.push_back(Item(ammoAnim, AMMO, 135, 15));
        items.push_back(Item(healthAnim, HEALTH, 45, 75));
        items.push_back(Item(kittensAnim, KITTENS, 135, 75));

        // Spawn enemies
        spawnEnemies();

        break;
      }

      default:
        break;
    }

	if (level == 1 && ascending)
		player.startSong1();
	else if (level != 0)
		Gosu::Sample(Gosu::resourcePrefix() + L"media/sounds/levelChange.wav").play();
    
    backgroundImage.reset(new Gosu::Image(graphics(), filename, true));
    environment = Environment(numWalls, walls);
  }
};


int main() 
{ 
    GameWindow window; 
    window.show(); 
} 
