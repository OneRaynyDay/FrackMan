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
    virtual void consume() = 0;
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
    virtual void consume(){
        if(hitpoints > 0)
            hitpoints --;
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
    virtual void consume(){
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
#endif // ACTOR_H_
