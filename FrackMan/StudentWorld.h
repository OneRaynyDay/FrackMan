#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

/* PLEASE REFER TO PG 53 OF SPEC */

#include "GameWorld.h"
#include "GameConstants.h"
#include "Dirt.h"
#include "Boulder.h"
#include "FrackMan.h"
#include "Barrel.h"
#include "Nugget.h"
#include "Sonar.h"
#include "Pool.h"
#include "Squirt.h"
#include "Protester.h"
#include "HardcoreProtester.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
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
    static const int MIN_Y_SPAWN = 20;
    static const int DIR_X = 61;
    static const int DIR_Y = 61;
    static const int MAX_MANHATTAN = 150;
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
        numProtesters = 0;
        maxProtesters = 0;
        protesterAddTick = 0;
        curTick = 0;
        for(int i = 0; i < WORLD_X; i++){
            for(int j = 0; j < DIRT_ROWS; j++){
                dirt[i][j] = nullptr;
            }
        }
        for(int i = 0; i < DIR_X; i++){
            for(int j = 0; j < DIR_Y; j++){
                dirMap[i][j] = Actor::none;
                stepMap[i][j] = MAX_MANHATTAN;
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
    
    void updateText();
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
    void generateCoord(int& x, int& y, int xsize = 1, int ysize = 1);
    void populateWater(int &xf, int&yf);
    bool attackProtestersAt(int x, int y, int dist, int hitDecrease, int& state, bool onlyKillWeak = false);
    bool attackFrackManAt(int x, int y, int dist, int hitDecrease, bool directionMatters = false, Actor* detector = nullptr);
    bool attackHumansAt(int x, int y, int dist, int hitDecrease, vector<Actor*> list, int& state, bool onlyKillWeak = false);
    bool inTunnel(int x, int y, int xsize = 1, int ysize = 1);
    /*
     Protester navigation function
     */
    void findPath(int x, int y, Actor::Direction dir, int step, Actor * except=nullptr);
    void pathFindToTopRight(Actor * except=nullptr);
    Actor::Direction getTopRightDir(int x, int y){
        return dirMap[x][y];
    }
    bool canSeeProtester(int x, int y);
    bool straightPathWithFrackMan(int x, int y);
    Actor::Direction dirTowardsFrackMan(int x, int y);
    
    bool checkDiscoveredFrackMan(Actor* detector);
    bool checkDiscoveredProtester(Actor* detector);
    bool existsBlock(int x, int y, int xsize, int ysize, bool& dirtOrActor, bool include=true, Actor* except=nullptr);
    bool existsBoulder(int x, int y, bool include=true, Actor* except=nullptr);
    bool existsDirt(int x, int y, int xsize, int ysize, bool include=true, Actor* except=nullptr);
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
    struct dirCoord{
        dirCoord(){
            x = 0;
            y = 0;
            dir = Actor::none;
            dist = 150; //greater than the entire manhattan distance of the map.
        }
        dirCoord(int xe, int ye){
            x = xe;
            y = ye;
            dir = Actor::none;
            dist = 150; //greater than the entire manhattan distance of map
        }
        dirCoord(int xe, int ye, Actor::Direction dire){
            x = xe;
            y = ye;
            dir = dire;
            dist = 150;
        }
        int x;
        int y;
        int dist;
        Actor::Direction dir;
    };
    Actor::Direction dirMap[DIR_X][DIR_Y];
    int stepMap[DIR_X][DIR_Y];

    Dirt* dirt[WORLD_X][DIRT_ROWS];
    FrackMan* player;
    vector<Actor*> actor;
    /* MORE TO WRITE HERE ... */
    int x_size, y_size;
    int curLevel;
    int curBarrels;
    int numProtesters, maxProtesters, protesterAddTick, curTick;
};

#endif // STUDENTWORLD_H_
