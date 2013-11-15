//
//  RayTracer.cpp
//  Ray Tracer
//
//  Created by Ang Li on 11/1/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "RayTracer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Sphere.h"
#include <GLUT/glut.h>

int MAX_RAY_DEPTH = 2;
float*** texture_image;

RayTracer::RayTracer(){
    width = 512;
    height = 512;
    max = 255;
    myImage = new ImageIO(width, height, max);
    //empty constructor
}

RayTracer::~RayTracer(){
    delete myImage;
}

Vec3 RayTracer::trace(Vec3 origin, Vec3 d, Sphere** sphereList, int size, int depth) {
    //take record of smallest t
    float t = INFINITY;
    float tNear = INFINITY;
    Sphere* hitSphere;
    
    for (int i = 0; i<size; i++) {
        if (sphereList[i]->intersect(origin, d, &t)) {
            if (t < tNear){
                tNear = t;
                hitSphere = sphereList[i];
            }
        }
    }
    
    if (tNear != INFINITY) {
        
        Vec3 surfaceColor = Vec3(0, 0, 0);
        Vec3 hitP = origin.add(d.times(tNear));
        Vec3 n = hitP.diff(hitSphere->getCenter());
        Vec3 normalizedN = n.unit();
        
        
        if (depth < MAX_RAY_DEPTH && (hitSphere->getRefl() > 0 || hitSphere->getTrans() >0)){
            Vec3 reflectedRay = d.diff(normalizedN.times(2).times(d.dot(normalizedN)));
            
            Vec3 planeA = Vec3(0, -200, 0);
            Vec3 planeN = Vec3(0, 1, 0);
            
            float r =hitSphere->getSurfaceColor().getElement(0);
            float g = hitSphere->getSurfaceColor().getElement(1);
            float b = hitSphere->getSurfaceColor().getElement(2);
            
            // if not parallel to the plane
            if (reflectedRay.dot(planeN) != 0) {
                float tForPlaneHit = ((planeA.diff(hitP).dot(planeN))/(reflectedRay.dot(planeN)));
                if (tForPlaneHit > 0) {
                    Vec3 planeHitP = hitP.add(reflectedRay.times(tForPlaneHit));
                    
                    int x = fabs(planeHitP.getElement(0));
                    int y = fabs(planeHitP.getElement(2));
                    
                    if (x < 288*2 && y < 288*2){
                        x = x%288;
                        y = y%288;
                        
                        r = hitSphere->getSurfaceColor().getElement(0)*(1- hitSphere->getRefl()) + texture_image[x][y][0]*(hitSphere->getRefl());
                        g = hitSphere->getSurfaceColor().getElement(1)*(1- hitSphere->getRefl()) + texture_image[x][y][1]*(hitSphere->getRefl());
                        b = hitSphere->getSurfaceColor().getElement(2)*(1- hitSphere->getRefl()) + texture_image[x][y][2]*(hitSphere->getRefl());
                    }
                }
            }
            Vec3 reflectionColor = trace(origin, d, sphereList, size, depth+1);
            return Vec3(r*0.3 + reflectionColor.getElement(0)*0.7, g*0.3 + reflectionColor.getElement(1)*0.7, b*0.3 + reflectionColor.getElement(b)*0.7);
            
        } else {    //diffuse/max depth -> no ray tracing any further
            Vec3 lightS = Vec3(100, 50, 50);
            Vec3 Blue = Vec3(100, 100, 255);
            Vec3 LightBlue = Vec3(200, 200, 255);
            Vec3 White = Vec3(255, 255, 255);
            float Diffuse = 1.0;
            float Ambient = 0.3;
            float Specular = 0.8;

            Vec3 lightDirection = lightS.diff(hitP);
            
            Vec3 h = d.times(-1).add(lightDirection.unit()).unit();
            
            Vec3 LDiffuse = LightBlue.times(Diffuse).times(n.unit().dot(lightDirection.unit()));
            
            Vec3 LSpecular = Vec3(0, 0, 0);
            if (n.unit().dot(h)>0)
                LSpecular = White.times(Specular).times(pow(n.unit().dot(h), 70));
            
            float r = (LDiffuse.getElement(0)/255>0?LDiffuse.getElement(0)/255:0) + Blue.times(Ambient).getElement(0)/255 + LSpecular.getElement(0)/255;
            float g = (LDiffuse.getElement(1)/255>0?LDiffuse.getElement(1)/255:0) + Blue.times(Ambient).getElement(1)/255 + LSpecular.getElement(1)/255;
            float b = (LDiffuse.getElement(2)/255>0?LDiffuse.getElement(2)/255:0) + Blue.times(Ambient).getElement(2)/255 + LSpecular.getElement(2)/255;
            
            return Vec3(r, g, b);
            
        }
        
    } else {
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
    Sphere** sphereList = new Sphere*[sphereSize];
    Sphere* sphere = new Sphere(Vec3(0, 0, -100), 100, Vec3(0, 1, 0), Vec3(0, 0, 1), 0.3, 0);
    Sphere* sphere2 = new Sphere(Vec3(0, -100, -40), 30, Vec3(0, 0, 1), Vec3(0, 0, 1), 0.3, 0);
    Sphere* sphere3 = new Sphere(Vec3(-100, 120, -80), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.3, 0);
    sphereList[0] = sphere;
    sphereList[1] = sphere2;
    sphereList[2] = sphere3;
    
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            Vec3 o = Vec3(j-width/2,height/2-i,0);
            Vec3 color = trace(o, d, sphereList, sphereSize, 1);
                
            image[i][j][0] = color.getElement(0);
            image[i][j][1] = color.getElement(1);
            image[i][j][2] = color.getElement(2);
        }
    }
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