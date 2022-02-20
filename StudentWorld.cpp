#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath) {
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), player(nullptr) {};

StudentWorld::~StudentWorld() {
    cleanUp();
}

/*bool StudentWorld::isActorAt(double x, double y, bool& solid, Actor* p) {
    for (size_t i = 0; i < actorList.size(); i++) {
        if (actorList.at(i)->getX() == x && actorList.at(i)->getY() == y) {
            if (actorList.at(i)->getSolidity()) solid = true;
            p = actorList.at(i);
            return true;
        }
    }
    return false;
}

bool StudentWorld::isSolidActorAt(double x, double y) {
    bool solid = false, result;
    Actor* p = nullptr;
    result = isActorAt(x, y, solid, p);
    if (solid == true)
        return true;
    return false;
}

Actor* StudentWorld::getActorAt(double x, double y) {
    bool solid = false, result;
    Actor* p = nullptr;
    result = isActorAt(x, y, solid, p);
    return p;
}*/

bool StudentWorld::isSolidActorAt(double x, double y) {
    for (size_t i = 0; i < actorList.size(); i++) {
        if (actorList.at(i)->getX() == x && actorList.at(i)->getY() == y) {
            if (actorList.at(i)->getSolidity()) 
                return true;
        }
    }
    return false;
}

Actor* StudentWorld::getActorAt(double x, double y) {
    for (size_t i = 0; i < actorList.size(); i++) {
        if (actorList.at(i)->getX() == x && actorList.at(i)->getY() == y)
            return actorList.at(i);    
    }
    return nullptr;
}

int StudentWorld::init() {
    Level lev(assetPath());
    string level_file = "level01.txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find " << level_file << " data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << level_file << " is improperly formatted" << endl;
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
    }
    for (int i = 0; i < 32; i++) 
        for (int j = 0; j < 32; j++) {
            Level::GridEntry ge;
            int posX = i * SPRITE_WIDTH;
            int posY = j * SPRITE_HEIGHT;
            ge = lev.getContentsOf(i, j);
            Actor* p = nullptr;
            switch (ge) {
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
            }
        }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    player->doSomething();
    for (size_t i = 0; i < actorList.size(); i++)
        actorList.at(i)->doSomething();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    vector<Actor*>().swap(actorList);
}
