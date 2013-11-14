//
//  Sphere.cpp
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 11/7/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "Sphere.h"

Sphere::Sphere(void) {
    radius = 0;
    reflection = 0;
    transparency = 0;
}

Sphere::Sphere(const Vec3 &c, int r, const Vec3 &sc, const Vec3 &ec, float refl, float trans) {
    center = Vec3(c);
    radius = r;
    surfaceColor = Vec3(sc);
    emissionColor = Vec3(ec);
    reflection = refl;
    transparency = trans;
}

Sphere::~Sphere(){
    //TODO
}

bool Sphere::intersect(const Vec3 &o, const Vec3 &d, float *t){
    Vec3 dd = Vec3(d);
    Vec3 oo = Vec3(o);
    float D = powf(dd.dot(oo.diff(center)), 2)-(dd.dot(dd)*(oo.diff(center).dot(oo.diff(center))-radius*radius));
    if (D < 0) return false;
    else if (D == 0) {
        float t1 = (-dd.dot(oo.diff(center))-sqrt(D))/dd.dot(dd);
        if (t1 < 0) {
            return false;
        } else {
            *t = t1;
            return true;
        }
    } else {
        float t1 = (-dd.dot(oo.diff(center))-sqrt(D))/dd.dot(dd);
        float t2 = (-dd.dot(oo.diff(center))+sqrt(D))/dd.dot(dd);
        if (t1<0 && t2<0) {
            return false;
        } else if (t1 < 0 && t2>=0) {
            *t = t2;
            return true;
        } else {
            *t = t1;
            return true;
        }
    }
}
Vec3 Sphere::getCenter() {
    return center;
}

