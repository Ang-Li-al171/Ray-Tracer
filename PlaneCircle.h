//
//  PlaneCircle.h
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 11/28/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__PlaneCircle__
#define __Ray_Tracer__PlaneCircle__

#include <iostream>
#include "Sphere.h"

using namespace std;

class PlaneCircle : public TObject{
public:
    PlaneCircle(void);
    PlaneCircle(const Vec3 &ct, const Vec3 &normal,
          int r,
          const Vec3 &sc, const Vec3 &ec, float refl, float trans,
          float*** textureImage, bool causeShadow);
    PlaneCircle(const Vec3 &ct, const Vec3 &normal, int r, float refl, float trans, bool causeShadow);
    ~PlaneCircle(void);
    bool intersect(const Vec3 &o, const Vec3 &d, float *t);
    Vec3 getCenter(void);
    Vec3 getN(const Vec3 &hitP, const Vec3 &d);
    float getRefl(void);
    float getTrans(void);
    Vec3 getSurfaceColor(void);
    Vec3 getEmissionColor(void);
    Vec3 getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l);
    bool rayInside(const Vec3 &hitP, const Vec3 &d);
    bool causeShadow();
    
private:
    float*** texture;
    bool causeShadowBool;
    Vec3 center;
    Vec3 n;
    int radius;
    Vec3 surfaceColor;
    Vec3 emissionColor;
    float reflection;
    float transparency;
};

#endif /* defined(__Ray_Tracer__PlaneCircle__) */
