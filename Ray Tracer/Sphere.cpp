//
//  Sphere.cpp
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 11/7/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "Sphere.h"

#define PI 3.141592653

Sphere::Sphere(void) {
    radius = 0;
    reflection = 0;
    transparency = 0;
}

Sphere::Sphere(const Vec3 &c, int r, float refl, float trans,
               bool shadow) {
    causeShadowBool = shadow;
    center = Vec3(c);
    radius = r;
    reflection = refl;
    transparency = trans;
}

Sphere::Sphere(const Vec3 &c, int r, const Vec3 &sc,
               const Vec3 &ec, float refl, float trans,
               float*** tex, int texW, int texH, bool shadow) {
    texture = tex;
    textureWidth = texW;
    textureHeight = texH;
    causeShadowBool = shadow;
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

bool Sphere::causeShadow(){
    return causeShadowBool;
}

bool Sphere::intersect(const Vec3 &o, const Vec3 &d, float *t){
    Vec3 dd = Vec3(d);
    Vec3 oo = Vec3(o);
    float D = powf(dd.dot(oo.diff(center)), 2)-(dd.dot(dd)*(oo.diff(center).dot(oo.diff(center))-radius*radius));
    if (D < 0) return false;
    else if (D == 0) {
        float t1 = (-dd.dot(oo.diff(center))-sqrt(D))/dd.dot(dd);
        if (t1 <= 0) {
            return false;
        } else {
            *t = t1;
            return true;
        }
    } else {
        float t1 = (-dd.dot(oo.diff(center))-sqrt(D))/dd.dot(dd);
        float t2 = (-dd.dot(oo.diff(center))+sqrt(D))/dd.dot(dd);
        if (t1<=0 && t2<=0) {
            return false;
        } else if (t1 <= 0 && t2>0) {
            *t = t2;
            return true;
        } else {
            *t = t1;
            return true;
        }
    }
}

Vec3 Sphere::getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l){
    
    //assuming the ball is always upright for now
    Vec3 hitPoint = Vec3(hitP);
    
    Vec3 textureColor;
    if (texture != NULL){
    // projection of distance vector onto the x and y directions
    // this calculation is kinda ugly... need to fix it soon
    int xposi = 0;
    if (hitPoint.diff(center).getElement(2) > 0){
        xposi = (hitPoint.diff(center).dot(Vec3(1, 0, 0))/(2.0*radius) + 1.0/2.0)*textureWidth/2.0;
    }
    else{
        xposi = ((hitPoint.diff(center).dot(Vec3(1, 0, 0))/(2.0*radius) + 1.0/2.0)/2.0 + 1.0/2.0)*textureWidth;
    }
    int yposi = (hitPoint.diff(center).dot(Vec3(0, 1, 0))/(2.0*radius) + 1.0/2.0)*textureHeight;
    
    textureColor = Vec3(texture[yposi][xposi][0],
                        texture[yposi][xposi][1],
                        texture[yposi][xposi][2]);
    }
    
    // use this as default, need to add as parameters in constructor
    float Diffuse = 1.20;
    float Ambient = 0.30;
    float Specular = 0.8;
//    float transmission = 1;
    
    Vec3 lightSource = l.getLightSource();
    
    Vec3 rayDirection = Vec3(d).unit();
    Vec3 lightDirection = lightSource.diff(hitPoint).unit();
    Vec3 n = hitPoint.diff(center).unit();
    Vec3 h = rayDirection.times(-1).add(lightDirection).unit();
    
    Vec3 LDiffuse = l.getDiffuse().times(Diffuse).times(n.dot(lightDirection)).clamp(0.0, 255.0);
    
    Vec3 LSpecular = Vec3(0, 0, 0);
    if (n.dot(h)>0){
        LSpecular = l.getSpecular().times(Specular).times(pow(n.dot(h), 70));
    }
    
    Vec3 LAmbient = l.getAmbient().times(Ambient);
    
    if (texture != NULL){
        return LDiffuse.add(LSpecular).add(LAmbient).times(1.0/255.0).times(0.2).add(textureColor.times(0.8));
    }else{
        return LDiffuse.add(LSpecular).add(LAmbient).times(1.0/255.0);
    }
    
}

Vec3 Sphere::getN(const Vec3 &hitP, const Vec3 &d){
    Vec3 P = Vec3(hitP);
    Vec3 rayDir = Vec3(d);
    Vec3 n = P.diff(center).unit();
    if (rayDir.dot(n) <= 0)
        return n;
    else{
        return n.times(-1);
    }
}

bool Sphere::rayInside(const Vec3 &hitP, const Vec3 &d){
    Vec3 P = Vec3(hitP);
    Vec3 rayDir = Vec3(d);
    Vec3 n = P.diff(center).unit();
    if (rayDir.dot(n) > 0)
        return true;
    else
        return false;
}

Vec3 Sphere::getCenter() {
    return center;
}

float Sphere::getRefl() {
    return reflection;
}

float Sphere::getTrans() {
    return transparency;
}

Vec3 Sphere::getSurfaceColor() {
    return surfaceColor;
}

Vec3 Sphere::getEmissionColor(){
    return emissionColor;
}
