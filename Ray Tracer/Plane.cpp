//
//  Plane.cpp
//  Ray Tracer
//
//  Created by Ang Li on 11/19/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "Plane.h"

Plane::Plane(void){
    width = 0;
    height = 0;
    reflection = 0;
    transparency = 0;
    // empty default constructor
}

Plane::Plane(const Vec3 &ct, const Vec3 &normal, int w, int h,
             const Vec3 &sc, const Vec3 &ec, float refl, float trans,
             float*** textureImage){
    center = Vec3(ct);
    n = Vec3(normal);
    width = w;
    height = h;
    surfaceColor = Vec3(sc);
    emissionColor = Vec3(ec);
    reflection = refl;
    transparency = trans;
    texture = textureImage;
}

Plane::~Plane(void){
    
}

bool Plane::intersect(const Vec3 &o, const Vec3 &d, float *t){
    
    Vec3 rayOrigin = Vec3(o);
    Vec3 rayDir = Vec3(d);
    
    // if not parallel to the plane
    if (rayDir.dot(n) != 0) {
        float tForPlaneHit = ((center.diff(rayOrigin).dot(n))/(rayDir.dot(n)));
        if (tForPlaneHit > 0) {
            
            Vec3 planeHitP = rayOrigin.add(rayDir.times(tForPlaneHit));
            
            
            // this part is not very right... only works for planes with dir only in the y direction
            int x = fabs(planeHitP.getElement(0));
            int y = fabs(planeHitP.getElement(2));
            if (x < width && y < height){
                x = x%288; // need to get this info from texture_image
                y = y%288; // need to get this info from texture_image
                
                *t = tForPlaneHit;
                return true;
            }
            
        }
    }
    
    return false;
}

Vec3 Plane::getLightAt(const Vec3 &d, const Vec3 &hitP, Light &l){
    
    Vec3 planeHitP = Vec3(hitP);
    
    int x = fabs(planeHitP.getElement(0));
    int y = fabs(planeHitP.getElement(2));
    
    x = x%288;
    y = y%288;
    
    Vec3 textureColor = Vec3(texture[x][y][0], texture[x][y][1], texture[x][y][2]);
    
    // use this as default, need to add as parameters in constructor
    float Diffuse = 0.90;
    float Ambient = 0.10;
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
    
    return LDiffuse.add(LSpecular).add(LAmbient).times(1.0/255.0).times(0.5).add(textureColor.times(0.5));

}

Vec3 Plane::getN(const Vec3 &hitP, const Vec3 &d){
    Vec3 rayDir = Vec3(d);
    Vec3 nNorm = n.unit();
    if (rayDir.dot(nNorm) < 0)
        return nNorm;
    else
        return nNorm.times(-1);
}

bool Plane::rayInside(const Vec3 &hitP, const Vec3 &d){
    return false;
}

Vec3 Plane::getCenter(void){
    return center;
}

float Plane::getRefl(void){
    return reflection;
}

float Plane::getTrans(void){
    return transparency;
}

Vec3 Plane::getSurfaceColor(void){
    return surfaceColor;
}

Vec3 Plane::getEmissionColor(void){
    return emissionColor;
}