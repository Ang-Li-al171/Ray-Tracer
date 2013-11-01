//
//  filter.h
//  C++_2
//
//  Created by Ang Li on 10/19/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __C___2__filter__
#define __C___2__filter__

#include <iostream>
#include <math.h>

#define PI 3.141592653

using namespace std;

enum { BOX = 0, TENT = 1, GAUSSIAN = 2};


class Filter{
public:
    Filter(int type, int rInput);
    ~Filter(void);
    void fillInBoxFilter(void);
    void fillInTentFilter(void);
    void fillInGaussianFilter(void);
    double* getFilterArray(void);
    int getR(void);
    
private:
    double* filterArray;
    int r;
};

#endif /* defined(__C___2__filter__) */
