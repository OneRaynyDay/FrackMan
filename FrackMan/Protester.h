//
//  Protester.h
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef Protester_h
#define Protester_h
#include "Actor.h"
#include "StudentWorld.h"

class Protester : public Human{
static const int INIT_HITPOINTS = 5;
static const int INIT_PERP_TICK = 200;
static const int DAMAGE = 20;
static const int SHOUT_WAIT = 15;
static const int MIN_STEP = 8;
static const int MAX_STEP = 60;
static const int EXIT_X = 60;
static const int EXIT_Y = 60;
public:
    Protester(StudentWorld* w, int startX, int startY, int ID, int hp = INIT_HITPOINTS);
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
    virtual void consume();
    virtual void setDead();
    virtual bool moveDelta(StudentWorld* world, Direction dir, int& xdir, int& ydir, int steps = 1);
private:
    int determineRandomSteps();
    bool canTurn(Actor::Direction dir);
    Actor::Direction randTurn(Actor::Direction dir);
    Actor::Direction oppositeDir(Actor::Direction dir);
    Actor::Direction numToDir(int num);
    int step;
    int restTick;
    int shoutTick;
    int perpTick;
};
#endif /* Protester_h */
