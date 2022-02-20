#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(StudentWorld* world, int health, bool status, bool solidity, int imageID, int startX, int startY, int startDirection, int depth, double size)
	: GraphObject(imageID, startX, startY, startDirection, depth, size), worldPointer(world), hitPoints(health), isAlive(status), isSolid(solidity) {};

int Actor::damage() {
	return 0;
}

int Actor::getHealth() {
	return hitPoints;
}

bool Actor::getSolidity() {
	return isSolid;
}

StudentWorld* Actor::getWorld() {
	return worldPointer;
}

Blocks::Blocks(StudentWorld* world, int startX, int startY, int imageID)
	: Actor(world, -1, true, true, imageID, startX, startY, 0, 2) {};

void Blocks::doSomething() {
	return;
}

Block::Block(StudentWorld* world, int startX, int startY, int goodie)
	: Blocks(world, startX, startY, IID_BLOCK), goodieReleased(false), whichGoodie(goodie) {};

void Block::bonk() {
	if (goodieReleased == true || whichGoodie == -1) 
		getWorld()->playSound(SOUND_PLAYER_BONK);
	else {
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		//...
		goodieReleased = true;
	}
	return;
}

Pipe::Pipe(StudentWorld* world, int startX, int startY)
	: Blocks(world, startX, startY, IID_PIPE) {};

void Pipe::bonk() {
	getWorld()->playSound(SOUND_PLAYER_BONK);
}

Peach::Peach(StudentWorld* world, int startX, int startY)
	: Actor(world, 1, true, true, IID_PEACH, startX, startY, 0), isInRechargeMode(false), isInvincible(false), isTempInvincible(false), initiatedJump(false), isFalling(false), remaining_jump_distance(0) {};

void Peach::bonk() {
	return;
}

void Peach::doSomething() {
	if (getWorld()->isSolidActorAt(getX(), getY())) {
		if (getWorld()->getActorAt(getX(), getY()) != nullptr)
			getWorld()->getActorAt(getX(), getY())->bonk();
	}
	int keyPressed = 0;
	double target = 0.0;
	if (getWorld()->getKey(keyPressed))
		switch (keyPressed) {
		case KEY_PRESS_LEFT:
			setDirection(180);
			target = getX() - 4.0;
			if (getWorld()->isSolidActorAt(target - 4.0, getY()))
				getWorld()->getActorAt(target - 4.0, getY())->bonk();
			else moveTo(target, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(0);
			target = getX() + 4.0;
			if (getWorld()->isSolidActorAt(target + 4.0, getY()))
				getWorld()->getActorAt(target + 4.0, getY())->bonk();
			else moveTo(target, getY());
			break;
		default: break;
		}
	return;
}