//
//  RayTracer.cpp
//  Ray Tracer
//
//  Created by Ang Li on 11/1/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "RayTracer.h"

int MAX_RAY_DEPTH = 2;
float*** texture_image;

RayTracer::RayTracer(){
    width = 512;
    height = 512;
    max = 255;
    myImage = new ImageIO(width, height, max);
    // other initiation in constructor?
}

RayTracer::~RayTracer(){
    delete myImage;
}

Vec3 RayTracer::trace(Vec3 origin, Vec3 d, TObject** sphereList, int size, int depth) {

    //take record of smallest t, for the nearest object in the direction of the ray
    float t = INFINITY;
    float tNear = INFINITY;
    Sphere* hitSphere;
    for (int i = 0; i<size; i++) {
        if (sphereList[i]->intersect(origin, d, &t)) {
            if (t < tNear){
                tNear = t;
                hitSphere = (Sphere*) sphereList[i];
            }
        }
    }
    
    // if there's an intersection with an object
    if (tNear != INFINITY) {
        
        Vec3 surfaceColor = Vec3(0, 0, 0);
        Vec3 hitP = origin.add(d.times(tNear));
        Vec3 n = hitP.diff(hitSphere->getCenter());
        Vec3 normalizedN = n.unit();
        
        float bias = 1e-4;
        bool inside = false;
        if (d.dot(normalizedN) > 0) {
            normalizedN = normalizedN.times(-1);
            inside = true;
        }
        
        // if reflection or refraction is needed, recurse
        if (depth < MAX_RAY_DEPTH && (hitSphere->getRefl() > 0 || hitSphere->getTrans() >0)){
            
            Vec3 reflectedRay = d.diff(normalizedN.times(2).times(d.dot(normalizedN)));
            
            Plane *checkerBoard = new Plane(Vec3(0, -200, 0), Vec3(0, 1, 0), 288*2, 288*2, Vec3(0, 0, 0), Vec3(0, 0, 0), 0.0, 0.0, texture_image);
            
            Vec3 reflectionColor = trace(origin, d, sphereList, size, depth+1);
            Vec3 surfaceColor = Vec3(0, 0, 0);
            
            float reflectT;
            if (checkerBoard->intersect(hitP, reflectedRay, &reflectT)){
            
                Vec3 planeHitP = hitP.add(reflectedRay.times(reflectT));
                int x = fabs(planeHitP.getElement(0));
                int y = fabs(planeHitP.getElement(2));

                x = x%288; // need to get this info from texture_image
                y = y%288; // need to get this info from texture_image
                
                surfaceColor = Vec3(texture_image[x][y][0], texture_image[x][y][1], texture_image[x][y][2]);
            
                reflectionColor = reflectionColor.times(0.8).add(surfaceColor.times(0.2));
        
            }
            
            Vec3 refractionColor = Vec3(0, 0, 0);
            float fresnelEffect = 1;
            
            if (hitSphere->getTrans() > 0) {
                float ior = 1.1;
                float eta = (inside) ? ior : 1 / ior;   //this is n/nt
                float cosi = normalizedN.times(-1).dot(d.unit());
                float cosiIn;
                if ((1 - eta * eta * (1 - cosi * cosi)) >0) {
                    cosiIn = sqrt(1 - eta * eta * (1 - cosi * cosi)); //this is cosi'
                } else {
                    return Vec3(0, 0, 0);
                }
                
                
                Vec3 refractionDirIn = (d.add(normalizedN.times(cosi))).times(eta).diff(normalizedN.times(cosiIn)).unit();
                Vec3 originIn = hitP.diff(normalizedN.times(bias));
                
                //ray inside sphere
                float tInside;
                if (hitSphere->intersect(originIn, refractionDirIn, &tInside)){
                    Vec3 hitPInside = originIn.add(refractionDirIn.times(tInside));
                    Vec3 nInside = hitPInside.diff(hitSphere->getCenter());
                    Vec3 normalizedNInside = nInside.unit().times(-1);
                    eta = ior;
                    cosi = normalizedNInside.times(-1).dot(refractionDirIn.unit());
                    if ((1 - eta * eta * (1 - cosi * cosi)) >0) {
                        cosiIn = sqrt(1 - eta * eta * (1 - cosi * cosi)); //this is cosi'
                    } else {
                        return Vec3(0, 0, 0);
                    }

                    
                    Vec3 refractionDirOut = (refractionDirIn.add(normalizedNInside.times(cosi))).times(eta).diff(normalizedNInside.times(cosiIn)).unit();
                    
                    Vec3 originOut = hitPInside.diff(normalizedNInside.times(bias));
                    
                    float tOutside = INFINITY;
                    float t0 = INFINITY;
                    Sphere* refraSphere;
                    
                    for (int k = 0; k<size; k++) {
                        if (sphereList[k]->intersect(originOut, refractionDirOut, &t0)) {
                            if (t0 < tOutside) {
                                tOutside = t0;
                                refraSphere = (Sphere*)sphereList[k];
                            }
                        }
                    }
                    if (tOutside != INFINITY) {
                        refractionColor = refraSphere->getSurfaceColor();
                    } else {
                        refractionColor = Vec3(0, 0, 0);
                    }
                } else {
                    refractionColor = Vec3(0, 0, 0);
                }
                fresnelEffect = 0.4;            //hard code reflection and refraction ratio

            }
            return reflectionColor.times(fresnelEffect).add(refractionColor.times(1-fresnelEffect));

            
        } else {    //diffuse/max depth -> no ray tracing any further
        
            Light l = Light(Vec3(200, 200, 255), Vec3(100, 100, 255),Vec3(255, 255, 255) ,Vec3(200, 70, 50));

            Vec3 LightColor = hitSphere->getLightAt(d, hitP, l);
            
            return LightColor;
            
        }
        
    }
    // the ray doesn't intersect any object in the list
    else {
//        drawing the checkerboard plane, doesn't fit in this recursion
//        Vec3 planeA = Vec3(0, -200, 0);
//        Vec3 planeN = Vec3(0, 1, 0.1);
//        if (d.dot(planeN) != 0) {    //now set reflection to plane (hard code)
//            float tForPlaneHit = ((planeA.diff(origin).dot(planeN))/(d.dot(planeN)));
//            if (tForPlaneHit > 0) {
//                Vec3 planeHitP = origin.add(d.times(tForPlaneHit));
//                Vec3 planeP = planeHitP.diff(planeA);
//                int x = (planeP.getElement(0));
//                int y = sqrt(pow(planeP.getElement(1), 2) + pow(planeP.getElement(2), 2));
//                //int z = (planeHitP.getElement(2));
//                
//                if (abs(x) < 288*2 && abs(y) < 288*2){
//                    x = fabs(x);
//                    y = fabs(y);
//                    x = x%288;
//                    y = y%288;
//                    return Vec3(texture_image[x][y][0], texture_image[x][y][1], texture_image[x][y][2]);
//                }
//            }
//        }

        
        // doesn't hit anything, return default black background color
        return Vec3(0, 0, 0);
    }

}


bool RayTracer::render(int objName, const char* filePath, ImageIO* texture){
    texture_image = texture->getImage();
    float*** image = myImage->getImage();
    
    // orthographical camera with 100*100 pixels
    // shoot a ray in d=(0,0,-1) direction from each of the pixels
    Vec3 d = Vec3(0, 0, -1);
    int sphereSize = 3;
    TObject** objectList = new TObject*[sphereSize];
    objectList[0] = new Sphere(Vec3(0, 0, -150), 90, Vec3(0, 1, 0), Vec3(0, 0, 1), 0.2, 0);
    objectList[1] = new Sphere(Vec3(-73, -50, -30), 30, Vec3(0, 0, 1), Vec3(0, 0, 1), 0.2, 0.3);
    objectList[2] = new Sphere(Vec3(100, 100, -80), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.2, 0);
    
    // ray trace every single pixel for the chosen type of projection
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            Vec3 o = Vec3(j-width/2,height/2-i,0);
            Vec3 color = trace(o, d, objectList, sphereSize, 0);
                
            image[i][j][0] = color.getElement(0);
            image[i][j][1] = color.getElement(1);
            image[i][j][2] = color.getElement(2);
        }
    }
    myImage->writeImage(filePath);
    
    for (int i=0; i<sphereSize; i++){
        delete objectList[i];
    }
    return true;
}

int RayTracer::getWidth(){
    return width;
}

int RayTracer::getHeight(){
    return height;
}

float*** RayTracer::getImage(){
    return myImage->getImage();
}