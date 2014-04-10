#ifndef PLAYER_H
#define PLAYER_H

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

#include "Enemy.h"
#include "Item.h"
#include "Shot.h"

class Player
{
    /* CONSTANTS */

	static const int NUMBER_OF_FRAMES = 3;			// number of frames per walk cycle

	static const int LEFT_GUN_DOWN_CYCLE = 5;		// starting position of left gun down walk cycle = 5 * NUMBER_OF_FRAMES
	static const int RIGHT_GUN_DOWN_CYCLE = 2;		// starting position of right gun down walk cycle = 2 * NUMBER_OF_FRAMES
	static const int LEFT_GUN_FORWARD_CYCLE = 3;	// starting position of left gun forward walk cycle = 3 * NUMBER_OF_FRAMES
	static const int RIGHT_GUN_FORWARD_CYCLE = 0;	// starting position of right gun forward walk cycle = 0 * NUMBER_OF_FRAMES
	static const int LEFT_GUN_UP_CYCLE = 4;			// starting position of left gun up walk cycle = 4 * NUMBER_OF_FRAMES
	static const int RIGHT_GUN_UP_CYCLE = 1;		// starting position of right gun up walk cycle = 1 * NUMBER_OF_FRAMES
	static const int ON_FIRE_RIGHT_CYCLE = 6;		// starting position of on fire right cycle = 6 * NUMBER_OF_FRAMES
	static const int ON_FIRE_LEFT_CYCLE = 9;		// starting position of on fire right cycle = 6 * NUMBER_OF_FRAMES

	static const int GUN_DOWN = 0;
	static const int GUN_FORWARD = 1;
	static const int GUN_UP = 2;

	
	Animation& animation;
	int posX, posY;

	int currentWalkCycle;
	int currentDirection;
	int currentWalkCycleDirection;
	int gunPosition;
	bool standingStill;
	unsigned long lastTurnTime;

	bool onFire;
	bool dead;
	bool recovering;
	unsigned long recoveringStartTime;

	int ammo;
	int health;
	int score;

	int startAmmo;
	int startHealth;

	bool ascending;

	int onFireFrameCount;

	Gosu::Song* song1;
	std::wstring song1Filename;
	Gosu::Song* song2;
	std::wstring song2Filename;

    public:
		Player(Animation& animation);

		int x() const;
		int y() const;

		bool isOnFire() const;
		bool isDead() const;
		bool isStandingStill() const;

		int getAmmo() const;
		int getHealth() const;
		int getScore() const;
    void setAscending(bool a);
		bool isAscending() const;

		int shotDirection() const;

		int getCurrentWalkCycleDirection() const;

    void warp(int x, int y);
		void move();
    void moveLeft();
    void moveRight();
		void moveUp();
    void moveDown();
		void turnLeft();
		void turnRight();

		void moveGunUp();
		void moveGunDown();

		void shoot(Shot& shot, std::list<Enemy>& enemies, Environment& env);

		void checkForEnemyCollisions(std::list<Enemy>& enemies);
		void checkForItemCollisions(std::list<Item>& items);

        void draw();

		void songUpdate();

	private:
		void changeWalkCycle();
};

#endif
