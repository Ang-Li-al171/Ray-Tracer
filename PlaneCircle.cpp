//
//  PlaneCircle.cpp
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 11/28/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "PlaneCircle.h"

PlaneCircle::PlaneCircle(void){
    radius = 0;
    reflection = 0;
    transparency = 0;
    // empty default constructor
}

PlaneCircle::PlaneCircle(const Vec3 &ct, const Vec3 &normal, int r,
             const Vec3 &sc, const Vec3 &ec,
             float refl, float trans,
             float*** textureImage, bool causeShadow){
    causeShadowBool = causeShadow;
    center = Vec3(ct);
    n = Vec3(normal).unit();
    radius = r;
    surfaceColor = Vec3(sc);
    emissionColor = Vec3(ec);
    reflection = refl;
    transparency = trans;
    texture = textureImage;
}

PlaneCircle::PlaneCircle(const Vec3 &ct, const Vec3 &normal, int r,
             float refl, float trans,
             bool causeShadow){
    causeShadowBool = causeShadow;
    center = Vec3(ct);
    n = Vec3(normal).unit();
    radius = r;
    reflection = refl;
    transparency = trans;
}

PlaneCircle::~PlaneCircle(void){
    
}

bool PlaneCircle::causeShadow(){
    return causeShadowBool;
}

bool PlaneCircle::intersect(const Vec3 &o, const Vec3 &d, float *t){
    
    Vec3 rayOrigin = Vec3(o);
    Vec3 rayDir = Vec3(d);
    
    // if not parallel to the plane
    if (rayDir.dot(n) != 0) {
        
        float tForPlaneHit = ((center.diff(rayOrigin).dot(n))/(rayDir.dot(n)));
        
        if (tForPlaneHit > 0) {
            
            Vec3 planeHitP = rayOrigin.add(rayDir.times(tForPlaneHit));
            
            // projection of distance vector onto the x and y directions
            float l = planeHitP.diff(center).length();
            if (l < radius){
                *t = tForPlaneHit;
                return true;
            }
        }
    }
    
    return false;
}

Vec3 PlaneCircle::getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l){
    
    Vec3 planeHitP = Vec3(hitP);
    
    Vec3 textureColor = Vec3(0, 0, 0);
    float texIndex = 0;
    if (texture != NULL){
        texIndex = 0.7;
        Vec3 t = Vec3(0, 0, 1);
        if (n.dot(t) == 1 || n.dot(t) == -1) {
            t = Vec3(0, 1, 0);
        }
        
        Vec3 x = (t.cross(n)).times(1/(t.cross(n).length()));
        Vec3 y = n.cross(x);
        
        // projection of distance vector onto the x and y directions
        int xposi = planeHitP.diff(center).dot(x) + radius/2;
        int yposi = planeHitP.diff(center).dot(y) + radius/2;
        
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
    Vec3 hitPoint = Vec3(hitP);
    
    Vec3 rayDirection = Vec3(d).unit();
    Vec3 lightDirection = lightSource.diff(hitPoint).unit();
    Vec3 n = getN(hitP, d);
    Vec3 h = rayDirection.times(-1).add(lightDirection).unit();
    
    Vec3 LDiffuse = l.getDiffuse().times(Diffuse).times(n.dot(lightDirection)).clamp(0.0, 255.0);
    
    Vec3 LSpecular = Vec3(0, 0, 0);
    if (n.dot(h)>0){
        LSpecular = l.getSpecular().times(Specular).times(pow(n.dot(h), 70));
    }
    
    Vec3 LAmbient = l.getAmbient().times(Ambient);
    
    return LDiffuse.add(LSpecular).add(LAmbient).times(1.0/255.0).times(1-texIndex).add(textureColor.times(texIndex));
    
}

Vec3 PlaneCircle::getN(const Vec3 &hitP, const Vec3 &d){
    Vec3 rayDir = Vec3(d);
    Vec3 nNorm = n.unit();
    if (rayDir.dot(nNorm) <= 0)
        return nNorm;
    else
        return nNorm.times(-1);
}

bool PlaneCircle::rayInside(const Vec3 &hitP, const Vec3 &d){
    return false;
}

Vec3 PlaneCircle::getCenter(void){
    return center;
}

float PlaneCircle::getRefl(void){
    return reflection;
}

float PlaneCircle::getTrans(void){
    return transparency;
}

Vec3 PlaneCircle::getSurfaceColor(void){
    return surfaceColor;
}

Vec3 PlaneCircle::getEmissionColor(void){
    return emissionColor;
}