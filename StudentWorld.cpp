#include "StudentWorld.h"
#include "GameConstants.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath) {
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), starPower(false), shootPower(false), jumpPower(false), player(nullptr), level_complete(false), game_won(false) {};

StudentWorld::~StudentWorld() {
    cleanUp();
}


bool StudentWorld::overlap(Actor* p, double x, double y) {
    double leftx = p->getX();
    double rightx = p->getX() + 7.0;
    double bottomy = p->getY();
    double topy = p->getY() + 7.0;
    if (x <= rightx && x >= leftx && y <= topy && y >= bottomy)
        return true;
    if (x + 7.0 <= rightx && x + 7.0 >= leftx && y <= topy && y >= bottomy)
        return true;
    if (x <= rightx && x >= leftx && y + 7.0 <= topy && y + 7.0 >= bottomy)
        return true;
    if (x + 7.0 <= rightx && x + 7.0 >= leftx && y + 7.0 <= topy && y + 7.0 >= bottomy)
        return true;
    return false;
}

bool StudentWorld::isSolidActorAt(double x, double y) {
    for (size_t i = 0; i < actorList.size(); i++) {
        if (overlap(actorList.at(i), x, y) && actorList.at(i)->getSolidity())
            return true;
    }
    return false;
}

bool StudentWorld::inSolid(double x, double y) {
    for (size_t i = 0; i < actorList.size(); i++)
        if (actorList.at(i)->getSolidity()) {
            Actor* p = actorList.at(i);
            double leftx = p->getX();
            double rightx = p->getX() + 7.0;
            double bottomy = p->getY();
            double topy = p->getY() + 7.0;
            if (x <= rightx && x >= leftx && y <= topy && y >= bottomy)
                return true;
        }
    return false;
}

Actor* StudentWorld::getActorAt(double x, double y) {
    for (size_t i = 0; i < actorList.size(); i++) {
        if (overlap(actorList.at(i), x, y))
            return actorList.at(i);
    }
    return nullptr;
}

Peach* StudentWorld::getPlayer() {
    return player;
}

void StudentWorld::addActor(Actor* p) {
    actorList.push_back(p);
}


void StudentWorld::setStarPower(bool setTo) {
    starPower = setTo;
}

bool StudentWorld::getStarPower() {
    return starPower;
}

void StudentWorld::setShootPower(bool setTo) {
    shootPower = setTo;
}

bool StudentWorld::getShootPower() {
    return shootPower;
}

void StudentWorld::setJumpPower(bool setTo) {
    jumpPower = setTo;
}

bool StudentWorld::getJumpPower() {
    return jumpPower;
}


void StudentWorld::setLevelComplete(bool setTo) {
    level_complete = setTo;
}

void StudentWorld::setGameWon(bool setTo) {
    game_won = setTo;
}

int StudentWorld::init() {
    level_complete = false;
    game_won = false;
    Level lev(assetPath());
    ostringstream level_file;
    level_file.fill('0');
    level_file << "level" << setw(2) << getLevel() << ".txt";
    string level = level_file.str();
    Level::LoadResult result = lev.loadLevel(level);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find " << level << " data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << level << " is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success) {
        cerr << "Successfully loaded " << level << endl;
    }
    for (int i = 0; i < 32; i++) 
        for (int j = 0; j < 32; j++) {
            Level::GridEntry ge;
            int posX = i * SPRITE_WIDTH;
            int posY = j * SPRITE_HEIGHT;
            ge = lev.getContentsOf(i, j);
            Actor* p = nullptr;
            switch (ge) {
            case Level::empty:
                break;
            case Level::peach:
                player = new Peach(this, posX, posY);
                break;
            case Level::block:
                p = new Block(this, posX, posY, -1);
                actorList.push_back(p);
                break;
            case Level::star_goodie_block:
                p = new Block(this, posX, posY, 1);
                actorList.push_back(p);
                break;
            case Level::mushroom_goodie_block:
                p = new Block(this, posX, posY, 2);
                actorList.push_back(p);
                break;
            case Level::flower_goodie_block:
                p = new Block(this, posX, posY, 3);
                actorList.push_back(p);
                break;
            case Level::pipe:
                p = new Pipe(this, posX, posY);
                actorList.push_back(p);
                break;
            case Level::flag:
                p = new Flag(this, posX, posY);
                actorList.push_back(p);
                break;
            case Level::mario:
                p = new Mario(this, posX, posY);
                actorList.push_back(p);
                break;
            case Level::goomba:
                p = new Goomba(this, posX, posY);
                actorList.push_back(p);
                break;
            case Level::koopa:
                p = new Koopa(this, posX, posY);
                actorList.push_back(p);
                break;
            case Level::piranha:
                p = new Piranha(this, posX, posY);
                actorList.push_back(p);
                break;
            }
        }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    player->doSomething();
    for (size_t i = 0; i < actorList.size(); i++)
        actorList.at(i)->doSomething();
    if (!player->getStatus()) {
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if (level_complete) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if (game_won) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    vector<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        if (!(*it)->getStatus()) {
            delete *it;
            it = actorList.erase(it);
        }
        else it++;
    }

    ostringstream status_text;
    status_text.fill('0');
    status_text << "Lives: " << getLives() << " Level: " << setw(2) << getLevel() << " Points: " << setw(6) << getScore();
    if (starPower)
        status_text << " StarPower!";
    if (shootPower)
        status_text << " ShootPower!";
    if (jumpPower)
        status_text << " JumpPower!";
    string status = status_text.str();
    setGameStatText(status);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    delete player;
    vector<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end()) {
        delete *it;
        it = actorList.erase(it);
    }
}
