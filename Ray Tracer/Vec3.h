//
//  Vec3.h
//  Ray Tracer
//
//  Created by Ang Li on 11/1/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__Vec3__
#define __Ray_Tracer__Vec3__

#include <iostream>

using namespace std;

class Vec3{
public:
    Vec3(float x, float y, float z);
    ~Vec3(void);
    float getElement(int index);
    Vec3 add(Vec3 v);
    Vec3 diff(Vec3 v);
    float dot(Vec3 v);
    Vec3 cross(Vec3 v);
    
private:
    float elements[3];
};


#endif /* defined(__Ray_Tracer__Vec3__) */
