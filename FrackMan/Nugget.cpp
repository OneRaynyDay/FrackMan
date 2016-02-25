//
//  Nugget.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "Nugget.h"

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
            getWorld()->attackProtestersAt(getX(), getY(), 4, HIT_DECREASE, flag, true);
            consume();
        }
    }
}