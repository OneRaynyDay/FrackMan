#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <cmath>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/* PLEASE REFER TO PG 52 OF SPEC */

//It must be derived from our GraphObject class

// It (or its base class) must make itself visible via a call to setVisible(true); (already done via inheritance)

class StudentWorld;

class Actor : public GraphObject{
public:
    // It must have a simple constructor and destructor
    // These 3 parameters are needed because of the base class
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject(imageID, startX, startY, dir, size, depth){
        // It (or its base class) must make itself visible via a call to setVisible(true);
    }
    virtual ~Actor(){};
    virtual void consume(int a = 1) = 0;
    virtual bool isDead() const = 0;
    virtual int getHitpoints(){ return 0; }
    virtual void setDead() = 0;
    virtual bool isStrong() = 0;
    // It must have a single virtual method called doSomething() that can
    // be called by the World to get one of the game’s actors to do
    // something.
    // This is pure virtual function
    virtual void doSomething() = 0;
    virtual int getSound() = 0;
    virtual int getPoints() = 0;
    virtual bool isBlock(){ return false; }
    virtual void setDiscovered(){ /* do nothing */ }
    virtual bool isDiscovered() const{ return true; }
    virtual bool isHuman() const{ return false; }
    virtual bool moveDelta(StudentWorld* world, Direction dir, int& xdir, int& ydir, int steps = 1);

private:
    // You may add other public/private methods and private member
    // variables to this base class, as you see fit.
};


/* A human is a class that represents:
 FrackMan
 AngryProtester
 HardcoreAngryProtestor
 with the following properties:
 -can move
 -can change directions
 -has size of 1
 -can pick up items
 */

class Human : public Actor{
public:
    Human(int hp, StudentWorld* w, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):Actor(imageID, startX, startY, dir, size, depth){
        world = w;
        // It (or its base class) must make itself visible via a call to setVisible(true);
        setVisible(true);
        hitpoints = hp;
    }
    virtual ~Human(){};
    virtual void doSomething() = 0;
    virtual int getSound(){ return SOUND_NONE; }
    virtual int getPoints(){ return 0; }
    int getHitpoints(){ return hitpoints; }
    virtual bool changeState(Direction dir);
    virtual bool isStrong(){
        return false; //can be bribed
    }
    StudentWorld* getWorld(){
        return world;
    }
    virtual void consume(int a = 1){
        if(hitpoints > 0)
            hitpoints -= a;
    }
    virtual void setDead(){
        hitpoints = 0;
    }
    virtual bool isDead() const{
        return hitpoints <= 0;
    }
    virtual bool isHuman() const { return true; }
private:
    int hitpoints;
    StudentWorld* world;
};

/* An item is a class that represents:
 Water
 Sonar
 Gold Nugget
 Barrels
 And it has the following properties:
 -does not move
 -can be picked up by humans
 -has size 1
 -has depth 2
 -randomly initialized startX and startY's
 */
class FrackMan;

class Item : public Actor{
public:
    Item(StudentWorld* w, FrackMan* f, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 2):Actor(imageID, startX, startY, dir, size, depth){
        world = w;
        player = f;
        setVisible(false);
        discovered = false;
        dead = false;
    }
    virtual ~Item(){};
    virtual void doSomething() = 0;
    virtual int getPoints() = 0;
    virtual int getSound(){ return SOUND_GOT_GOODIE; }

    virtual void setDiscovered(){
        setVisible(true);
        discovered = true;
    }
    virtual bool isDiscovered() const{
        return discovered;
    }
    /*returns true if an object is acquired*/
    FrackMan* getPlayer(){
        return player;
    }
    StudentWorld* getWorld(){
        return world;
    }
    virtual void setDead(){
        consume();
    }
    virtual void consume(int a = 1){
        dead = true;
    }
    virtual bool isDead() const{
        return dead;
    }
    virtual bool isStrong(){
        return true; //can't be bribed
    }
private:
    FrackMan* player;
    StudentWorld* world;
    bool discovered;
    bool dead;
};

/*---------- ACTUAL IMPLEMENTATIONS HERE ------------*/
/*---- DIRT -----*/
class Dirt : public Actor{
    static constexpr double DIRT_SIZE = 0.25;
    static const int DIRT_DEPTH = 3;
public:
    // It must have a simple constructor and destructor that initialize a
    // new Dirt object.
    Dirt(int startX, int startY) : Actor(IID_DIRT, startX, startY, right, DIRT_SIZE, DIRT_DEPTH){
        setVisible(true);
    };
    virtual ~Dirt(){};
    virtual bool isBlock(){ return true; }
    virtual bool isStrong(){
        return true; //can't be bribed
    }
    /* We don't need these. We have a separate vector of Dirt */
    virtual void consume(int a = 1){};
    virtual int getSound(){ return SOUND_NONE; }
    virtual int getPoints(){ return 0; }
    virtual void setDead(){};
    virtual bool isDead() const {return false;};
    virtual void doSomething(){}; // doesn't need to do anything
private:
    // You may add any set of public/private methods and private
    // member variables to your Dirt class as you see fit, so long as you
    // use good object oriented programming style (e.g., you must not duplicate functionality across classes)
};
/*---- BOULDER -----*/

class Boulder : public Item{
    static const int STABLE_STATE = 0;
    static const int WAITING_STATE = 1;
    static const int FALLING_STATE = 2;
    static const int SCORE_INCREASE = 500;
    static const int WAITING_HP = 30;
    static const int DECREASE_HP = 100;
public:
    Boulder(StudentWorld* w, FrackMan* f, int startX, int startY): Item(w, f, IID_BOULDER, startX, startY, down, 1.0, 1){
        /*
         1. The Boulder object must have an image ID of IID_BOULDER.
         2. A Boulder object must have its x,y location specified for it – the StudentWorld
         class can pass in this x,y location when constructing a new Boulder object.
         3. Boulders start out in a stable state (more on this below).
         4. Boulders start out facing downward.
         5. Boulders have the following graphic parameters:
         a. They have an image depth of 1 – behind actors like Protesters, but above Dirt
         b. They have a size of 1.0
         */
        setVisible(true);
        state = STABLE_STATE;
        hitpoint = WAITING_HP;
    }
    virtual ~Boulder();
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
    virtual int getSound(){ return SOUND_FALLING_ROCK; }
    virtual bool isBlock(){ return true; }
    bool getState(){ return state; }
private:
    int state;
    int hitpoint;
};

/*---- BARREL -----*/

class Barrel : public Item{
    static const int SCORE_INCREASE = 1000;
public:
    /*
     All Barrels must have an image ID of IID_BARREL.
     2. All Barrels must have their x,y location specified for them when they are created.
     3. All Barrels must start off facing rightward.
     4. All Barrels start out invisible – they must be discovered (by the FrackMan
     walking in close proximity to them) before they become visible.
     5. Barrels have the following graphic parameters:
     a. They have an image depth of 2 – behind actors like Protesters, but above Dirt
     b. They have a size of 1.0
     */
    Barrel(StudentWorld* w, FrackMan* f, int startX, int startY):Item(w, f, IID_BARREL, startX, startY){
    }
    virtual ~Barrel(){};
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
    virtual int getSound(){ return SOUND_FOUND_OIL; }
private:
    
};

/*---- NUGGET -----*/


class Nugget : public Item{
    static const int SCORE_INCREASE = 10;
    static const int HIT_DECREASE = 6;
public:
    /*1. All Gold Nuggets must have an image ID of IID_GOLD.
     2. All Gold Nuggets must have their x,y location specified for them when they are
     created.
     3. All Gold Nuggets must start off facing rightward.
     4. A Gold Nugget may either start out invisible or visible – this must be specified by
     the code that creates the Nugget, depending on the context of its creation. Nuggets buried within the Dirt of the oil field always start out invisible, whereas Nuggets dropped by the FrackMan start out visible.
     5. A Gold Nugget will either be pickup-able by the FrackMan or pickup-able by Protesters, but not both. This state must be specified by the code that creates the Gold Nugget object.
     6. A Gold Nugget will either start out in a permanent state (where they will remain in the oil field until they are picked up by the FrackMan or the level ends) or a temporary state (where they will only remain in the oil field for a limited number of ticks before disappearing or being picked up by a Protester). This state must be specified by the code that creates the Gold Nugget object.
     7. Gold Nuggets have the following graphic parameters:
     a. They have an image depth of 2 – behind actors like Protesters, but above
     Dirt
     b. They have a size of 1.0*/
    Nugget(StudentWorld* w, FrackMan* f, int startX, int startY, bool vis = false, bool pickUpFrack = true, int autoSound = SOUND_GOT_GOODIE, int hp = -1):Item(w, f, IID_GOLD, startX, startY){
        setVisible(vis);
        visible = vis;
        pickup = pickUpFrack;
        hitpoints = hp;
        sound = autoSound;
    }
    virtual ~Nugget();
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
    virtual int getSound(){ return sound; }
private:
    bool visible;
    bool pickup; // true means pick up by player, false means pick up by protester
    int sound;
    int hitpoints;
};

/*---- SONAR -----*/

class Sonar : public Item{
    /* When it is first created:
     1. All Sonar Kits must have an image ID of IID_SONAR.
     2. All Sonar Kits must have their x,y location specified for them when they are
     created.
     3. All Sonar Kits must start off facing rightward.
     4. All Sonar Kits starts out visible.
     5. A Sonar Kit is only pickup-able by the FrackMan.
     6. A Sonar Kit will always start out in a temporary state (where they will only
     remain in the oil field for a limited number of ticks before disappearing) – the number of ticks T a Sonar Kit will exist can be determined from the following formula:
     T = min(100, 300 – 10*current_level_number)
     7. Sonar Kits have the following graphic parameters:
     a. They have an image depth of 2 – behind actors like Protesters, but above
     Dirt
     b. They have a size of 1.0
     In addition to any other initialization that you decide to do in your Sonar Kit class, a Sonar Kit object must set itself to be visible using the GraphObject class’s setVisible() method, e.g.:
     setVisible(true); */
    static const int INIT_HITPOINTS = 100;
    static const int DIRT_ROWS = 60;
    static const int SCORE_INCREASE = 75;
public:
    Sonar(StudentWorld* w, FrackMan* f);
    virtual ~Sonar();
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
private:
    int hitpoints;
};

/*---- POOL -----*/

class Pool : public Item{
    static const int SCORE_INCREASE = 100;
public:
    static const int DEFAULT_SIZE = 1;
    Pool(StudentWorld* w, FrackMan* f, int startX, int startY);
    virtual ~Pool(){};
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
private:
    int hitpoints;
};

/*---- SQUIRT -----*/

class Squirt : public Item{
    /*
     1. The Squirt object must have an image ID of IID_SQUIRT.
     2. All Squirt objects must have their x,y location specified for them by the
     FrackMan.
     3. All Squirt objects must have their direction specified for them by the FrackMan.
     4. All Squirt objects start off with an initial travel distance of 4 squares
     5. All Squirt objects have the following graphic parameters:
     a. They have an image depth of 1 – behind actors like Protesters, but above Dirt
     b. They have a size of 1.0
     */
    static const int SQUIRT_DAMAGE = 2;
    static const int SQUIRT_HEALTH = 4;
public:
    Squirt(StudentWorld* w, FrackMan* f, Direction dir, int startX, int startY) : Item(w, f, IID_WATER_SPURT, startX, startY, dir, 1.0, 1){
        setVisible(true);
        hit = false;
        hitpoints = SQUIRT_HEALTH;
    }
    virtual ~Squirt();
    virtual void doSomething();
    virtual int getPoints(){ return 0; }
    virtual int getSound(){ return SOUND_NONE; }
    bool hitSomeone(){ return hit; }
    void changeState(int x, int y, int size);
private:
    int hitpoints;
    bool hit;
};

/*---- PROTESTER -----*/

class Protester : public Human{
    static const int MIN_STEP = 8;
    static const int MAX_STEP = 60;
    static const int EXIT_X = 60;
    static const int EXIT_Y = 60;
public:
    static const int DAMAGE = 20;
    static const int INIT_HITPOINTS = 5;
    static const int SHOUT_WAIT = 25;
    static const int INIT_PERP_TICK = 200;

    Protester(StudentWorld* w, int startX, int startY, int ID, int hp);
    virtual ~Protester();
    virtual void doSomething();
    Direction pickRandomDirection();
    virtual bool changeState(Direction dir);
    virtual bool isDead() const{
        return Human::isDead() && getX() == 60 && getY() == 60;
    }
    virtual bool isDeadState() const{
        return Human::isDead();
    }
    virtual void consume(int a = 1);
    virtual void setDead();
    virtual bool moveDelta(StudentWorld* world, Direction dir, int& xdir, int& ydir, int steps = 1);
    int determineRandomSteps();
    bool canTurn(Actor::Direction dir);
    Actor::Direction randTurn(Actor::Direction dir);
    Actor::Direction oppositeDir(Actor::Direction dir);
    Actor::Direction numToDir(int num);
    int getStep(){ return step; }
    int getRestTick(){ return restTick; }
    int getShoutTick(){ return shoutTick; }
    int getPerpTick(){ return perpTick; }
    void setStep(int num){ step = num; }
    void setRestTick(int num){ restTick = num; }
    void setShoutTick(int num){ shoutTick = num; }
    void setPerpTick(int num){ perpTick = num; }
private:
    int step;
    int restTick;
    int shoutTick;
    int perpTick;
};

/*---- HARDCOREPROTESTER -----*/

class HardcoreProtester : public Protester{
public:
    HardcoreProtester(StudentWorld* w, int x, int y) : Protester(w, x, y, IID_HARD_CORE_PROTESTER, 20){};
    virtual void doSomething();
    virtual bool isStrong(){
        return true;
    }
private:
};

/*---- FRACKMAN ----*/

using namespace std;

class StudentWorld;
/* The fact that it depends on Actor is TENTATIVE. we might change this! */
// It (or its base class) must make itself visible via a call to setVisible(true);
class FrackMan : public Human{
    
    static const int START_Y = 60;
    static const int START_X = 30;
    static const int GOLD_TICKS = 100;
    static const int INIT_HITPOINT = 100;
    static const int INIT_WATER = 20;
    static const int INIT_SONAR = 1;
    static const int INIT_NUGGET = 0;
    
public:
    // It must have a simple constructor and destructor that
    // initializes/deinitializes the FrackMan – see the FrackMan section
    // for more details on where to initially place the FrackMan on the screen.
    
    // from pg 28: The FrackMan must always start at location x=30, y=60
    FrackMan(StudentWorld* w) : Human(INIT_HITPOINT, w, IID_PLAYER, START_X, START_Y){
        /* The FrackMan, in its default state:
         a. Is given 10 hit points
         b. Is given 5 units of water to squirt with his squirt gun (he may pick up
         additional Water in the oil field)
         c. Is given 1 sonar charge
         d. Starts out with zero gold nuggets
         e. Should start facing rightward*/
        water = INIT_WATER;
        sonar = INIT_SONAR;
        nugget = INIT_NUGGET;
    };
    virtual ~FrackMan(){};
    
    /* It must have a limited version of a doSomething() method that lets
     the user pick a direction by hitting a directional key. If the player
     hits a directional key during the current tick, it updates the
     FrackMan’s location to the target square, removing any Dirt
     objects that overlap with the FrackMan’s 4x4 graphic image. To
     move the FrackMan, all this doSomething() method has to do is
     properly adjust the player’s X,Y coordinates using GraphObject’s
     moveTo() method and our graphics system will automatically
     animate its movement it around the oil field!
     */
    virtual void doSomething();
    
    Direction keyToDir(int keyPressed){
        switch(keyPressed){
            case KEY_PRESS_LEFT:
                return left;
                break;
            case KEY_PRESS_RIGHT:
                return right;
                break;
            case KEY_PRESS_UP:
                return up;
                break;
            case KEY_PRESS_DOWN:
                return down;
                break;
        }
        return none;
    }
    /* getters */
    int getWater(){
        return water;
    }
    int getSonar(){
        return sonar;
    }
    int getNugget(){
        return nugget;
    }
    /* setters */
    void increaseNugget(int amt){
        nugget += amt;
    }
    void increaseSonar(int amt){
        sonar += amt;
    }
    void increaseWater(int amt){
        water += amt;
    }
    virtual bool changeState(Direction dir);
    
private:
    // You may add any public/private methods and private member
    // variables to your player class as you see fit, so long as you use
    // good object oriented programming style (e.g., you must not duplicate functionality across classes).
    int water;
    int sonar;
    int nugget;
};

#endif // ACTOR_H_
