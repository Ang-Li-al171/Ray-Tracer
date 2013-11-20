//
//  RayTracer.cpp
//  Ray Tracer
//
//  Created by Ang Li on 11/1/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "RayTracer.h"

int MAX_RAY_DEPTH = 5;
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

Vec3 RayTracer::trace(Vec3 origin, Vec3 d, TObject** objList, int size, int depth) {
    
    d = d.unit();
    
    //take record of smallest t, for the nearest object in the direction of the ray
    float t = INFINITY;
    float tNear = INFINITY;
    TObject* hitObj;
    for (int i = 0; i<size; i++) {
        if (objList[i]->intersect(origin, d, &t)) {
            if (t < tNear){
                tNear = t;
                hitObj = objList[i];
            }
        }
    }
    
    // if there's an intersection with an object
    if (tNear < INFINITY) {
        
        // epsilon value used to a point slightly away from a plane
        float bias = 1e-3;
        
        Vec3 hitP = origin.add(d.times(tNear));
        
        
        // color from light and surface
        
        Light l = Light(Vec3(181, 126, 220), Vec3(100, 100, 100),
                        Vec3(255, 255, 255) ,Vec3(100, 170, 100));
        Vec3 LightColor = hitObj->getLightAt(d, hitP, l);

        
        // if path to light source blocked, use shadow color
        for (int i = 0; i<size; i++) {
            float t0;
            if (objList[i]->intersect(hitP.add(hitObj->getN(hitP, d).times(bias)),
                                      l.getLightSource().diff(hitP), &t0)) {
                // need a better shadowing algo for transparent objects
                //LightColor = l.getAmbient().times(1.0/255.0).times(0.10).add(LightColor.times(objList[i]->getTrans()));
                LightColor = l.getAmbient().times(1.0/255.0).times(0.30);
                break;
            }
        }
        
        // color from reflection, recursive
        Vec3 reflectionColor = Vec3(0, 0, 0);
        if (depth < MAX_RAY_DEPTH && (hitObj->getRefl() > 0)){
            
            Vec3 n = hitObj->getN(hitP, d);
            
            Vec3 reflectedRay = d.diff(n.times(2.0).times(d.dot(n)));
            
            reflectionColor = trace(hitP.add(n.times(bias)), reflectedRay,
                                         objList, size, depth+1);
            
        }
        
        // color from refraction recursive
        Vec3 refractionColor = Vec3(0, 0, 0);
        if (depth < MAX_RAY_DEPTH && (hitObj->getTrans() > 0)){
            
            Vec3 n = hitObj->getN(hitP, d);
            
            bool inside = hitObj->rayInside(hitP, d);
            float nNT = 1.0/1.6;   //this is n/nt
            float eta = (inside) ? 1.0/nNT : nNT;
            float cosi = n.times(-1.0).dot(d.unit());
            float cosiIn;
            if ((1.0 - eta * eta * (1.0 - cosi * cosi)) >0) {
                
                cosiIn = sqrt(1.0 - eta * eta * (1.0 - cosi * cosi)); //this is cosi'
                
                Vec3 originIn = hitP.diff(n.times(bias));
                
                Vec3 refractionDirIn = (d.add(n.times(cosi))).times(eta).diff(n.times(cosiIn)).unit();
                
                refractionColor = trace(originIn, refractionDirIn, objList, size, depth+1);
                
            } else if (inside){
                // need to fix total internal reflection
                Vec3 n = hitObj->getN(hitP, d);
                
                Vec3 reflectedRay = d.diff(n.times(2.0).times(d.dot(n)));
                
                refractionColor = trace(hitP.add(n.times(bias)), reflectedRay,
                                        objList, size, depth+1);
                
                // if rays didn't come out of the object due to total internal reflection
                if (depth == MAX_RAY_DEPTH-1){
                    printf("Stuck inside");
                }
            }
            else{
                // for debugging purposes
                refractionColor = Vec3(255, 0, 0);
            }
        }
        
        return LightColor.times(1.0-hitObj->getRefl()-hitObj->getTrans()).add(reflectionColor.times(hitObj->getRefl())).add(refractionColor.times(hitObj->getTrans()));
    }
        
    // doesn't hit anything, return default black background color
    return Vec3(0, 0, 0);
}


bool RayTracer::render(int objName, const char* filePath, ImageIO* texture){
    
    texture_image = texture->getImage();
    float*** image = myImage->getImage();
    
    // orthographical camera with 100*100 pixels
    // shoot a ray in d=(0,0,-1) direction from each of the pixels
    Vec3 d = Vec3(0, 0, -1);
    
    int objListSize = 6;
    
    TObject** objectList = new TObject*[objListSize];
    
    objectList[0] = new Sphere(Vec3(0, 0, -150), 90, Vec3(0, 1, 0), Vec3(0, 0, 1), 0.5, 0);
    objectList[1] = new Sphere(Vec3(-20, -150, 0), 30, Vec3(0, 0, 1), Vec3(0, 0, 1), 0.0, 0.8);
    objectList[2] = new Sphere(Vec3(160, 50, -80), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.1, 0);
    objectList[3] = new Plane(Vec3(0, -200, 0), Vec3(0, 1, 0.1), 288*2, 288*2,
                              Vec3(0, 0, 0), Vec3(0, 0, 0), 0.0, 0.0, texture_image);
    objectList[4] = new Sphere(Vec3(-80, -80, -80), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.1, 0);
    objectList[5] = new Sphere(Vec3(120, -80, -120), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.3, 0);
    
    // ray trace every single pixel for the chosen type of projection
    // this is orthographic projection
//    for (int i=0;i<height;i++){
//        for (int j=0;j<width;j++){
//            Vec3 o = Vec3(j-width/2,height/2-i,0);
//            Vec3 color = trace(o, d, objectList, objListSize, 0);
//            
//            image[i][j][0] = color.getElement(0);
//            image[i][j][1] = color.getElement(1);
//            image[i][j][2] = color.getElement(2);
//        }
//    }
    
    // this is perspective projection
    Vec3 o = Vec3(0, -50, 200);
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            
            d = Vec3(j-((float)width)/2.0, ((float)height)/2.0-i, 0.0).diff(o);
            
            Vec3 color = trace(o, d, objectList, objListSize, 0);
            
            image[i][j][0] = color.getElement(0);
            image[i][j][1] = color.getElement(1);
            image[i][j][2] = color.getElement(2);
        }
    }
    
    myImage->writeImage(filePath);
    
    for (int i=0; i<objListSize; i++){
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