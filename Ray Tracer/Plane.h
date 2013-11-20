//
//  Plane.h
//  Ray Tracer
//
//  Created by Ang Li on 11/19/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__Plane__
#define __Ray_Tracer__Plane__

#include <iostream>
#include "Sphere.h"

#endif /* defined(__Ray_Tracer__Plane__) */

class Plane: public TObject{
public:
    Plane(void);
    Plane(const Vec3 &ct, const Vec3 &normal, int w, int h,
          const Vec3 &sc, const Vec3 &ec, float refl, float trans,
          float*** textureImage);
    ~Plane(void);
    bool intersect(const Vec3 &o, const Vec3 &d, float *t);
    Vec3 getCenter(void);
    Vec3 getN(const Vec3 &hitP, const Vec3 &d);
    float getRefl(void);
    float getTrans(void);
    Vec3 getSurfaceColor(void);
    Vec3 getEmissionColor(void);
    Vec3 getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l);
    bool rayInside(const Vec3 &hitP, const Vec3 &d);
    
private:
    float*** texture;
    Vec3 center;
    Vec3 n;
    int width;
    int height;
    Vec3 surfaceColor;
    Vec3 emissionColor;
    float reflection;
    float transparency;
};