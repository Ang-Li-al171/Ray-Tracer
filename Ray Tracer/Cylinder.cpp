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
    axisDirection = axisDirection.unit();
    reflection = refl;
    transparency = trans;
    centerUp = center.add(axisDirection.times(height));
    objectList[0] = new PlaneCircle(center, axisDirection.times(-1), radius, reflection, transparency, shadow);
    objectList[1] = new PlaneCircle(centerUp, axisDirection, radius, reflection, transparency, shadow);
}

Cylinder::Cylinder(const Vec3 &c, int r, int h, const Vec3 &ad, const Vec3 &sc,
               const Vec3 &ec, float refl, float trans,
               Texture* tex, bool shadow) {
    texture = tex;
    causeShadowBool = shadow;
    center = Vec3(c);
    radius = r;
    height = h;
    axisDirection = ad;
    axisDirection = axisDirection.unit();
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
    for (int i = 0; i<2; i++) {
        if (objectList[i]->intersect(oo, dd, &tHit)) {
            if (tHit < tNear){
                tNear = tHit;
            }
        }
    }
    
    
    
    Vec3 oTemp = oo.diff(axisDirection.unit().times(axisDirection.unit().dot(oo)));
    Vec3 dTemp = dd.diff(axisDirection.unit().times(axisDirection.unit().dot(dd)));
    Vec3 oCylinderTemp = center.diff(axisDirection.unit().times(axisDirection.unit().dot(center)));

    float a = dTemp.dot(dTemp);
    float b = oTemp.dot(dTemp)*2 - dTemp.dot(oCylinderTemp)*2;
    float c = oTemp.dot(oTemp) - oTemp.dot(oCylinderTemp)*2 + oCylinderTemp.dot(oCylinderTemp) - radius*radius;
    
    float delta = b*b - 4*a*c;
    Vec3 ooCynlinderTemp = oo.diff(oCylinderTemp);
    
    if (delta >= 0) {
        float tFinal = INFINITY;
        float t1 = (-b-sqrt(delta))/(2*a);
        Vec3 hitP1 = oo.add(dd.times(t1));
        Vec3 pc1 = hitP1.diff(center);
        Vec3 pcProjected1 = axisDirection.unit().times(pc1.dot(axisDirection.unit()));

        float t2 = (-b+sqrt(delta))/(2*a);
        Vec3 hitP2 = oo.add(dd.times(t2));
        Vec3 pc2 = hitP2.diff(center);
        Vec3 pcProjected2 = axisDirection.unit().times(pc2.dot(axisDirection.unit()));

        
        if (t1 > 0 || t2 < 0) {
            if (t1>0 && pcProjected1.dot(axisDirection.unit()) > 0 && pcProjected1.length() <= height){
                tFinal = t1;
            }
            else if (t2 > 0 && pcProjected2.dot(axisDirection.unit()) > 0 && pcProjected2.length() <= height) {
                tFinal = t2;
            }
            if (tFinal<tNear) {
                tNear = tFinal;
            }
        }
    }
    
    if (tNear != INFINITY) {
        *t = tNear;
        return true;
    }
    return false;

}

Vec3 Cylinder::getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l){
    
    //assuming the ball is always upright for now
    Vec3 hitPoint = Vec3(hitP);
    
    Vec3 textureColor = Vec3(0, 0, 0);
    float texIndex = 0;
    
    //TODO Texture
    if (!texture->getInvalid()){
        texIndex = 0.7;
        
        
        if (hitPoint.diff(center).length() < radius || hitPoint.diff(centerUp).length()<radius) {
            //one of the caps
            float u = (radius + hitPoint.getElement(0) - center.getElement(0))/ (float)(2 * radius);
            float v = (radius + hitPoint.getElement(2) - center.getElement(2))/(float) (2 * radius);
            textureColor = texture->getColor(u, v);
            
        } else {
            //not on caps
            float u = acos(hitPoint.getElement(0) / (float) radius)/ (2*PI);
            float v = (hitPoint.getElement(1) - centerUp.getElement(1)) / (float) height;
            
            if (u < 0) u = 0;
            
            textureColor = texture->getColor(u, v);
        }
    }
    
    // use this as default, need to add as parameters in constructor
    float Diffuse = 1.20;
    float Ambient = 0.30;
    float Specular = 0.8;
    
    Vec3 lightSource = l.getLightSource();
    
    Vec3 rayDirection = Vec3(d).unit();
    Vec3 lightDirection = lightSource.diff(hitPoint).unit();
    Vec3 n = getN(hitPoint, rayDirection);
    Vec3 h = rayDirection.times(-1).add(lightDirection).unit();
    
    Vec3 LDiffuse = l.getDiffuse().times(Diffuse).times(n.dot(lightDirection)).clamp(0.0, 255.0);
    
    Vec3 LSpecular = Vec3(0, 0, 0);
    if (n.dot(h)>0){
        LSpecular = l.getSpecular().times(Specular).times(pow(n.dot(h), 70));
    }
    
    Vec3 LAmbient = l.getAmbient().times(Ambient);
    
    return LDiffuse.add(LSpecular).add(LAmbient).times(1.0/255.0).times(1-texIndex).add(textureColor.times(texIndex));
    
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
        n = v1.diff(v2).unit();
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