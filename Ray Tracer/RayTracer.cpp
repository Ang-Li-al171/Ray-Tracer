//
//  RayTracer.cpp
//  Ray Tracer
//
//  Created by Ang Li on 11/1/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "RayTracer.h"


enum{PERSPECTIVE, ORTHOGRAPHIC};

RayTracer::RayTracer(){
    width = 1024;
    height = 768;
    max = 255;
    MAX_RAY_DEPTH = 5;
    myImage = new ImageIO(width, height, max);
    // other initiation in constructor?
}

RayTracer::~RayTracer(){
    delete myImage;
}

Vec3 RayTracer::trace(Vec3 origin, Vec3 d, TObject** objList, int size, int depth, int* outsideT) {
    
    // make sure d is normalized!
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
    
    if (outsideT != NULL)
        *outsideT = tNear;
    
    // if there's an intersection with an object
    if (tNear < INFINITY) {
        
        // epsilon value used to a point slightly away from a plane
        float bias = 1e-3;
        
        Vec3 hitP = origin.add(d.times(tNear));
        
        
        // color from light and surface
        Light l = Light(Vec3(255, 255, 255), Vec3(100, 100, 100),
                        Vec3(255, 255, 255) ,Vec3(10, 170, 100).times(3));
        Vec3 LightColor = hitObj->getLightAt(d, hitP, l);

        
        // if path to light source blocked, use shadow color
        for (int i = 0; i<size; i++) {
            float t0;
            if (objList[i]->causeShadow() && objList[i]->intersect(hitP.add(hitObj->getN(hitP, d).times(bias)), l.getLightSource().diff(hitP), &t0)) {
                // need a better shadowing algo for transparent objects here
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
                                         objList, size, depth+1, NULL);
            
        }
        
        // color from refraction recursive
        Vec3 refractionColor = Vec3(0, 0, 0);
        if (depth < MAX_RAY_DEPTH && (hitObj->getTrans() > 0)){
            
            Vec3 n = hitObj->getN(hitP, d);
            
            bool inside = hitObj->rayInside(hitP, d);
            float nNT = 1.0/2.0;   //this is n/nt
            float eta = (inside) ? 1.0/nNT : nNT;
            float cosi = n.times(-1.0).dot(d.unit());
            float cosiIn;
            
            if ((1.0 - eta * eta * (1.0 - cosi * cosi)) >0) {
                
                cosiIn = sqrt(1.0 - eta * eta * (1.0 - cosi * cosi));
                
                Vec3 originIn = hitP.diff(n.times(bias));
                
                Vec3 refractionDirIn = (d.add(n.times(cosi))).times(eta).diff(n.times(cosiIn)).unit();
                
                refractionColor = trace(originIn, refractionDirIn, objList, size, depth+1, NULL);
                
            } else if (inside){
                // need to fix total internal reflection
                Vec3 n = hitObj->getN(hitP, d);
                
                Vec3 reflectedRay = d.diff(n.times(2.0).times(d.dot(n)));
                
                refractionColor = trace(hitP.add(n.times(bias)), reflectedRay,
                                        objList, size, depth+1, NULL);
                
                // if rays didn't come out of the object due to total internal reflection
                if (depth == MAX_RAY_DEPTH-1){
                    printf("Stuck inside");
                }
            }
            else{
                // for debugging purposes, color it red!
                refractionColor = Vec3(255, 0, 0);
            }
        }
        
        return LightColor.times(1.0-hitObj->getRefl()-hitObj->getTrans()).add(reflectionColor.times(hitObj->getRefl())).add(refractionColor.times(hitObj->getTrans()));
    }
        
    // doesn't hit anything, return default black background color
    return Vec3(0, 0, 0);
}


bool RayTracer::render(int objName, Vec3 eyeLocation, const char* filePath,
                       TObject* objectList[], int numObj,
                       int projectionType, int depth, int depthOfField,
                       bool antiAliasing){
    
    MAX_RAY_DEPTH = depth;
    
    float*** image = myImage->getImage();

    switch (projectionType) {
        case PERSPECTIVE:{
            // this is perspective projection
            
            // camera location
            Vec3 persOrigin = Vec3(eyeLocation);
            
            bool** applyFilter = myImage->getApplyFilter();
            
            for (int i=0;i<height;i++){
                
                for (int j=0;j<width;j++){
                    
                    int t;
                    
                    Vec3 persDir = Vec3(j-((float)width)/2.0, ((float)height)/2.0-i, 0.0).diff(persOrigin);
                    
                    Vec3 color = trace(persOrigin, persDir, objectList, numObj, 0, &t);
                    
                    Vec3 combinedColor;
                    if (antiAliasing){
                        Vec3 persDir2 = Vec3(j-(((float)width)-0.25)/2.0,
                                             (((float)height)-0.25)/2.0-i,
                                             0.0).diff(persOrigin);
                        
                        Vec3 color2 = trace(persOrigin, persDir2, objectList, numObj, 0, &t);
                        
                        Vec3 persDir3 = Vec3(j-(((float)width)+0.25)/2.0,
                                             (((float)height)-0.25)/2.0-i,
                                             0.0).diff(persOrigin);
                        
                        Vec3 color3 = trace(persOrigin, persDir3, objectList, numObj, 0, &t);
                        
                        Vec3 persDir4 = Vec3(j-(((float)width)-0.25)/2.0,
                                             (((float)height)+0.25)/2.0-i,
                                             0.0).diff(persOrigin);
                        
                        Vec3 color4 = trace(persOrigin, persDir4, objectList, numObj, 0, &t);
                        
                        Vec3 persDir5 = Vec3(j-(((float)width)+0.25)/2.0,
                                             (((float)height)+0.25)/2.0-i,
                                             0.0).diff(persOrigin);
                        
                        Vec3 color5 = trace(persOrigin, persDir5, objectList, numObj, 0, &t);
                        
                        combinedColor = Vec3(color.times(0.20).add(color2.times(0.20)).add(color3.times(0.20)).add(color4.times(0.20)).add(color5.times(0.20)));
                    }
                    
                    if (t<depthOfField){
                        applyFilter[i][j]=false;
                    }
                    
                    if (antiAliasing){
                        image[i][j][0] = combinedColor.getElement(0);
                        image[i][j][1] = combinedColor.getElement(1);
                        image[i][j][2] = combinedColor.getElement(2);
                    } else{
                        image[i][j][0] = color.getElement(0);
                        image[i][j][1] = color.getElement(1);
                        image[i][j][2] = color.getElement(2);
                    }
                }
            }
            break;
        }
        case ORTHOGRAPHIC:{
            // ray trace every single pixel for the chosen type of projection
            // this is orthographic projection
            
            // orthographical camera with 100*100 pixels
            // shoot a ray in d=(0,0,-1) direction from each of the pixels
            Vec3 orthoDir = Vec3(0, 0, -1);
            
            bool** applyFilter = myImage->getApplyFilter();
            
            for (int i=0;i<height;i++){
                
                for (int j=0;j<width;j++){
                    int t;
                    Vec3 orthoOrigin = Vec3(j-width/2,height/2-i,0);
                    Vec3 color = trace(orthoOrigin, orthoDir, objectList, numObj, 0, &t);
                    
                    if (t<depthOfField){
                        applyFilter[i][j]=false;
                    }
                    
                    image[i][j][0] = color.getElement(0);
                    image[i][j][1] = color.getElement(1);
                    image[i][j][2] = color.getElement(2);
                }
            }
            break;
        }
        default:{
            break;
        }
    }
    
//    myImage = myImage->blur(new Filter(TENT, 10), NULL);
    myImage->writeImage(filePath);
    
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