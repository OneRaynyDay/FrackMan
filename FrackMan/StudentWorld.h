#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

/* PLEASE REFER TO PG 53 OF SPEC */

#include "GameWorld.h"
#include "GameConstants.h"
#include "Dirt.h"
#include "FrackMan.h"
#include "Barrel.h"
#include "Nugget.h"
#include "Sonar.h"
#include "Pool.h"
#include "Squirt.h"
#include "Protester.h"
#include <vector>
#include <string>
#include <cmath>
#include <random>


using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class FrackMan;

class StudentWorld : public GameWorld
{
    static const int DIRT_ROWS = 60;
    static const int WORLD_X = 64;
    static const int WORLD_Y = 64;
    static const int CHARACTER_SPACING = 6;
    static const int SONAR_RANGE = 12;
public:
    // Implement a constructor for this class that initializes all member
    // variables required for proper gameplay.
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
        x_size = WORLD_X;
        y_size = WORLD_Y;
        curLevel = 0;
        curBarrels = 0;
        for(int i = 0; i < WORLD_X; i++){
            for(int j = 0; j < DIRT_ROWS; j++){
                dirt[i][j] = nullptr;
            }
        }
        /* MORE TO WRITE HERE ... */
	}
    // Implement a destructor for this class that frees any remaining
    // dynamically allocated data that has not yet been freed at the time
    // the class is destroyed (e.g., the FrackMan and all remaining Dirt).
    virtual ~StudentWorld();
    /*
    Implement the init() method in this class. It must:
    1. Create the FrackMan object and insert it into the oil field at
    the right starting location (see the StudentWorld init()
    section of this document for details on the starting
    location).
    2. Creates all of the oil field’s Dirt objects and inserts them
    into a data structure that tracks active Dirt (see the
    StudentWorld init() section for details on where to place
    Dirt, and what data structure to use track all of the
    remaining Dirt in the game).
    */
    
    virtual int init();
	
    virtual int move();
	
    virtual void cleanUp();

    void removeDirt(int x, int y, int size);
    void revealSonar(int x, int y);
    
    //this is necessary for nuggets
    void addActor(Actor* act);
    
    bool inRange(int tx, int ty, int x, int y, int xt, int yt){
        /* 
         tx, ty : test x test y
         x, y : start x start y
         xt, yt : x thresh y thresh
         */
        return (tx >= x && tx < x + xt) && (ty >= y && ty < y + yt);
    }
    bool inBound(int x, int y){
        return (x >= 0 && x <= x_size) && (y >= 0 && y <= y_size);
    }
    void squirt(int x, int y, Actor::Direction dir);
    /*
     No distributed game object may be within a radius (Euclidian distance) of 6 squares of any other distributed game object
     */
    int getRandomNum(int range);
    bool sparseEnough(int x, int y);
    bool blank(int x, int y, int tx, int ty);
    void generateCoord(int& x, int& y);
    void populateWater(int &xf, int&yf);
    void attackProtestersAt(int x, int y, int size, int hitDecrease);
    
    bool checkDiscoveredFrackMan(Actor* detector);
    bool checkDiscoveredProtester(Actor* detector);
    bool existsBlock(int x, int y, int size);
    
    int dist(int x1, int y1, int x2, int y2){
        int d = (int)(pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5));
        //std::cout<< "x1 : " << x1 << " x2 : " << x2 << " y1 : " << y1 << " y2 : " << y2 << "d : " << d << std::endl;
        return d;
    }
    
    void decreaseBarrels(){
        curBarrels--;
    }
    /*Getters*/
    int getLevel(){return curLevel;}
private:
    /* Add any private member variables to this class required to keep
    track of all Dirt in the oil field as well as the FrackMan object.
    You may ignore all other items in the oil field such as Boulders,
    Barrels of oil, Protesters, Nuggets, etc. for part #1. */
    Dirt* dirt[WORLD_X][DIRT_ROWS];
    FrackMan* player;
    vector<Actor*> actor;
    /* MORE TO WRITE HERE ... */
    int x_size, y_size;
    int curLevel;
    int curBarrels;
};

#endif // STUDENTWORLD_H_
