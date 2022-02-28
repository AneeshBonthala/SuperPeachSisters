#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(StudentWorld* world, int health, bool status, bool solidity, int imageID, double startX, double startY, int startDirection = 0, int depth = 0, double size = 1.0);
	virtual void bonk() = 0;
	virtual void peachBonk();
	virtual void doSomething() = 0;
	virtual void beDamaged();
	int getHealth();
	void addToHealth(int adding);
	bool getStatus();
	void setStatus(bool status);
	bool getSolidity();
	StudentWorld* getWorld();

	void changeDirection();

private:
	int hitPoints;	// -1 for none
	bool isAlive;
	bool isSolid;
	StudentWorld* worldPointer;
};

//--------------------------------------------------------------------------------------------

class Peach : public Actor {
public:
	Peach(StudentWorld* world, double startX, double startY);
	void setStarPowerTicks();
	virtual void beDamaged();
	virtual void bonk();
	virtual void doSomething();
private:
	int starPowerTicks;
	int tempInvincTicks;
	int time_to_recharge_before_next_fire;
	int remaining_jump_distance;
};

//--------------------------------------------------------------------------------------------

class Blocks : public Actor {
public:
	Blocks(StudentWorld* world, double startX, double startY, int imageID);
	virtual void bonk() = 0;
	virtual void doSomething();
};

class Block : public Blocks {
public:
	Block(StudentWorld* world, double startX, double startY, int goodie);
	virtual void bonk();
private:
	int whichGoodie;	// -1 for none
	bool goodieReleased;
};

class Pipe : public Blocks {
public:
	Pipe(StudentWorld* world, double startX, double startY);
	virtual void bonk();
};

//--------------------------------------------------------------------------------------------

class Goals : public Actor {
public:
	Goals(StudentWorld* world, double startX, double startY, int imageID);
	virtual void bonk();
	virtual void doSomething();
private:
	virtual void advanceGame() = 0;
};

class Flag : public Goals {
public:
	Flag(StudentWorld* world, double startX, double startY);
private:
	virtual void advanceGame();
};

class Mario : public Goals {
public:
	Mario(StudentWorld* world, double startX, double startY);
private:
	virtual void advanceGame();
};

//--------------------------------------------------------------------------------------------

class Goodies : public Actor {
public:
	Goodies(StudentWorld* world, double startX, double startY, int imageID);
	virtual void bonk();
	void doSomething();
private:
	virtual void doSomethingHelper() = 0;
};

class Flower : public Goodies {
public:
	Flower(StudentWorld* world, double startX, double startY);
private:
	virtual void doSomethingHelper();
};

class Mushroom : public Goodies {
public:
	Mushroom(StudentWorld* world, double startX, double startY);
private:
	virtual void doSomethingHelper();
};

class Star : public Goodies {
public:
	Star(StudentWorld* world, double startX, double startY);
private:
	virtual void doSomethingHelper();
};

//--------------------------------------------------------------------------------------------

class Projectiles : public Actor {
public:
	Projectiles(StudentWorld* world, double startX, double startY, int startDir, int imageID);
	virtual void bonk();
	virtual void doSomething();
};

class Piranha_Fireball : public Projectiles {
public:
	Piranha_Fireball(StudentWorld* world, double startX, double startY, int startDir);
	virtual void doSomething();
};

class Peach_Fireball : public Projectiles {
public:
	Peach_Fireball(StudentWorld* world, double startX, double startY, int startDir);
};

class Shell : public Projectiles {
public:
	Shell(StudentWorld* world, double startX, double startY, int startDir);
};

//--------------------------------------------------------------------------------------------

class Monsters : public Actor {
public:
	Monsters(StudentWorld* world, double startX, double startY, int imageID);
	virtual void beDamaged();
	virtual void bonk();
	void peachBonk();
	virtual void doSomething();
};

class Goomba : public Monsters {
public:
	Goomba(StudentWorld* world, double startX, double startY);
};

class Koopa : public Monsters {
public:
	Koopa(StudentWorld* world, double startX, double startY);
	virtual void beDamaged();
};

class Piranha : public Monsters {
public:
	Piranha(StudentWorld* world, double startX, double startY);
	virtual void doSomething();
private:
	int fireDelay;
};

#endif // ACTOR_H_