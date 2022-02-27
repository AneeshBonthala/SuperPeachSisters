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

  bool isSolidActorAt(double x, double y);
  Actor* getActorAt(double x, double y);
  Peach* getPlayer();
  void addActor(Actor* p);

  void addToScore(int adding);
  void setStarPower(bool setTo);
  bool getStarPower();
  void setShootPower(bool setTo);
  bool getShootPower();
  void setJumpPower(bool setTo);
  bool getJumpPower();

  virtual int init();
  virtual int move();
  virtual void cleanUp();

private:
	int score;
	int lives;
	bool starPower;
	bool shootPower;
	bool jumpPower;
	Peach* player;
	std::vector<Actor*> actorList;
};

#endif // STUDENTWORLD_H_
