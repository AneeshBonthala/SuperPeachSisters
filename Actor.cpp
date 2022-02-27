#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(StudentWorld* world, int health, bool status, bool solidity, int imageID, double startX, double startY, int startDirection, int depth, double size)
	: GraphObject(imageID, startX, startY, startDirection, depth, size), hitPoints(health), damaged(false), isAlive(status), isSolid(solidity), worldPointer(world) {};


int Actor::getHealth() {
	return hitPoints;
}

void Actor::addToHealth(int adding) {
	hitPoints += adding;
}

bool Actor::getDamaged() {
	return damaged;
}

void Actor::setDamaged(bool setTo) {
	damaged = setTo;
}

bool Actor::getStatus() {
	return isAlive;
}

void Actor::setStatus(bool status) {
	isAlive = status;
}

bool Actor::getSolidity() {
	return isSolid;
}

StudentWorld* Actor::getWorld() {
	return worldPointer;
}

void Actor::changeDirection() {
	if (getDirection() == 0)
		setDirection(180);
	else setDirection(0);
}

//--------------------------------------------------------------------------------------

Peach::Peach(StudentWorld* world, double startX, double startY)
	: Actor(world, 1, true, true, IID_PEACH, startX, startY),
	isInRechargeMode(false), isInvincible(false), starPowerTicks(0), isTempInvincible(false), initiatedJump(false), isFalling(false), remaining_jump_distance(0) {};

void Peach::setStarPowerTicks() {
	starPowerTicks = 149;
}

void Peach::bonk() {
	return;	// to be changed
}

void Peach::doSomething() {
	if (getWorld()->isSolidActorAt(getX(), getY()))
		if (getWorld()->getActorAt(getX(), getY()) != nullptr)
			getWorld()->getActorAt(getX(), getY())->bonk();
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

//---------------------------------------------------------------------------------------

Blocks::Blocks(StudentWorld* world, double startX, double startY, int imageID)
	: Actor(world, -1, true, true, imageID, startX, startY, 0, 2) {};

void Blocks::doSomething() {
	return;
}

Block::Block(StudentWorld* world, double startX, double startY, int goodie)
	: Blocks(world, startX, startY, IID_BLOCK), whichGoodie(goodie), goodieReleased(false) {};

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

Pipe::Pipe(StudentWorld* world, double startX, double startY)
	: Blocks(world, startX, startY, IID_PIPE) {};

void Pipe::bonk() {
	getWorld()->playSound(SOUND_PLAYER_BONK);
}

//---------------------------------------------------------------------------------------

Goals::Goals(StudentWorld* world, double startX, double startY, int imageID)
	: Actor(world, -1, true, false, imageID, startX, startY, 0, 1) {};

void Goals::bonk() {
	return;
}

Flag::Flag(StudentWorld* world, double startX, double startY)
	: Goals(world, startX, startY, IID_FLAG) {};

void Flag::advanceGame() {
	return;
}

Mario::Mario(StudentWorld* world, double startX, double startY)
	: Goals(world, startX, startY, IID_MARIO) {};

void Mario::advanceGame() {
	return;
}

void Goals::doSomething() {
	if (getStatus()) return;
	else {
		double playerX = getWorld()->getPlayer()->getX();
		double playerY = getWorld()->getPlayer()->getY();
		if (playerX == getX() && playerY == getY()) {
			getWorld()->addToScore(1000);
			setStatus(false);
			advanceGame();
		}
	}
}

//---------------------------------------------------------------------------------------

Goodies::Goodies(StudentWorld* world, double startX, double startY, int imageID)
	: Actor(world, -1, true, false, imageID, startX, startY, 0, 1) {};

void Goodies::bonk() {
	return;
}

void Goodies::doSomething() {
	double playerX = getWorld()->getPlayer()->getX();
	double playerY = getWorld()->getPlayer()->getY();
	if (playerX == getX() && playerY == getY()) {
		doSomethingOverlap();
		setStatus(false);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		return;
	}
	double targetY = getY() - 2;
	if (!getWorld()->isSolidActorAt(getX(), targetY))
		moveTo(getX(), targetY);
	double targetX = getX() + 2.0;
	if (getDirection() == 180)
		targetX = getX() - 2.0;
	if (getWorld()->isSolidActorAt(targetX, getY())) {
		changeDirection();
		return;
	}
	moveTo(targetX, getY());
}

Flower::Flower(StudentWorld* world, double startX, double startY)
	: Goodies(world, startX, startY, IID_FLOWER) {};

void Flower::doSomethingOverlap() {
	getWorld()->addToScore(50);
	getWorld()->setShootPower(true);
	getWorld()->getPlayer()->addToHealth(1);
}

Mushroom::Mushroom(StudentWorld* world, double startX, double startY)
	: Goodies(world, startX, startY, IID_MUSHROOM) {};

void Mushroom::doSomethingOverlap() {
	getWorld()->addToScore(75);
	getWorld()->setJumpPower(true);
	getWorld()->getPlayer()->addToHealth(1);
}

Star::Star(StudentWorld* world, double startX, double startY)
	: Goodies(world, startX, startY, IID_STAR) {};

void Star::doSomethingOverlap() {
	getWorld()->addToScore(100);
	getWorld()->setStarPower(true);
	getWorld()->getPlayer()->setStarPowerTicks();
}

//---------------------------------------------------------------------------------------

Projectiles::Projectiles(StudentWorld* world, double startX, double startY, int startDir, int imageID)
	: Actor(world, -1, true, false, imageID, startX, startY, startDir, 1) {};

void Projectiles::bonk() {
	return;
}

void Projectiles::doSomething() {
	if (getWorld()->getActorAt(getX(), getY())) {
		getWorld()->getActorAt(getX(), getY())->setDamaged(true);
		setStatus(false);
		return;
	}
	double targetY = getY() - 2.0;
	if (!getWorld()->isSolidActorAt(getX(), targetY))
		moveTo(getX(), targetY);
	double targetX = getX() + 2.0;
	if (getDirection() == 180)
		targetX = getX() - 2.0;
	if (getWorld()->isSolidActorAt(targetX, getY())) {
		setStatus(false);
		return;
	}
	moveTo(targetX, getY());
}

Piranha_Fireball::Piranha_Fireball(StudentWorld* world, double startX, double startY, int startDir)
	: Projectiles(world, startX, startY, startDir, IID_PIRANHA_FIRE) {};

void Piranha_Fireball::doSomething() {
	double playerX = getWorld()->getPlayer()->getX();
	double playerY = getWorld()->getPlayer()->getY();
	if (playerX == getX() && playerY == getY()) {
		getWorld()->getPlayer()->setDamaged(true);
		setStatus(false);
		return;
	}
	double targetY = getY() - 2.0;
	if (!getWorld()->isSolidActorAt(getX(), targetY))
		moveTo(getX(), targetY);
	double targetX = getX() + 2.0;
	if (getDirection() == 180)
		targetX = getX() - 2.0;	//make a function
	if (getWorld()->isSolidActorAt(targetX, getY())) {
		setStatus(false);
		return;
	}
	moveTo(targetX, getY());
}

Peach_Fireball::Peach_Fireball(StudentWorld* world, double startX, double startY, int startDir)
	: Projectiles(world, startX, startY, startDir, IID_PEACH_FIRE) {};

Shell::Shell(StudentWorld* world, double startX, double startY, int startDir)
	: Projectiles(world, startX, startY, startDir, IID_SHELL) {};

//---------------------------------------------------------------------------------------

Monsters::Monsters(StudentWorld* world, double startX, double startY, int imageID)
	: Actor(world, 1, true, false, imageID, startX, startY, 180*(rand() % 2), 0) {};

void Monsters::bonk() {
	//make sure its peach
	if (getWorld()->getStarPower()) {
		getWorld()->playSound(SOUND_PLAYER_KICK);
		getWorld()->increaseScore(100);
		setStatus(false);
	}
	if (getDamaged()) {
		getWorld()->increaseScore(100);
		setStatus(false);
	}
}

void Monsters::doSomething() {
	if (!getStatus())
		return;
	double playerX = getWorld()->getPlayer()->getX();
	double playerY = getWorld()->getPlayer()->getY();
	if (playerX == getX() && playerY == getY()) {
		getWorld()->getPlayer()->bonk();
		return;
	}
	double targetX = getX() + 1.0;
	if (getDirection() == 180)
		targetX = getX() - 1.0;
	if (getWorld()->isSolidActorAt(targetX, getY()))
		changeDirection();
	double belowY = getY() - 1.0;
	if (!getWorld()->isSolidActorAt(targetX, belowY))
		changeDirection();
	if (getDirection() == 0)
		targetX = getX() + 1.0;
	else targetX = getX() - 1.0;
	if (!getWorld()->isSolidActorAt(targetX, getY()))
		moveTo(targetX, getY());
	else return;
}

Goomba::Goomba(StudentWorld* world, double startX, double startY)
	: Monsters(world, startX, startY, IID_GOOMBA) {};

Koopa::Koopa(StudentWorld* world, double startX, double startY)
	: Monsters(world, startX, startY, IID_KOOPA) {};

void Koopa::bonk() {
	//make sure its peach
	if (getWorld()->getStarPower()) {
		getWorld()->playSound(SOUND_PLAYER_KICK);
		getWorld()->increaseScore(100);
		setStatus(false);
	}
	if (getDamaged()) {
		getWorld()->increaseScore(100);
		setStatus(false);
		Shell* s = new Shell(getWorld(), getX(), getY(), getDirection());
		getWorld()->addActor(s);
	}
}

Piranha::Piranha(StudentWorld* world, double startX, double startY)
	: Monsters(world, startX, startY, IID_PIRANHA), fireDelay(0) {};

void Piranha::doSomething() {
	if (!getStatus()) return;
	increaseAnimationNumber();
	double playerX = getWorld()->getPlayer()->getX();
	double playerY = getWorld()->getPlayer()->getY();
	if (playerX == getX() && playerY == getY()) {
		getWorld()->getPlayer()->bonk();
		return;
	}
	if (playerY > getY() + 12 || playerY < getY())
		return;
	if (playerX < getX())
		setDirection(180);
	if (playerX > getX())
		setDirection(0);
	if (fireDelay > 0) {
		fireDelay--;
		return;
	}
	double distance = abs(playerX - getX());
	if (distance < 64.0) {
		Piranha_Fireball* f = new Piranha_Fireball(getWorld(), getX(), getY(), getDirection());
		getWorld()->addActor(f);
		getWorld()->playSound(SOUND_PIRANHA_FIRE);
		fireDelay = 40;
	}
}




