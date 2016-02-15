#include "StudentWorld.h"
#include <string>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::~StudentWorld()
{
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

bool StudentWorld::sparseEnough(int x, int y){
    for(int i = 0; i < actor.size(); i++){
        if(dist(actor[i]->getX(), actor[i]->getY(), x, y) < CHARACTER_SPACING)
            return false;
    }
    return true;
}

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
    
    // Take care of level stats
    curLevel++;
    // boulders
    int B = min(curLevel / 2 + 2, 6);
    // gold nuggets
    int G = max(5-curLevel / 2, 2);
    // barrels
    int L = min(2 + curLevel, 20);
    curBarrels = L;
    std::cout<<"Current barrel num : " << curBarrels<<endl;
    /*load the barrels*/
    for(int i = 0; i < L; i++){
        int x = -1, y = -1; // This will get through the do while at least once.
        do{
            x = (int)((double) rand() / (RAND_MAX) * WORLD_X);
            y = (int)((double) rand() / (RAND_MAX) * DIRT_ROWS);
        }while(!sparseEnough(x, y));
        Barrel* barrel = new Barrel(this, player, x, y);
        std::cout<<"x : " << x << " y : " << y;
        actor.push_back(barrel);
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
    if(curBarrels == 0)
        return GWSTATUS_FINISHED_LEVEL;
    if(player->isDead())
        return GWSTATUS_PLAYER_DIED;
    player->doSomething();
    //        decLives();
    
    /* removes things */
    for (vector<Actor*>::iterator it = actor.begin() ; it != actor.end();){
        if((*it)->isDead()){
            delete *it;
            it = actor.erase(it);
        }
        else{
            (*it)->doSomething();
            it++;
        }
    }

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
    bool flag = false;
    for (vector<Dirt*>::iterator it = dirt.begin() ; it != dirt.end();){
        if(inRange((*it)->getX(), (*it)->getY(), x, y, size, size)){
            //remove dirt
            delete *it;
            it = dirt.erase(it);
            flag = true;
        }
        else{
            it++;
        }
    }
    //play sound
    if(flag)
        playSound(SOUND_DIG);
}

void squirt(int x, int y, Actor::Direction dir){
    
}
