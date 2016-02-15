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
    getPlayer()->increaseNugget(1);
}


void Nugget::doSomething(){
    if(pickup){
        checkDiscovered(getPlayer());
    }
    else{
        hitpoints--;
        if(hitpoints == 0){
            consume();
        }
        vector<Protester*> protesters = getWorld()->getProtesters();
        for(vector<Protester*>::iterator it = protesters.begin() ; it != protesters.end(); it++){
            checkDiscovered((*it));
        }
    }
}