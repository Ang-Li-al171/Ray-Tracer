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