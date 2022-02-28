#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(StudentWorld* world, int health, bool status, bool solidity, int imageID, double startX, double startY, int startDirection, int depth, double size)
	: GraphObject(imageID, startX, startY, startDirection, depth, size), hitPoints(health), isAlive(status), isSolid(solidity), worldPointer(world) {};

void Actor::peachBonk() {
	bonk();
}

void Actor::beDamaged() {
	return;
}

int Actor::getHealth() {
	return hitPoints;
}

void Actor::addToHealth(int adding) {
	hitPoints += adding;
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
	starPowerTicks(0), tempInvincTicks(0), time_to_recharge_before_next_fire(0), remaining_jump_distance(0) {};

void Peach::setStarPowerTicks() {
	starPowerTicks = 149;
}

void Peach::bonk() {
	if (!getWorld()->getStarPower()) {
		addToHealth(-1);
		tempInvincTicks = 10;
		getWorld()->setShootPower(false);
		getWorld()->setJumpPower(false);
		if (getHealth() > 0)
			getWorld()->playSound(SOUND_PLAYER_HURT);
		if (getHealth() <= 0)
			setStatus(false);
	}
}

void Peach::doSomething() {
	if (!getStatus())
		return;
	if (getWorld()->getStarPower()) {
		starPowerTicks--;
		if (starPowerTicks == 0)
			getWorld()->setStarPower(false);
	}
	if (tempInvincTicks > 0)
		tempInvincTicks--;
	if (time_to_recharge_before_next_fire > 0)
		time_to_recharge_before_next_fire--;
	if (getWorld()->getActorAt(getX(), getY()) != nullptr)
		getWorld()->getActorAt(getX(), getY())->peachBonk();
	if (remaining_jump_distance > 0) {
		double targetY = getY() + 4.0;
		if (getWorld()->isSolidActorAt(getX(), targetY)) {
			getWorld()->getActorAt(getX(), targetY)->peachBonk();
			remaining_jump_distance = 0;
		}
		else if (getWorld()->getActorAt(getX(), targetY) == nullptr) {
			moveTo(getX(), targetY);
			remaining_jump_distance--;
		}
	}
	else if (!getWorld()->isSolidActorAt(getX(), getY() - 1.0) && !getWorld()->isSolidActorAt(getX(), getY() - 2.0) && !getWorld()->isSolidActorAt(getX(), getY() - 3.0)) //FALLING
		moveTo(getX(), getY() - 4.0);
	int keyPressed = 0;
	double target = 0.0;
	if (getWorld()->getKey(keyPressed))
		switch (keyPressed) {
		case KEY_PRESS_LEFT:
			setDirection(180);
			target = getX() - 4.0;
			if (getWorld()->isSolidActorAt(target, getY()))
				getWorld()->getActorAt(target, getY())->bonk();
			else moveTo(target, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(0);
			target = getX() + 4.0;
			if (getWorld()->isSolidActorAt(target, getY()))
				getWorld()->getActorAt(target, getY())->bonk();
			else moveTo(target, getY());
			break;
		case KEY_PRESS_UP:
			if (getWorld()->isSolidActorAt(getX(), getY() - 4.0)) {
				if (getWorld()->getJumpPower()) remaining_jump_distance = 12;
				else remaining_jump_distance = 8;
				getWorld()->playSound(SOUND_PLAYER_JUMP);
			}
			break;
		case KEY_PRESS_SPACE:
			if (getWorld()->getShootPower() && time_to_recharge_before_next_fire == 0) {
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				time_to_recharge_before_next_fire = 8;
				double targetX = getX() + 4.0;
				if (getDirection() == 180)
					targetX = getX() - 4.0;
				Peach_Fireball* f = new Peach_Fireball(getWorld(), targetX, getY(), getDirection());
				getWorld()->addActor(f);
			}
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
	if (goodieReleased == true || whichGoodie == -1)	// THESE DOUBLES ARE RIGHT
		getWorld()->playSound(SOUND_PLAYER_BONK);
	else {
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		switch (whichGoodie) {
		case 1: {
			Star* s = new Star(getWorld(), getX(), getY() + 8.0);
			getWorld()->addActor(s);
			break;
		}
		case 2: {
			Mushroom* m = new Mushroom(getWorld(), getX(), getY() + 8.0);
			getWorld()->addActor(m);
			break;
		}
		case 3: {
			Flower* f = new Flower(getWorld(), getX(), getY() + 8.0);
			getWorld()->addActor(f);
			break;
		}
		}
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
	getWorld()->setLevelComplete(true);
}

Mario::Mario(StudentWorld* world, double startX, double startY)
	: Goals(world, startX, startY, IID_MARIO) {};

void Mario::advanceGame() {
	getWorld()->setGameWon(true);
}

void Goals::doSomething() {
	if (!getStatus()) return;
	else {
		if (getWorld()->overlap(getWorld()->getPlayer(), getX(), getY())) {
			getWorld()->increaseScore(1000);
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
	if (getWorld()->overlap(getWorld()->getPlayer(), getX(), getY())) {
		doSomethingHelper();
		setStatus(false);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		return;
	}
	double targetY = getY() - 2.0;
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

void Flower::doSomethingHelper() {
	getWorld()->increaseScore(50);
	getWorld()->setShootPower(true);
	getWorld()->getPlayer()->addToHealth(1);
}

Mushroom::Mushroom(StudentWorld* world, double startX, double startY)
	: Goodies(world, startX, startY, IID_MUSHROOM) {};

void Mushroom::doSomethingHelper() {
	getWorld()->increaseScore(75);
	getWorld()->setJumpPower(true);
	getWorld()->getPlayer()->addToHealth(1);
}

Star::Star(StudentWorld* world, double startX, double startY)
	: Goodies(world, startX, startY, IID_STAR) {};

void Star::doSomethingHelper() {
	getWorld()->increaseScore(100);
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
	if (getWorld()->getActorAt(getX(), getY())->getHealth() != -1) {
		getWorld()->getActorAt(getX(), getY())->beDamaged();
		setStatus(false);
		return;
	}
	double targetY = getY() - 2.0;
	if (!getWorld()->inSolid(getX(), targetY))
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
	if (getWorld()->overlap(getWorld()->getPlayer(), getX(), getY())) {
		getWorld()->getPlayer()->addToHealth(-1);
		setStatus(false);
		return;
	}
	double targetY = getY() - 2.0;
	if (!getWorld()->inSolid(getX(), targetY))
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

Peach_Fireball::Peach_Fireball(StudentWorld* world, double startX, double startY, int startDir)
	: Projectiles(world, startX, startY, startDir, IID_PEACH_FIRE) {};

Shell::Shell(StudentWorld* world, double startX, double startY, int startDir)
	: Projectiles(world, startX, startY, startDir, IID_SHELL) {};

//---------------------------------------------------------------------------------------

Monsters::Monsters(StudentWorld* world, double startX, double startY, int imageID)
	: Actor(world, 1, true, false, imageID, startX, startY, 180*(rand() % 2), 0) {};

void Monsters::beDamaged() {
	getWorld()->increaseScore(100);
	setStatus(false);
}

void Monsters::bonk() {
	return;
}

void Monsters::peachBonk() {
	if (getWorld()->getStarPower()) {
		getWorld()->playSound(SOUND_PLAYER_KICK);
		beDamaged();
	}
}

void Monsters::doSomething() {
	if (!getStatus())
		return;
	if (getWorld()->overlap(getWorld()->getPlayer(), getX(), getY())) {
		getWorld()->getPlayer()->bonk();
		return;
	}
	double targetX = getX() + 1.0;
	if (getDirection() == 180)
		targetX = getX() - 1.0;
	if (getWorld()->isSolidActorAt(targetX, getY()))
		changeDirection();
	else if (!getWorld()->inSolid(targetX, getY() - 1.0))
		changeDirection();
	if (getDirection() == 0)
		targetX = getX() + 1.0;
	else targetX = getX() - 1.0;
	if (getWorld()->isSolidActorAt(targetX, getY()))
		return;
	else moveTo(targetX, getY());
}

Goomba::Goomba(StudentWorld* world, double startX, double startY)
	: Monsters(world, startX, startY, IID_GOOMBA) {};

Koopa::Koopa(StudentWorld* world, double startX, double startY)
	: Monsters(world, startX, startY, IID_KOOPA) {};

void Koopa::beDamaged() {
	getWorld()->increaseScore(100);
	setStatus(false);
	Shell* s = new Shell(getWorld(), getX(), getY(), getDirection());
	getWorld()->addActor(s);
}

Piranha::Piranha(StudentWorld* world, double startX, double startY)
	: Monsters(world, startX, startY, IID_PIRANHA), fireDelay(0) {};

void Piranha::doSomething() {
	if (!getStatus()) return;
	increaseAnimationNumber();
	if (getWorld()->overlap(getWorld()->getPlayer(), getX(), getY())) {
		getWorld()->getPlayer()->bonk();
		return;
	}
	double playerY = getWorld()->getPlayer()->getY();
	if (playerY > getY() + 12.0 || playerY < getY() - 12.0)
		return;
	double playerX = getWorld()->getPlayer()->getX();
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




