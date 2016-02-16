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
    for(int i = 0; i < WORLD_X; i++){
        for(int j = 0; j < DIRT_ROWS; j++){
            if(dirt[i][j] == nullptr)
                continue;
            delete dirt[i][j];
            dirt[i][j] = nullptr;
        }
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

void StudentWorld::generateCoord(int &xf, int &yf){
    int x = -1, y = -1;
    do{
        x = (int)((double) rand() / (RAND_MAX) * WORLD_X);
        y = (int)((double) rand() / (RAND_MAX) * DIRT_ROWS);
    }while(!sparseEnough(x, y));
    xf = x;
    yf = y;
}

int StudentWorld::init(){
    player = new FrackMan(this);
    for(int i = 0; i < WORLD_X; i++){
        for(int j = 0; j < DIRT_ROWS; j++){
            if(i >= 30 && i <= 33)
                continue;
            dirt[i][j] = new Dirt(i, j);
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
    
    
    for(int i = 0; i < G; i++){
        int x = -1, y = -1; // This will get through the do while at least once.
        generateCoord(x, y);
        Nugget* nugget = new Nugget(this, player, x, y);
        actor.push_back(nugget);
    }
    for(int i = 0; i < L; i++){
        int x = -1, y = -1; // This will get through the do while at least once.
        generateCoord(x, y);
        Barrel* barrel = new Barrel(this, player, x, y);
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
    
    /* adding things */
    int G = curLevel * 25 + 300;
    //1 in G chance a water or sonar will show up:
    int itemAppearanceProb = (int)((double) rand() / (RAND_MAX) * G);
    //std::cout<<itemAppearanceProb<<std::endl;
    if(itemAppearanceProb == 1){
        //1/5th of the time, a sonar should show up
        //if((int)((double) rand() / (RAND_MAX) * 5) == 1){
            Sonar* sonar = new Sonar(this, player);
            actor.push_back(sonar);
        //}
    }
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
    for(int i = DIRT_ROWS-1; i>=0; i--){
        for(int j = WORLD_X-1; j>=0; j--){
            dirt[i][j] = nullptr;
        }
    }
    for(int i = actor.size()-1; i>=0; i--){
        delete actor[i];
        actor.pop_back();
    }
    delete player;
}

void StudentWorld::removeDirt(int x, int y, int size){
    bool flag = false;
    for(int i = 0; i < WORLD_X; i++){
        for(int j = 0; j < DIRT_ROWS; j++){
            if(dirt[i][j] == nullptr) continue;
            if(inRange(dirt[i][j]->getX(), dirt[i][j]->getY(), x, y, size, size)){
                delete dirt[i][j];
                dirt[i][j] = nullptr;
                flag = true;
            }
        }
    }
    //play sound
    if(flag)
        playSound(SOUND_DIG);
}

void StudentWorld::revealSonar(int x, int y){
    for(vector<Actor*>::iterator it = actor.begin() ; it != actor.end();it++){
        Actor* a = *it;
        if(dist(a->getX(), a->getY(), x, y) < SONAR_RANGE){
            if(Item* i = dynamic_cast<Item*>(a)) {
                i->setDiscovered();
            }
        }
    }
    playSound(SOUND_SONAR);
}

vector<Protester*> StudentWorld::getProtesters(){
    vector<Protester*> protesters;
    for(vector<Actor*>::iterator it = actor.begin() ; it != actor.end();it++){
        if(Protester* v = dynamic_cast<Protester*>(*it)) {
            protesters.push_back(v);
        }
    }
    return protesters;
}

void StudentWorld::addActor(Actor* act){
    actor.push_back(act);
}

void squirt(int x, int y, Actor::Direction dir){
    
}


