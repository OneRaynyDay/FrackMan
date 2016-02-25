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

bool StudentWorld::inTunnel(int x, int y, int xsize, int ysize){
    for(int i = x; i < x+xsize; i++){
        for(int j = y; j < y+ysize; j++){
            if((i >= 30 && i <= 33) && (j >= 4))
                return true;
        }
    }
    return false;
}

bool StudentWorld::blank(int x, int y, int threshx, int threshy){
    for(int i = 0; i < threshx; i++){
        for(int j = 0; j < threshy; j++){
            if(dirt[x+i][y+j] != nullptr)
                return false;
        }
    }
    return true;
}

int StudentWorld::getRandomNum(int range){
    return (int)(rand() % range);
}

void StudentWorld::pathFindToTopRight(Actor * except){
    //initially we have just the beginning of the path(vent down)
    for(int i = 0; i < DIR_X; i++){
        for(int j = 0; j < DIR_Y; j++){
            dirMap[i][j] = Actor::none;
            stepMap[i][j] = MAX_MANHATTAN;
        }
    }
    findPath(60,60, Actor::right, 0);
}

void StudentWorld::findPath(int x, int y, Actor::Direction dir, int step, Actor* except){
    bool flag = true;
    if(((y < DIR_Y && x < DIR_X) && inBound(x, y)) && (y == DIRT_ROWS || !existsBlock(x, y, 4, 4, flag, except)) && step < stepMap[x][y]){
        stepMap[x][y] = step;
        dirMap[x][y] = dir;	// array of directions
    }
    else
        return;
    findPath(x-1, y, Actor::right, step++, except);
    findPath(x, y-1, Actor::up, step++, except);
    findPath(x, y+1, Actor::down, step++, except);
    findPath(x+1, y, Actor::left, step++, except);
}


void StudentWorld::generateCoord(int &xf, int &yf, int xsize, int ysize){
    int x = -1, y = -1;
    do{
        x = getRandomNum(WORLD_X-xsize);
        y = getRandomNum(DIRT_ROWS-ysize-MIN_Y_SPAWN)+MIN_Y_SPAWN;
    }while(!sparseEnough(x, y) || inTunnel(x, y, xsize, ysize));
    xf = x;
    yf = y;
}

void StudentWorld::populateWater(int &xf, int&yf){
    int x = -1, y = -1;
    do{
        x = getRandomNum(WORLD_X-4);
        y = getRandomNum(DIRT_ROWS-4);
    }while(!blank(x, y, Pool::DEFAULT_SIZE*4, Pool::DEFAULT_SIZE*4));
    xf = x;
    yf = y;
}

int StudentWorld::init(){
    player = new FrackMan(this);
    for(int i = 0; i < WORLD_X; i++){
        for(int j = 0; j < DIRT_ROWS; j++){
            if(inTunnel(i,j))
                continue;
            dirt[i][j] = new Dirt(i, j);
        }
    }
    
    // Take care of level stats
    // boulders
    int B = min(curLevel / 2 + 2, 6);
    // gold nuggets
    int G = max(5-curLevel / 2, 2);
    // barrels
    int L = min(2 + curLevel, 20);
    curBarrels = L;
    /*load the barrels*/
    
    for(int i = 0; i < B; i++){
        int x = -1, y = -1; // This will get through the do while at least once.
        generateCoord(x, y, 4, 4);
        for(int i = x; i < x+4; i++){
            for(int j = y; j < y+4; j++){
                if(dirt[i][j] != nullptr){
                    delete dirt[i][j];
                    dirt[i][j] = nullptr;
                }
            }
        }
        Boulder* boulder = new Boulder(this, player, x, y);
        //remove all the dirt around it!
        actor.push_back(boulder);
    }
    /*
    for(int i = 0; i < 1; i++){
        int x = 58, y = 48; // This will get through the do while at least once.
        //generateCoord(x, y, 4, 4);
        for(int i = x; i < x+4; i++){
            for(int j = y; j < y+4; j++){
                if(dirt[i][j] != nullptr){
                    delete dirt[i][j];
                    dirt[i][j] = nullptr;
                }
            }
        }
        cout<<x<<y<<endl;
        Boulder* boulder = new Boulder(this, player, x, y);
        //remove all the dirt around it!
        actor.push_back(boulder);
        
    }
    */
    for(int i = 0; i < G; i++){
        int x = -1, y = -1; // This will get through the do while at least once.
        generateCoord(x, y, 4, 4);
        Nugget* nugget = new Nugget(this, player, x, y);
        actor.push_back(nugget);
    }
    for(int i = 0; i < L; i++){
        int x = -1, y = -1; // This will get through the do while at least once.
        generateCoord(x, y, 4, 4);
        Barrel* barrel = new Barrel(this, player, x, y);
        actor.push_back(barrel);
    }
    protesterAddTick = max(25, 200 - curLevel);
    curTick += protesterAddTick;
    maxProtesters = min(15, (int)(2 + curLevel * 1.5));
    
    Protester* protester = new Protester(this, 60, 60, IID_PROTESTER, Protester::INIT_HITPOINTS);
    actor.push_back(protester);
    numProtesters++;
    
    //load it for initial kills
    pathFindToTopRight();

    return GWSTATUS_CONTINUE_GAME;
}

string formatDisplay(string front, int end, int width = 2, char fill = ' '){
    stringstream ss;
    ss << front << setw(width) << setfill(fill) << end;
    return ss.str();
}

void StudentWorld::updateText(){
    int water = player->getWater();
    int sonar = player->getSonar();
    int nugget = player->getNugget();
    int hitpoints = player->getHitpoints();
    // format -> Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2
    string text = formatDisplay("Scr: ", getScore(), 8, '0') + formatDisplay(" Lvl: ", getLevel(), 2) + formatDisplay(" Lives: ", getLives(), 1) + formatDisplay(" Hlth: ", hitpoints, 3) + formatDisplay("% Water: ", water) + formatDisplay(" Gld: ", nugget) + formatDisplay(" Sonar: ", sonar) + formatDisplay(" Oil left: ", curBarrels);
    setGameStatText(text);
}

int StudentWorld::move(){
    /* it must ask your FrackMan object to do something. Your
     move() method need not check to see if the FrackMan has died or
     not; you may assume at this point that the FrackMan cannot die.
     Nor need your move() method deal with any Protesters or other
     actors (e.g., Nuggets or Boulders) at this point – just the
     FrackMan.
     */
    updateText();
    pathFindToTopRight();
    if(curBarrels == 0){
        curLevel++;
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if(player->isDead()){
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    player->doSomething();
    //        decLives();
    
    /* adding things */
    int G = curLevel * 25 + 300;
    //1 in G chance a water or sonar will show up:
    int itemAppearanceProb = (int)((double) rand() / (RAND_MAX) * G);
    //std::cout<<itemAppearanceProb<<std::endl;
    if(itemAppearanceProb == 1){
        //1/5th of the time, a sonar should show up
        if(getRandomNum(5) == 1){
            Sonar* sonar = new Sonar(this, player);
            actor.push_back(sonar);
        }
        else{
            int x = 0, y = 0;
            populateWater(x, y);
            Pool* pool = new Pool(this, player, x, y);
            actor.push_back(pool);
        }
    }
    
    if(curTick <= 0 && numProtesters != maxProtesters){ // add a protester
        int probabilityOfHardcore = min(90, curLevel * 10 + 30);
        if(rand() % 100 > probabilityOfHardcore){
            Protester* protester = new Protester(this, 60, 60, IID_PROTESTER, Protester::INIT_HITPOINTS);
            actor.push_back(protester);
        }
        else{
            HardcoreProtester* protester = new HardcoreProtester(this, 60, 60);
        }
        curTick += protesterAddTick;
        
        
        numProtesters++;
    }
    curTick--;
    
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
    if(flag){
        playSound(SOUND_DIG);
        //update coordinate position
        pathFindToTopRight();
    }
}

bool StudentWorld::existsBoulder(int x, int y, bool include, Actor* except){
    for(vector<Actor*>::iterator it = actor.begin() ; it != actor.end();it++){
        if(!include && (*it) == except)
            continue;
        if((*it)->isBlock() && dist(x, y, (*it)->getX(), (*it)->getY()) <= 3){
            return true;
        }
    }
    return false;
}

bool StudentWorld::existsBlock(int x, int y, int xsize, int ysize, bool& dirtOrActor, bool include, Actor* except){
    //first loop through dirt blocks:
    if(existsDirt(x, y, xsize, ysize, include, except)){
        dirtOrActor = true;
        return true;
    }
    if(existsBoulder(x, y, include, except)){
        dirtOrActor = false;
        return true;
    }
    //then loop through actors for boulders
    return false;
}

bool StudentWorld::existsDirt(int x, int y, int xsize, int ysize, bool include, Actor* except){
    for(int i = x; i >= 0 && i < x+xsize && i < WORLD_X; i++){
        for(int j = y; j >= 0 && j < y+ysize && j < DIRT_ROWS; j++){
            if(!include && dirt[i][j] == except)
                continue;
            if(dirt[i][j] != nullptr && dirt[i][j]->isBlock()){
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::revealSonar(int x, int y){
    for(vector<Actor*>::iterator it = actor.begin() ; it != actor.end();it++){
        Actor* a = *it;
        if(dist(a->getX(), a->getY(), x, y) <= SONAR_RANGE){
            a->setDiscovered();
        }
    }
    playSound(SOUND_SONAR);
}

void StudentWorld::addActor(Actor* act){
    actor.push_back(act);
}

bool StudentWorld::checkDiscoveredFrackMan(Actor* detector){
    /*
     FrackOrProtester = true if frack, false if protester
     returns true only if a player got it.
     */
    
    int d = dist(player->getX(), player->getY(), detector->getX(), detector->getY());
    if(!detector->isDiscovered()){
        if(d <= 4){
            detector->setDiscovered();
            return false;
        }
    }
    if(d <= 3){
        detector->consume();
        playSound(detector->getSound());
        increaseScore(detector->getPoints());
        return true;
    }
    return false;
}
bool StudentWorld::checkDiscoveredProtester(Actor* detector){
    /* all objects are already visible when protesters pick them up */
    bool flag = false;
    for(int i = 0; i < actor.size(); i++){
        if(!actor[i]->isHuman()) continue;

        int d = dist(actor[i]->getX(), actor[i]->getY(), detector->getX(), detector->getY());
        if(d <= 3){
            playSound(detector->getSound());
            flag = true;
        }
    }
    return flag;
}
bool StudentWorld::attackProtestersAt(int x, int y, int d, int hitDecrease, int& state, bool onlyAttackWeak){
//    vector<Actor*> actors;
//    std::cout<<"-----" << std::endl;
//
//    for(int i = 0; i < actor.size(); i++){
//            actors.push_back(actor[i]);
//            std::cout<<"found protester at " << actor[i]->getX() << " ... " << actor[i]->getY() << std::endl;
//        }
//    }
    return attackHumansAt(x, y, d, hitDecrease, actor, state, onlyAttackWeak);
}
bool StudentWorld::attackFrackManAt(int x, int y, int d, int hitDecrease, bool directionMatters, Actor* detector){
    /* The state is 0 for did not kill anyone, 1 for killed someone */
    vector<Actor*> l;
    int state = -1;
    l.push_back(player);
    if(directionMatters){
        //decide beforehand to whether do anything:
        Actor::Direction neededDir = dirTowardsFrackMan(detector->getX(), detector->getY());
        if(dirTowardsFrackMan(detector->getX(), detector->getY()) != detector->getDirection())
            detector->setDirection(neededDir);
    }
    return attackHumansAt(x, y, d, hitDecrease, l, state);
}

Actor::Direction StudentWorld::dirTowardsFrackMan(int x, int y){
    int playerX = player->getX(), playerY = player->getY();
    if(playerY == y){
        if(playerX < x)
            return Actor::left;
        else
            return Actor::right;
    }
    else if(playerX == x){//same on the x coordinate
        if(playerY < y)//then player's below
            return Actor::down;
        else
            return Actor::up;
    }
    else
        return Actor::none;
    
}

bool StudentWorld::canSeeProtester(int x, int y){
    //flush out the dirt before this happens
    removeDirt(player->getX(), player->getY(), 4);
    if(!straightPathWithFrackMan(x,y))
        return false;

    int startx = min(x, player->getX()), endx = max(x, player->getX());
    int starty = min(y, player->getY()), endy = max(y, player->getY());
    bool flag = true;
    if(dist(startx, starty, endx, endy) < 4){
        //you're technically looking INTO frackman so I guess return true
        return true;
    }
    for(int i = startx; i < endx; i++){
        if(existsBlock(i, starty, 4,4, flag))
            return false;
    }
    for(int i = starty; i < endy; i++){
        if(existsBlock(startx, i, 4,4, flag))
            return false;
    }
    return true;
}

bool StudentWorld::straightPathWithFrackMan(int x, int y){
    return x == player->getX() || y == player->getY();
}

bool StudentWorld::attackHumansAt(int x, int y, int d, int hitDecrease, vector<Actor*> list, int& state, bool onlyKillWeak){
    for(int i = 0; i < list.size(); i++){
        if(!list[i]->isHuman()) continue;
        if(list[i]->getHitpoints() <= 0){
            continue;
        }
        else if(dist(list[i]->getX(), list[i]->getY(), x, y) <= d && (!list[i]->isStrong() || (list[i]->isStrong() && !onlyKillWeak))){
            if(hitDecrease == 100){
                list[i]->setDead();
                state = 1;
            }
            else{
                for(int j = 0; j < hitDecrease; j++){
                    list[i]->consume();
                }
                if(list[i]->getHitpoints() <= 0)
                    state = 1;
                else
                    state = 0;
            }
            return true;
        }
    }
    return false;
}

void StudentWorld::squirt(int x, int y, Actor::Direction dir){
    Squirt* sq = new Squirt(this, player, dir, x, y);
    playSound(SOUND_PLAYER_SQUIRT);
    actor.push_back(sq);
}


