#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

/* PLEASE REFER TO PG 53 OF SPEC */

#include "GameWorld.h"
#include "GameConstants.h"
#include "Dirt.h"
#include "FrackMan.h"
#include <vector>
#include <string>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class FrackMan;

class StudentWorld : public GameWorld
{
    static const int DIRT_ROWS = 60;
public:
    // Implement a constructor for this class that initializes all member
    // variables required for proper gameplay.
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
        /* MORE TO WRITE HERE ... */
	}
    // Implement a destructor for this class that frees any remaining
    // dynamically allocated data that has not yet been freed at the time
    // the class is destroyed (e.g., the FrackMan and all remaining Dirt).
    virtual ~StudentWorld(){
        cleanUp();
    }
    
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
    bool inRange(int tx, int ty, int x, int y, int xt, int yt){
        /* tx, ty : test x test y
         * x, y : start x start y
         * xt, yt : x thresh y thresh 
         */
        return (tx >= x && tx <= x + xt) && (ty >= y && ty <= y + yt);
    }
    void squirt(int x, int y, Actor::Direction dir);

private:
    /* Add any private member variables to this class required to keep
    track of all Dirt in the oil field as well as the FrackMan object.
    You may ignore all other items in the oil field such as Boulders,
    Barrels of oil, Protesters, Nuggets, etc. for part #1. */
    vector<Dirt*> dirt;
    FrackMan* player;
    vector<Actor*> actor;
    /* MORE TO WRITE HERE ... */
    
};

#endif // STUDENTWORLD_H_
