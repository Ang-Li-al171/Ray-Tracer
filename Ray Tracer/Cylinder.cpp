//
//  Cylinder.cpp
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 11/27/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "Cylinder.h"

#define PI 3.141592653

Cylinder::Cylinder(void) {
    radius = 0;
    reflection = 0;
    transparency = 0;
    height = 0;
}

Cylinder::Cylinder(const Vec3 &c, int r, int h, const Vec3 &ad, float refl, float trans,
               bool shadow) {
    causeShadowBool = shadow;
    center = Vec3(c);
    radius = r;
    height = h;
    axisDirection = ad;
    reflection = refl;
    transparency = trans;
    centerUp = center.add(axisDirection.times(height));
    objectList[0] = new PlaneCircle(center, axisDirection.times(-1), radius, reflection, transparency, shadow);
    objectList[1] = new PlaneCircle(centerUp, axisDirection, radius, reflection, transparency, shadow);
}

Cylinder::Cylinder(const Vec3 &c, int r, int h, const Vec3 &ad, const Vec3 &sc,
               const Vec3 &ec, float refl, float trans,
               float*** tex, int texW, int texH, bool shadow) {
    texture = tex;
    textureWidth = texW;
    textureHeight = texH;
    causeShadowBool = shadow;
    center = Vec3(c);
    radius = r;
    height = h;
    axisDirection = ad;
    surfaceColor = Vec3(sc);
    emissionColor = Vec3(ec);
    reflection = refl;
    transparency = trans;
    objectList[0] = new PlaneCircle(center, axisDirection.times(-1), radius, reflection, transparency, shadow);
    objectList[1] = new PlaneCircle(centerUp, axisDirection, radius, reflection, transparency, shadow);
}

Cylinder::~Cylinder(){
    //TODO
}

bool Cylinder::causeShadow(){
    return causeShadowBool;
}

bool Cylinder::intersect(const Vec3 &o, const Vec3 &d, float *t){
    Vec3 dd = Vec3(d).unit();
    Vec3 oo = Vec3(o);
    
    float tHit = INFINITY;
    float tNear = INFINITY;
    TObject* hitObj;
    for (int i = 0; i<2; i++) {
        if (objectList[i]->intersect(oo, dd, &tHit)) {
            if (tHit < tNear){
                tNear = tHit;
                hitObj = objectList[i];
            }
        }
    }
    
    if (tHit != INFINITY) {
        *t = tHit;
        return true;
    }
    return false;

    
    
//    Vec3 oTemp = oo.diff(axisDirection.unit().times(axisDirection.unit().dot(oo)));
//    Vec3 dTemp = dd.diff(axisDirection.unit().times(axisDirection.unit().dot(dd)));
//    Vec3 oCylinderTemp = center.diff(axisDirection.unit().times(axisDirection.unit().dot(center)));
//
//    float a = dTemp.dot(dTemp);
//    float b = oTemp.dot(dTemp)*2 - dTemp.dot(oCylinderTemp)*2;
//    float c = oTemp.dot(oTemp) - oTemp.dot(oCylinderTemp)*2 + oCylinderTemp.dot(oCylinderTemp) - radius*radius;
//    
//    float delta = b*b - 4*a*c;
//    Vec3 ooCynlinderTemp = oo.diff(oCylinderTemp);
//    
//    if (delta < 0) {
//        return false;
//    } else {
//        float tFinal = 0;
//        float t1 = (-b-sqrt(delta))/(2*a);
//        float t2 = (-b+sqrt(delta))/(2*a);
//        if (t1 < 0 && t2 < 0) return false;
//        else {
//            if (t1>0) tFinal = t1;
//            else tFinal = t2;
//            Vec3 hitP = oo.add(dd.times(tFinal));
//            Vec3 pc = hitP.diff(center);
//            Vec3 pcProjected = axisDirection.unit().times(pc.dot(axisDirection.unit()));
//            if (pcProjected.dot(axisDirection.unit()) > 0 && pcProjected.length() <= height) {
//                *t = tFinal;
//                return true;
//            } else {
//                return false;
//            }
//        }
//    }
}

//TODO
Vec3 Cylinder::getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l){
    
//    //assuming the ball is always upright for now
//    Vec3 hitPoint = Vec3(hitP);
//    
//    Vec3 textureColor = Vec3(0, 0, 0);
//    float texIndex = 0;
//    if (texture != NULL){
//        texIndex = 0.7;
//        // projection of distance vector onto the x and y directions
//        // this calculation is kinda ugly... need to fix it soon
//        int xposi = 0;
//        if (hitPoint.diff(center).getElement(2) > 0){
//            xposi = (hitPoint.diff(center).dot(Vec3(1, 0, 0))/(2.0*radius) + 1.0/2.0)*textureWidth/2.0-1;
//        }
//        else{
//            xposi = ((hitPoint.diff(center).dot(Vec3(1, 0, 0))/(2.0*radius) + 1.0/2.0)/2.0 + 1.0/2.0)*textureWidth-1;
//        }
//        int yposi = (hitPoint.diff(center).dot(Vec3(0, -1, 0))/(2.0*radius) + 1.0/2.0)*textureHeight-1;
//        
//        if (yposi < 0)
//            yposi = 0;
//        if (xposi < 0)
//            xposi = 0;
//        textureColor = Vec3(texture[yposi][xposi][0],
//                            texture[yposi][xposi][1],
//                            texture[yposi][xposi][2]);
//    }
//    
//    // use this as default, need to add as parameters in constructor
//    float Diffuse = 1.20;
//    float Ambient = 0.30;
//    float Specular = 0.8;
//    
//    Vec3 lightSource = l.getLightSource();
//    
//    Vec3 rayDirection = Vec3(d).unit();
//    Vec3 lightDirection = lightSource.diff(hitPoint).unit();
//    Vec3 n = hitPoint.diff(center).unit();
//    Vec3 h = rayDirection.times(-1).add(lightDirection).unit();
//    
//    Vec3 LDiffuse = l.getDiffuse().times(Diffuse).times(n.dot(lightDirection)).clamp(0.0, 255.0);
//    
//    Vec3 LSpecular = Vec3(0, 0, 0);
//    if (n.dot(h)>0){
//        LSpecular = l.getSpecular().times(Specular).times(pow(n.dot(h), 70));
//    }
//    
//    Vec3 LAmbient = l.getAmbient().times(Ambient);
//    
//    return LDiffuse.add(LSpecular).add(LAmbient).times(1.0/255.0).times(1-texIndex).add(textureColor.times(texIndex));
    
    return Vec3(1, 1, 0);
}

Vec3 Cylinder::getN(const Vec3 &hitP, const Vec3 &d){
    
    Vec3 P = Vec3(hitP);
    Vec3 rayDir = Vec3(d);
    Vec3 n = Vec3();
    
    //hit point on floor cap
    if (P.diff(center).length() < radius) n = axisDirection.times(-1);
    
    //hit point on ceiling cap
    else if (P.diff(centerUp).length()<radius) n = axisDirection;
    
    //hit point not on caps
    else {
        Vec3 v1 = P.diff(center);
        Vec3 v2 = axisDirection.times(v1.dot(axisDirection));
        n = v1.diff(v2);
    }
    
    if (rayDir.dot(n) <= 0){
        return n;
    } else{
        return n.times(-1);
    }
}

bool Cylinder::rayInside(const Vec3 &hitP, const Vec3 &d){
    Vec3 P = Vec3(hitP);
    Vec3 rayDir = Vec3(d);
    
    Vec3 n = Vec3();
    //hit point on floor cap
    if (P.diff(center).length() < radius) n = axisDirection.times(-1);
    
    //hit point on ceiling cap
    else if (P.diff(centerUp).length()<radius) n = axisDirection;
    
    //hit point not on caps
    else {
        Vec3 v1 = P.diff(center);
        Vec3 v2 = axisDirection.times(v1.dot(axisDirection));
        n = v1.diff(v2);
    }

    if (rayDir.dot(n) > 0)
        return true;
    else
        return false;
}

Vec3 Cylinder::getCenter() {
    return center;
}

float Cylinder::getRefl() {
    return reflection;
}

float Cylinder::getTrans() {
    return transparency;
}

Vec3 Cylinder::getSurfaceColor() {
    return surfaceColor;
}

Vec3 Cylinder::getEmissionColor(){
    return emissionColor;
}