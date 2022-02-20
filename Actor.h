#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(StudentWorld* world, int health, bool status, bool solidity, int imageID, int startX, int startY, int startDirection, int depth = 0, double size = 1.0);
	virtual void bonk() = 0;
	virtual void doSomething() = 0;
	virtual int damage();
	int getHealth();
	StudentWorld* getWorld();

private:
	int hitPoints;	// -1 for none
	bool isAlive;
	bool isSolid;
	StudentWorld* worldPointer;
};

class Blocks : public Actor {
public:
	Blocks(StudentWorld* world, int startX, int startY, int imageID);
	virtual void bonk() = 0;
	virtual void doSomething();
};

class Block : public Blocks {
public:
	Block(StudentWorld* world, int startX, int startY, int goodie);
	virtual void bonk();
private:
	int whichGoodie;	// -1 for none
	bool goodieReleased;
};

class Pipe : public Blocks {
public:
	Pipe(StudentWorld* world, int startX, int startY);
	virtual void bonk();
};

class Peach : public Actor {
public:
	Peach(StudentWorld* world, int startX, int startY);
	virtual void bonk();
	virtual void doSomething();
private:
	bool isInRechargeMode;
	bool isInvincible;
	bool isTempInvincible;
	bool initiatedJump;
	bool isFalling;
	int remaining_jump_distance;
};




#endif // ACTOR_H_
