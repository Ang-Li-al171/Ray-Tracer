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
#include <GLUT/glut.h>

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

bool RayTracer::render(int objName, const char* filePath, ImageIO* texture){
    
    float*** texture_image = texture->getImage();
    
    // ray tracing a sphere center at (0, 0, -100) with radius 100
    
    // light source position Vec3
    
    Vec3 lightS = Vec3(100, 50, 50);
    Vec3 Blue = Vec3(100, 100, 255);
    Vec3 LightBlue = Vec3(200, 200, 255);
    Vec3 White = Vec3(255, 255, 255);
    float Diffuse = 1.0;
    float Ambient = 0.3;
    float Specular = 0.8;
    
    // define a plane parallel to x-z plane
    
    Vec3 planeA = Vec3(0, -200, 0);
    Vec3 planeN = Vec3(0, 1, 0);
    
    float*** image = myImage->getImage();
    
    if (objName == 1){
        // orthographical camera with 100*100 pixels
        // shoot a ray in d=(0,0,-1) direction from each of the pixels
        
        float R = 100;
        Vec3 c = Vec3(0, 0, -100);
        Vec3 d = Vec3(0, 0, -1);
        
        
        for (int i=0;i<height;i++){
            for (int j=0;j<width;j++){
                Vec3 o = Vec3(j-width/2,height/2-i,0);
                
                //check determinant for hitting the sphere
                float D = powf(d.dot(o.diff(c)), 2)-(d.dot(d)*(o.diff(c).dot(o.diff(c))-R*R));
                if (D>0){
                    // ray hits the sphere
                    float t = (-d.dot(o.diff(c))-sqrt(D))/d.dot(d);
                    // doesn't interact with positive half-ray
                    if (t<0) return false;
                    
                    Vec3 hitP = o.add(d.times(t));
                    Vec3 l = lightS.diff(hitP);
                    Vec3 n = hitP.diff(c);
                    Vec3 h = d.times(-1).add(l.unit()).unit();
                    
                    Vec3 LDiffuse = LightBlue.times(Diffuse).times(n.unit().dot(l.unit()));
                    
                    Vec3 LSpecular = Vec3(0, 0, 0);
                    if (n.unit().dot(h)>0)
                    LSpecular = White.times(Specular).times(pow(n.unit().dot(h), 70));
                    
                    image[i][j][0] = (LDiffuse.getElement(0)/255>0?LDiffuse.getElement(0)/255:0) + Blue.times(Ambient).getElement(0)/255 + LSpecular.getElement(0)/255;
                    image[i][j][1] = (LDiffuse.getElement(1)/255>0?LDiffuse.getElement(1)/255:0) + Blue.times(Ambient).getElement(1)/255 + LSpecular.getElement(1)/255;
                    image[i][j][2] = (LDiffuse.getElement(2)/255>0?LDiffuse.getElement(2)/255:0) + Blue.times(Ambient).getElement(2)/255 + LSpecular.getElement(2)/255;
                    
                    
                    Vec3 normalizedN = n.unit();
                    Vec3 reflectedRay = d.diff(normalizedN.times(2).times(d.dot(normalizedN)));
                    
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
                                
                                image[i][j][0] = image[i][j][0]*0.90 + texture_image[x][y][0]*0.10;
                                image[i][j][1] = image[i][j][1]*0.90 + texture_image[x][y][1]*0.10;
                                image[i][j][2] = image[i][j][2]*0.90 + texture_image[x][y][2]*0.10;
                            }
                        }
                    }
                }
            }
        }
        myImage->writeImage(filePath);
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