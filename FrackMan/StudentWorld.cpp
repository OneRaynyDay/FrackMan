#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init(){
    player = new FrackMan(this);
    for(int j = 0; j < VIEW_HEIGHT; j++){
        for(int i = 0; i < DIRT_ROWS; i++){
            if(j >= 30 && j <= 33)
                continue;
            // passing in j as x and i as y
            Dirt* currDirt = new Dirt(j, i);
            dirt.push_back(currDirt);
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    /* it must ask your FrackMan object to do something. Your
     move() method need not check to see if the FrackMan has died or
     not; you may assume at this point that the FrackMan cannot die.
     Nor need your move() method deal with any Protesters or other
     actors (e.g., Nuggets or Boulders) at this point – just the
     FrackMan.
     */
    player->doSomething();
    //        decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    /*Implement a cleanup() method that frees any dynamically
     allocated data that was allocated during calls to the init() method or
     the move() method (e.g., it should delete all your allocated Dirt and
     the FrackMan).
     Note: Your StudentWorld class must have both a
     destructor and the cleanUp() method even though they likely do
     the same thing.
     */
    for(int i = dirt.size()-1; i>=0; i--){
        delete dirt[i];
        dirt.pop_back();
    }
    for(int i = actor.size()-1; i>=0; i--){
        delete actor[i];
        actor.pop_back();
    }
    delete player;
}

void StudentWorld::removeDirt(int x, int y, int size){
    for(int i = 0; i < dirt.size(); i++){
        if(inRange(dirt[i]->getX(), dirt[i]->getY(), x, y, size, size)){
            //remove dirt
            delete dirt[i];
            dirt.erase(dirt.begin() + i);
            //play sound
            playSound(SOUND_DIG);
        }
    }
}
