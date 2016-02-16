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
        std::cout<<"hi"<<std::endl;
    }
    else{
        /*DO SOMETHING HERE TO THE PROTESTERS*/
    }
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