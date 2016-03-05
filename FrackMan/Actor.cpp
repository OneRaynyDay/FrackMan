#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Actor::moveDelta(StudentWorld* world, Direction dir, int& xdir, int& ydir, int steps){
    int tempx = xdir, tempy = ydir;
    switch(dir){
        case left:
            tempx-=steps;
            break;
        case right:
            tempx+=steps;
            break;
        case up:
            tempy+=steps;
            break;
        case down:
            tempy-=steps;
            break;
        default:
            return false;
    }
    if(world->inBound(tempx, tempy) && world->inBound(tempx+(getSize()*4), tempy+(getSize()*4))){
        xdir = tempx;
        ydir = tempy;
    }
    return true;
}


bool Human::changeState(Direction dir){
    if(dir != getDirection() && dir != none){
        setDirection(dir);
        return false;
    }
    int xdelta = getX(), ydelta = getY();
    if(moveDelta(getWorld(), dir, xdelta, ydelta)){
        moveTo(xdelta, ydelta);
        return true;
    }
    else{
        return false;
    }
}

/*---------- ACTUAL IMPLEMENTATIONS HERE ------------*/
/*---- DIRT -----*/
/*---- BOULDER -----*/

Boulder::~Boulder(){
}
void Boulder::doSomething(){
    /*Check to see if it’s still alive. If not, its doSomething() method should immediately return.
     2. If the Boulder is currently in the stable state, then it must check to see if there is any Dirt in the 4 squares immediately below it. For example, if the Boulder is at location x=20,y=30, it would check squares (20,29), (21,29), (22,29) and (23,29) for Dirt. If there is any Dirt below the Boulder, it does nothing – after all it’s a dumb rock. However, if none of the 4 squares beneath the Boulder have any Dirt, then the Boulder must transition into a waiting state:
     A. It must enter a waiting state for the next 30 ticks
     3. If the Boulder is in a waiting state and 30 ticks have elapsed, then it must
     transition into a falling state and play the sound SOUND_FALLING_ROCK.
     4. If the Boulder is in a falling state, then:
     A. It must continue to move downward one square during each tick until it either (a) hits the bottom of the oil field (i.e., it tries to move to y=-1), (b) runs into the top of another Boulder, or (c) it runs into Dirt (i.e., by moving down a square, the Boulder would overlap over one or more Dirt objects). When any of the above conditions are met the Boulder must set its state to dead so it can be removed from the game at the end of the current tick.
     B. If the Boulder comes within a radius of 3 (i.e. less than or equal to 3 squares, so 2.99 would count) of any Protester(s) or the FrackMan while falling, it must cause 100 points of annoyance to those actors (effectively completely annoying them in one fell swoop). The Boulder will continue falling down as described above, even if it annoys one or more Protesters on its way down (i.e., it won’t stop falling if it runs into a Protester). However, if the Boulder annoys the FrackMan, the player will instantly lose a life.
     */
    if(state == STABLE_STATE){
        bool dirtOrActor = false;
        
        getWorld()->existsBlock(getX(), getY()-1, 4, 1, dirtOrActor, false, this);
        if(!dirtOrActor){
            state = WAITING_STATE;
        }
    }
    else if(state == WAITING_STATE){
        hitpoint--;
        if(hitpoint == 0){
            state = FALLING_STATE;
            getWorld()->playSound(getSound());
        }
    }
    else if(state == FALLING_STATE){
        int x = getX(), y = getY();
        bool flag = false; // stubby
        moveDelta(getWorld(), getDirection(), x, y);
        if(getWorld()->existsBlock(x, y, 4, 1, flag, false, this)){
            x = getX();
            y = getY();
        }
        if(getX() == x && getY() == y){
            consume();
        }
        else{
            /*if(getWorld()->attackProtestersAt(x, y, 3, DECREASE_HP) || getWorld()->attackFrackManAt(x, y, 3, DECREASE_HP))
             std::cout << x << "\t" << y << "\t" << getPlayer()->getX() << "\t" << getPlayer()->getY() << "\t" << std::endl;*/
            int state = -1;
            if(getWorld()->attackProtestersAt(x, y, 3, DECREASE_HP, state, true)){
                getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            }
            if(getWorld()->attackFrackManAt(x, y, 3, DECREASE_HP)){
            }
            moveTo(x, y);
        }
        //must constantly update position
    }
}

/*---- BARREL -----*/

void Barrel::doSomething(){
    if(getWorld()->checkDiscoveredFrackMan(this)){
        getWorld()->decreaseBarrels();
    }
}

/*---- NUGGET -----*/

Nugget::~Nugget(){
    if(pickup){
        getPlayer()->increaseNugget(1);
    }
    else{
    }
}

void Nugget::doSomething(){
    if(pickup){
        getWorld()->checkDiscoveredFrackMan(this);
    }
    else{
        if(hitpoints == 0){
            consume();
            return;
        }
        hitpoints--;
        if(getWorld()->checkDiscoveredProtester(this)){
            int flag = -1;
            getWorld()->attackProtestersAt(getX(), getY(), 4, HIT_DECREASE, flag, false, true);
            consume();
        }
    }
}

/*---- SONAR -----*/

Sonar::Sonar(StudentWorld* w, FrackMan* f):Item(w, f, IID_SONAR, 0, DIRT_ROWS+1){
    setVisible(true);
    hitpoints = min(100, 300 - 10*w->getLevel());
}


Sonar::~Sonar(){
}

void Sonar::doSomething(){
    if(isDead())
        return;
    hitpoints--;
    if(hitpoints <= 0){
        consume();
    }
    if(getWorld()->checkDiscoveredFrackMan(this)){
        getPlayer()->increaseSonar(1);
    }
}

/*---- POOL -----*/

Pool::Pool(StudentWorld* w, FrackMan* f, int startX, int startY) : Item(w, f, IID_WATER_POOL, startX, startY){
    setVisible(true);
    hitpoints = min(100, 300 - 10*w->getLevel());
}

void Pool::doSomething(){
    if(isDead())
        return;
    hitpoints--;
    if(hitpoints <= 0){
        consume();
    }
    if(getWorld()->checkDiscoveredFrackMan(this)){
        getPlayer()->increaseWater(5);
    }
}

/*---- SQUIRT -----*/

Squirt::~Squirt(){
}

void Squirt::doSomething(){
    if(getWorld()->checkDiscoveredProtester(this)){
        int state = -1;
        if(getWorld()->attackProtestersAt(getX(), getY(), 4, 2, state, true)){
            hit = true;
            consume();
            if(state == 0)
                getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
            else if(state == 1)
                getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            return;
        }
    }
    if(hitpoints == 0){
        consume();
        return;
    }
    hitpoints--;
    // move in the specified direction 1 square forward
    int x = getX(), y = getY();
    moveDelta(getWorld(), getDirection(), x, y);
    changeState(x, y, getSize()*4);
}

void Squirt::changeState(int x, int y, int size){
    bool flag = false; // stub
    if(getWorld()->existsBlock(x, y, size, size, flag)){
        x = getX();
        y = getY();
    }
    if(getX() == x && getY() == y){
        consume();
    }
    else{
        moveTo(x, y);
    }
}

/*---- PROTESTER -----*/

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

void Protester::consume(int a){
    if(isDeadState())
        return;
    Human::consume(a);
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
        restTick += max(0, 3 - getWorld()->getLevel()/4);
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
    restTick += max(0, 3 - getWorld()->getLevel()/4);
}

/*---- HARDCOREPROTESTER -----*/

void HardcoreProtester::doSomething(){
    //The Hardcore Protester must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed.
    if(isDead())
        return;
    //If the Hardcore Protester is in a “rest state” during the current tick, it must do nothing other than update its resting tick count, and immediately return.
    if(getRestTick() > 0){
        setRestTick(getRestTick()-1);
        if(getShoutTick() > 0) // I only added this part because it seems like the game does this. 15 non-resting ticks is way too long in comparison.
            setShoutTick(getShoutTick()-1);
        return;
    }
    //Otherwise, if the Hardcore Protester is in a leave-the-oil-field state (because their hit points reached zero due to being repeatedly squirted by Squirt objects fired from the FrackMan’s squirt gun or bonked by a falling Boulder), then:
    if(isDeadState()){
        //a. If the Hardcore Protester is at its exit point (at location x=60, y=60) then it will immediately set its status to dead so that it will be removed from the game at the end of the current tick (by your StudentWorld class).
        //already taken care of ^
        
        //b. Otherwise, the Hardcore Protester must move one square closer to its exit point (at x=60, y=60). It must determine what direction to move based on a queue-based maze-searching algorithm like the one we learned in class. The Hardcore Protester may not walk through Dirt or Boulders to reach its exit point (meaning that none of the 16 squares that comprise the 4x4 Hardcore Protester icon may overlap with Dirt and that the Hardcore Protester must not come within (within means <=) a distance of 3.0 units of any Boulders). Hint: It’s inefficient for every single Hardcore Protester to try to figure out its own optimal path to their common exit point of 60,60, so see if you can figure out a way to compute a single data structure that enables all Protesters to quickly determine their optimal path to their common exit, no matter where they are located in the oil field.
        //c. After moving one step toward their exit point, the Hardcore Protester will return immediately. It will not perform any of the following steps.
        int x = getX(), y = getY();
        Direction dir = getWorld()->getTopRightDir(x, y);
        moveDelta(getWorld(), dir, x, y);
        changeState(dir);
        setRestTick( max(0, 3 - getWorld()->getLevel()/4));
        return;
    }
    /*Otherwise, the Hardcore Protester will check to see if they are within a distance of 4 units of the FrackMan, AND they are currently facing in the FrackMan’s
        46
        direction5. If both are true and the Hardcore Protester hasn’t shouted within the last non-resting 15 ticks, then the Hardcore Protester will:
        a. Play the shouting sound: SOUND_PROTESTER_YELL
        b. Inform the FrackMan that he’s been annoyed for a total of 2 annoyance points (deducting 2 points from the FrackMan’s hit points, and possibly
                                                                                     causing the FrackMan to become sufficiently annoyed to quit the level
                                                                                     (aka die)).
        c. Update some state variable in a manner that prevents this Hardcore
        Protester from shouting again for at least 15 non-resting ticks.
        d. Return immediately.*/
    if(getShoutTick() <= 0){
        if(getWorld()->attackFrackManAt(getX(), getY(), 4, DAMAGE, true, this)){
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            setShoutTick(SHOUT_WAIT);
            setRestTick(SHOUT_WAIT);
            return;
        }
    }
    //Otherwise, if the Hardcore Protester is more than 4.0 units away from the FrackMan, the Hardcore Protester will:
    //Compute a value M, such that M = 16 + current_level_number * 2
    int M = 16 + getWorld()->getLevel()*2;
    /*b. If the Hardcore Protester is less than or equal to a total of M legal
     horizontal or vertical moves away from the current location of the FrackMan (as can be determined using the same basic queue-based maze- searching algorithm described in Hardcore Protester item 3.b above), then the Hardcore Protester will sense the FrackMan’s cell phone signal and know where he is. This is true even if the Hardcore Protester has no direct line of sight to the FrackMan, so long as he can be reached in M moves without digging through Dirt or going through a Boulder. In such a situation, the Hardcore Protester will:*/
    if(getWorld()->manhattanDistFromPlayer(getX(), getY()) <= M){
        /*i. Determine which horizontal/vertical direction to move in (as dictated by its maze-searching algorithm) such that if it were to make such a move it would be one square closer to the FrackMan.
         ii. Change its current direction to face this new direction.
         iii. Move one square in this direction such that after the move it is one
         square closer to the FrackMan’s current location. iv. Return immediately.*/
        getWorld()->computePathToFrackMan(getX(), getY(), M);
        Direction dir = getWorld()->getFrackManDir(getX(), getY());
        if(dir != none){
            setDirection(dir);
            changeState(getDirection());
            setRestTick(max(0, 3 - getWorld()->getLevel()/4));
            return;
        }
    }
    if(getWorld()->canSeeProtester(getX(), getY())){
        int x = getX(), y = getY();
        setDirection(getWorld()->dirTowardsFrackMan(x, y));
        changeState(getDirection());
        setStep(0);
    }
    else{
        setStep(getStep()-1);
        if(getStep() > 0){
            if(getPerpTick() > 0 || !canTurn(getDirection())){
                setPerpTick(getPerpTick() - 1);
            }
            else{
                setDirection(randTurn(getDirection()));
                setPerpTick(INIT_PERP_TICK);
                setStep(determineRandomSteps());
            }
        }
        else{
            int x = getX(), y = getY();
            Actor::Direction dir = none;
            do{
                dir = pickRandomDirection();
                moveDelta(getWorld(), dir, x, y);
            }while(x == getX() && y == getY());
            setDirection(dir);
            setStep(determineRandomSteps());
        }
        int x = getX(), y = getY();
        if(!changeState(getDirection())){
            setStep(0);
        }
        else if(x == getX() && y == getY()){
            setStep(0);
        }
    }
    setRestTick(max(0, 3 - getWorld()->getLevel()/4));
}

/*---- FRACKMAN ----*/
bool FrackMan::changeState(Direction dir){
    if(dir != getDirection()){
        setDirection(dir);
        return false;
    }
    int xdelta = getX(), ydelta = getY();
    if(moveDelta(getWorld(), dir, xdelta, ydelta)){
        if(!getWorld()->existsBoulder(xdelta, ydelta)){
            moveTo(xdelta, ydelta);
            return true;
        }
    }
    return false;
}

void FrackMan::doSomething(){
    //The FrackMan must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed.
    if(isDead())
        return;
    /*If the FrackMan’s 4x4 image currently overlaps any Dirt objects within the oil field, then it will call upon the StudentWorld object to:
     a. Remove/destroy the Dirt objects from the 4x4 area occupied by the FrackMan (from x, y to x+3,y+3 inclusive)
     b. Make a digging sound by playing the SOUND_DIG sound (see the StudentWorld section of this document for details on how to play a sound).*/
    getWorld()->removeDirt(getX(), getY(), (int)getSize()*4);
    
    
    /*Otherwise, the doSomething() method must check to see if the human player pressed a key (the section below shows how to check if the user pressed a key, and if so, how to determine what key the user pressed).
     a. If the user presses the Escape key, this allows the user to abort the current level. In this case, the FrackMan object should set itself to completely annoyed (aka dead). The code in your StudentWorld class should detect that the FrackMan has died and address this appropriately (e.g., replay the level from scratch, or end the game if the player has run out of lives).
     b. If the user presses the spacebar key, then the FrackMan will fire a Squirt into the oil field, assuming they have sufficient water in their squirt gun to do so. The FrackMan will then reduce their water count by 1. To fire a Squirt, the player must create and add a new Squirt object into the oil field at a location that is four squares immediately in front of the FrackMan, facing the same direction as the FrackMan.
     If the player wants their FrackMan to turn to face a different direction then it is currently facing (by pressing one of the arrow keys), then the FrackMan’s direction should be adjusted to the indicated direction but the FrackMan must not move in that direction. For example, if the FrackMan is currently facing up, and the player hits the right arrow key, then the FrackMan should turn to face right, but not move right.*/
    int keyPressed = -1;
    if(getWorld()->getKey(keyPressed)){
        switch(keyPressed){
            case KEY_PRESS_ESCAPE:
                while(!isDead())
                    consume();
                break;
            case KEY_PRESS_SPACE:{
                if(water <= 0)
                    break;
                
                int x = getX(), y = getY();
                water--;
                moveDelta(getWorld(), getDirection(), x, y, 4);
                if(x == getX() && y == getY())
                    break;
                getWorld()->squirt(x, y, getDirection());
                break;
            }
            case KEY_PRESS_LEFT:
            case KEY_PRESS_RIGHT:
            case KEY_PRESS_UP:
            case KEY_PRESS_DOWN:{
                Direction dir = keyToDir(keyPressed);
                changeState(dir);
                break;
            }
            case KEY_PRESS_TAB:{
                if(nugget <= 0)
                    break;
                
                Nugget* n = new Nugget(getWorld(), this, getX(), getY(), true, false, SOUND_PROTESTER_FOUND_GOLD, 100);
                getWorld()->addActor(n);
                nugget--;
                break;
            }
            case 'Z':
            case 'z':{
                if(sonar <= 0)
                    break;
                
                getWorld()->revealSonar(getX(), getY());
                sonar--;
            }
        }
    }
};