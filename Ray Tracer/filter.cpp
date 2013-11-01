//
//  filter.cpp
//  C++_2
//
//  Created by Ang Li on 10/19/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "filter.h"

Filter::Filter(int type, int rInput){
    
    filterArray = new double[rInput*2+1];
    r = rInput;
    
    switch(type){
        case BOX: {fillInBoxFilter(); break;}
        case TENT: {fillInTentFilter(); break;}
        case GAUSSIAN: {fillInGaussianFilter(); break;}
        default: printf("Unknown type of filter");
    }
    
}

double* Filter::getFilterArray(){
    return filterArray;
}

int Filter::getR(void){
    return r;
}

void Filter::fillInBoxFilter(){
    for (int i=0;i<r*2+1;i++){
        double rd = (double) r;
        filterArray[i]=1/(2*rd+1);
    }
}

void Filter::fillInTentFilter(){
    // first create tent with y = -|x|+r then normalize through dividing by r^2
    for (int i=0;i<2*r+1;i++){
        double rd = (double) r;
        double x = -rd+i;
        if (x>0)
            filterArray[i]= (rd-x)/(rd*rd);
        else
            filterArray[i]= (rd+x)/(rd*rd);
    }
}

void Filter::fillInGaussianFilter(){
    // not sure how to normaliza this one
    for (int i=0;i<2*r+1;i++){
        double rd = (double) r;
        double x = -rd+i;
        filterArray[i]= 1/sqrt(2*PI)*exp(-x*x/2);
    }
}