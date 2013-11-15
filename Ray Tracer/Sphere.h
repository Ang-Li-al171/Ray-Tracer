//
//  Sphere.h
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 11/7/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__Sphere__
#define __Ray_Tracer__Sphere__

#include <iostream>
#include "Vec3.h"

using namespace std;

class Sphere{
public:
    Sphere(void);
    Sphere(const Vec3 &c, int r, const Vec3 &sc, const Vec3 &ec, float refl, float trans);
    ~Sphere(void);
    bool intersect(const Vec3 &o, const Vec3 &d, float *t);
    Vec3 getCenter(void);
    float getRefl(void);
    float getTrans(void);
    Vec3 getSurfaceColor(void);
    
private:
    Vec3 center;
    int radius;
    Vec3 surfaceColor;
    Vec3 emissionColor;
    float reflection;
    float transparency;
};

#endif /* defined(__Ray_Tracer__Sphere__) */
