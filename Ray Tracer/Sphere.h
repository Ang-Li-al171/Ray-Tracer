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
#include "Light.h"
#include <math.h>

using namespace std;

class TObject
{
public:
    virtual ~TObject() {}
    virtual bool intersect(const Vec3 &o, const Vec3 &d, float *t) = 0;
//    virtual Vec3 getSurfaceColor(void);
//    virtual Vec3 getEmissionColor(void);
    virtual float getRefl(void) = 0;
    virtual float getTrans(void) = 0;
    virtual Vec3 getN(const Vec3 &hitP, const Vec3 &d) = 0;
    virtual Vec3 getCenter(void) = 0;
    virtual Vec3 getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l) = 0;
    virtual bool rayInside(const Vec3 &hitP, const Vec3 &d) = 0;
    virtual bool causeShadow() = 0;
};


class Sphere: public TObject{
public:
    Sphere(void);
    Sphere(const Vec3 &c, int r, float refl, float trans,
                   bool shadow);
    Sphere(const Vec3 &c, int r, const Vec3 &sc, const Vec3 &ec,
           float refl, float trans,
           float*** tex, int texW, int texH, bool shadow);
    ~Sphere(void);
    bool intersect(const Vec3 &o, const Vec3 &d, float *t);
    Vec3 getCenter(void);
    float getRefl(void);
    float getTrans(void);
    Vec3 getN(const Vec3 &hitP, const Vec3 &d);
    Vec3 getSurfaceColor(void);
    Vec3 getEmissionColor(void);
    Vec3 getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l);
    bool rayInside(const Vec3 &hitP, const Vec3 &d);
    bool causeShadow();
    
private:
    Vec3 center;
    float*** texture;
    int radius;
    bool causeShadowBool;
    int textureHeight;
    int textureWidth;
    Vec3 surfaceColor;
    Vec3 emissionColor;
    float reflection;
    float transparency;
};

#endif /* defined(__Ray_Tracer__Sphere__) */
