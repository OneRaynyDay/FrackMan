//
//  Protester.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/18/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "Protester.h"

/*
 1. Each Regular Protester must have an image ID of IID_PROTESTER.
 2. Each Regular Protester must start out facing left.
 3. Each Regular Protester must decide how many squares,
 numSquaresToMoveInCurrentDirection, it will decide to move left before possibly switching its direction. This value must be: 8 <= numSquaresToMoveInCurrentDirection <= 60
 4. Each Regular Protester starts out with 5 hit-points.
 5. Each Regular Protester starts out NOT in a leave-the-oil-field state.
 6. Each Regular Protester have the following graphic parameters:
 a. It has an image depth of 0 – they’re always in the foreground
 b. It has a size of 1.0
 */
Protester::Protester(StudentWorld* w, int startX, int startY, int ID, int hp):Human(hp, w, ID, startX, startY, left){
    step = 0;
    restTick = 0;
    shoutTick = 0;
    perpTick = 0;
}

int Protester::determineRandomSteps(){
    return getWorld()->getRandomNum(MAX_STEP-MIN_STEP) + MIN_STEP;
}

Protester::~Protester(){ /* do nothing */
}

Actor::Direction Protester::numToDir(int num){
    switch(num){
        case 0:
            return up;
        case 1:
            return down;
        case 2:
            return right;
        case 3:
            return left;
        default:
            return none;
    }
}

Actor::Direction Protester::oppositeDir(Actor::Direction dir){
    switch(dir){
        case up:
            return down;
        case down:
            return up;
        case right:
            return left;
        case left:
            return right;
        default:
            return none;
    }
}

Actor::Direction Protester::pickRandomDirection(){
    return numToDir(rand() % 4);
}

Actor::Direction Protester::randTurn(Actor::Direction dir){
    int x = getX(), y = getY(), r = rand() % 2;
    if(dir == up || dir == down)
        r += 2;
    
    moveDelta(getWorld(), numToDir(r), x, y);
    if(y == getY()){
        moveDelta(getWorld(), oppositeDir(numToDir(r)), x, y);
        if(y == getY()){
            return Actor::none;
        }
        else
            return oppositeDir(numToDir(r));
    }
    else{
        return numToDir(r);
    }
}

bool Protester::canTurn(Actor::Direction dir){
    int x = getX(), y = getY();
    if(dir == right || dir == left){
        moveDelta(getWorld(), up, x, y);
        if(y != getY())
            return true;
        moveDelta(getWorld(), down, x, y);
        if(y != getY())
            return true;
        return false;
    }
    else{
        moveDelta(getWorld(), left, x, y);
        if(x != getX())
            return true;
        moveDelta(getWorld(), right, x, y);
        if(x != getX())
            return true;
        return false;
    }
}

bool Protester::moveDelta(StudentWorld* world, Direction dir, int& xdir, int& ydir, int steps){
    int tempx = xdir, tempy = ydir;
    bool flag = true;
    if(Actor::moveDelta(world, dir, tempx, tempy, steps) && !getWorld()->existsBlock(tempx, tempy, 4, 4, flag)){
        xdir = tempx;
        ydir = tempy;
        return true;
    }
    return false;
}

bool Protester::changeState(Direction dir){
    if(dir == none){
        return false;
    }
    if(dir != getDirection()){
        setDirection(dir);
        return false;
    }
    int xdelta = getX(), ydelta = getY();
    if(moveDelta(getWorld(), dir, xdelta, ydelta)){
        bool flag = true;
        if(!getWorld()->existsBlock(xdelta, ydelta, 4, 4, flag)){
            moveTo(xdelta, ydelta);
            return true;
        }
    }
    return false;
}

void Protester::consume(){
    if(isDeadState())
        return;
    Human::consume();
    restTick = min(50, 100 - getWorld()->getLevel()* 10);
    if(isDeadState()){
        restTick = 0;
    }
}

void Protester::setDead(){
    Human::setDead();
    restTick = 0;
}


void Protester::doSomething(){
    /* 1. The Regular Protester must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed. */
    if(isDead())
        return;
    /* 2.If the Regular Protester is in a “rest state” during the current tick, it must do nothing other than update its resting tick count, and immediately return. */
    if(restTick > 0){
        restTick--;
        if(shoutTick > 0)
            shoutTick--;
        return;
    }

    /* 3. Otherwise, if the Regular Protester is in a leave-the-oil-field state (because their hit points reached zero due to being repeatedly squirted by the FrackMan’s squirt gun or bonked by a falling Boulder), then: */
    if(isDeadState()){
        /* a. If the Regular Protester is at its exit point (at location x=60, y=60) then it will immediately set its status to dead so that it will be removed from the game at the end of the current tick (by your StudentWorld class).*/
        /* b. Otherwise, the Regular Protester must move one square closer to its exit point (at x=60, y=60). It must determine what direction to move based on a queue-based maze-searching algorithm like the one we learned in class. The Regular Protester may not walk through Dirt or Boulders to reach its exit point (meaning that none of the 16 squares that comprise the 4x4 Regular Protester icon may overlap with a Dirt object and that the Regular Protester must not come within (within means <=) a distance of 3.0 units of any Boulders). Hint: It’s inefficient for every single Regular Protester to try to figure out its own optimal path to the exit, so see if you can figure out a way to compute a single data structure that enables all Protesterrs to quickly determine their optimal path to their common exit point of 60,60, no matter where they are located in the oil field.
            c. After moving one step toward their exit point, the Regular Protester will return immediately. It will not perform any of the following steps. */
        int x = getX(), y = getY();
        Direction dir = getWorld()->getTopRightDir(x, y);
        moveDelta(getWorld(), dir, x, y);
        changeState(dir);
        restTick += 4;
        return;
    }

    /* 4. Otherwise, the Regular Protester will check to see if they are within a distance of 4 units of the FrackMan, AND they are currently facing in the FrackMan’s direction2. If both are true and the Regular Protester hasn’t shouted within its last non-resting 15 ticks3, then the Regular Protester will:
        a. Play the shouting sound: SOUND_PROTESTER_YELL
        b. Inform the FrackMan that he’s been annoyed for a total of 2 annoyance points (deducting 2 points from the FrackMan’s hit points, and possibly
             causing the FrackMan to become sufficiently annoyed to quit the level
             (aka die)).
        c. Update some state variable in a manner that prevents this Regular
        Protester from shouting again for at least 15 non-resting ticks.
        d. Return immediately.
     */
    if(shoutTick <= 0){
        if(getWorld()->attackFrackManAt(getX(), getY(), 4, DAMAGE, true, this)){
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            shoutTick += SHOUT_WAIT;
            restTick += SHOUT_WAIT;
            return;
        }
        else{
            //was unsuccessful
        }
    }

    /*5. Otherwise, if the Regular Protester:
        a. Is in a straight horizontal or vertical line of sight to the FrackMan (even if
          the Regular Protester isn’t currently facing the Frackman), and
        b. Is more than 4 units away from the FrackMan – that is, the radius from the
        Regular Protester and the FrackMan is greater than 4.0 units away, and
        c. Could actually move the entire way to the FrackMan with no Dirt or
        Boulders blocking its path (assuming it kept walking straight over the
                                     next N turns) */
    if(getWorld()->canSeeProtester(getX(), getY())){
        /* Then the Regular Protester will:
         a. Change its direction to face in the direction of the FrackMan, AND then take one step toward him.
         b. The Regular Protester will set its numSquaresToMoveInCurrentDirection value to zero, forcing it to pick a new direction/distance to move during its next non-resting tick (unless of course, the Regular Protester still sees the FrackMan in its line of sight, in which case it will continue to move toward the FrackMan).
         c. Then the Regular Protester will immediately return. */
        int x = getX(), y = getY();
        setDirection(getWorld()->dirTowardsFrackMan(x, y));
        changeState(getDirection());
        step = 0;
    }
    else{
        /* 7. Otherwise, if the Regular Protester:
         a. Is sitting at an intersection where it could turn and move at least one square in a perpendicular direction from its currently facing direction (e.g., it is currently facing left, and is at a junction where it could turn and move one step either upward/downward without being blocked by Dirt or a Boulder), and
         b. The Regular Protester hasn’t made a perpendicular turn in the last 200 non-resting ticks. */
        
        /* Then the Regular Protester will:
         a. Determine which of the two perpendicular directions are viable (a viable
         direction is one that allows movement of at least one square without the
         Regular Protester being blocked by Dirt or a Boulder).
         b. Pick a viable perpendicular direction. If both perpendicular directions are
         viable, then pick one of the two choices randomly.
         c. Set its direction to the selected perpendicular direction.
         d. Pick a new value for numSquaresToMoveInCurrentDirection that will
         govern how far it should move in the selected perpendicular direction over the next numSquaresToMoveInCurrentDirection non-resting ticks. The value must be such that: 8 <= numSquaresToMoveInCurrentDirection <= 60.
         e. Remember that it made a perpendicular turn so that it doesn’t make another perpendicular turn for at least 200 more non-resting ticks.
         f. Continue on with the next step. */
        step--;
        if(step > 0){
            if(perpTick > 0 || !canTurn(getDirection())){
                perpTick--;
            }
            else{
                setDirection(randTurn(getDirection()));
                perpTick = INIT_PERP_TICK;
                step = determineRandomSteps();
            }
        }
        else{
        /* 6. Otherwise, the Regular Protester can’t directly see the FrackMan. As such, it will
         decrement its numSquaresToMoveInCurrentDirection variable by one. If the Regular Protester has finished walking numSquaresToMoveInCurrentDirection steps in its currently-selected direction (i.e., numSquaresToMoveInCurrentDirection <= 0), then: */
        /* a. The Regular Protester will pick a random new direction to move (up, down, left or right).
         b. If the random direction is blocked either by Dirt or a Boulder such that it can’t take even a single step in that chosen direction, then it will select a different direction and check it for blockage (it will continue checking directions until it has picked a direction that is not blocked). */
            int x = getX(), y = getY();
            Actor::Direction dir = none;
            do{
                dir = pickRandomDirection();
                moveDelta(getWorld(), dir, x, y);
            }while(x == getX() && y == getY());
            setDirection(dir);
            step = determineRandomSteps();
        }
        int x = getX(), y = getY();
        if(!changeState(getDirection())){
            step = 0;
        }
        else if(x == getX() && y == getY()){
            step = 0;
        }
    }
    restTick += 4;
}
