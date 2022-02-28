#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  bool overlap(Actor* p, double x, double y);
  bool isSolidActorAt(double x, double y);
  bool inSolid(double x, double y);
  Actor* getActorAt(double x, double y);
  Peach* getPlayer();
  void addActor(Actor* p);
  void setStarPower(bool setTo);
  bool getStarPower();
  void setShootPower(bool setTo);
  bool getShootPower();
  void setJumpPower(bool setTo);
  bool getJumpPower();
  void setLevelComplete(bool setTo);
  void setGameWon(bool setTo);

  virtual int init();
  virtual int move();
  virtual void cleanUp();

private:
	bool starPower;
	bool shootPower;
	bool jumpPower;
	Peach* player;
	std::vector<Actor*> actorList;

	bool level_complete;
	bool game_won;
};

#endif // STUDENTWORLD_H_
