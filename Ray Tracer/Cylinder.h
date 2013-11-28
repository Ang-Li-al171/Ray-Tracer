//
//  Cylinder.h
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 11/27/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__Cylinder__
#define __Ray_Tracer__Cylinder__

#include <iostream>
#include "Vec3.h"
#include "Light.h"
#include <math.h>
#include "Sphere.h"
#include "PlaneCircle.h"


using namespace std;

class Cylinder: public TObject{
public:
    Cylinder(void);
    Cylinder(const Vec3 &c, int r, int h, const Vec3 &ad, float refl, float trans,
           bool shadow);
    Cylinder(const Vec3 &c, int r, int h, const Vec3 &ad, const Vec3 &sc, const Vec3 &ec,
           float refl, float trans,
           float*** tex, int texW, int texH, bool shadow);
    ~Cylinder(void);
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
    TObject** objectList = new TObject*[2];
    Vec3 center;
    Vec3 centerUp;
    float*** texture;
    int radius;
    int height;
    Vec3 axisDirection;
    bool causeShadowBool;
    int textureHeight;
    int textureWidth;
    Vec3 surfaceColor;
    Vec3 emissionColor;
    float reflection;
    float transparency;
};



#endif /* defined(__Ray_Tracer__Cylinder__) */
