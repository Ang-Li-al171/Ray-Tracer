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

void RayTracer::render(int objName){
    
    ImageIO * texture = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/Ray Tracer/checkerboard_red.ppm");
    float*** texture_image = texture->getImage();

    
    // ray tracing a sphere center at (0, 0, -100) with radius 100
    
    // light source position Vec3
    
    Vec3 lightS = Vec3(100, 100, 50);
    float Reflec = 1.0;
    Vec3 E = Vec3(100, 100, 255);
    float Ambient = 0.3;

    // define a plane parallel to x-z plane
    
    Vec3 planeA = Vec3(0, -200, 0);
    Vec3 planeN = Vec3(0, 1, 0);
    
    
    if (objName == 1){
        // orthographical camera with 100*100 pixels
        // from (0,0,0) to (99,99,0)
        // shoot a ray in (0,0,-1) direction from each of the pixels
        
        float R = 100;
        Vec3 c = Vec3(0, 0, -100);
        Vec3 d = Vec3(0, 0, -1);
        
        float*** image = myImage->getImage();
        
        for (int i=0;i<height;i++){
            for (int j=0;j<width;j++){
                Vec3 o = Vec3(j-width/2,height/2-i,0);
                
                //check discriminant for hitting the sphere
                float D = powf(d.dot(o.diff(c)), 2)-(d.dot(d)*(o.diff(c).dot(o.diff(c))-R*R));
                if (D>0){
                    // hit the sphere
                    
                    float t = (-d.dot(o.diff(c))-sqrt(D))/d.dot(d);
                    if (t<0)
                        t = (-d.dot(o.diff(c))+sqrt(D))/d.dot(d);
                    
                    Vec3 hitP = o.add(d.times(t));
                    Vec3 l = lightS.diff(hitP);
                    Vec3 n = hitP.diff(c);
                    
                    
                    Vec3 L = E.times(Reflec).times(n.unit().dot(l.unit()));
                    
                    
                    image[i][j][0] = (L.getElement(0)/255>0?L.getElement(0)/255:0) + E.times(Ambient).getElement(0)/255;
                    image[i][j][1] = (L.getElement(1)/255>0?L.getElement(1)/255:0) + E.times(Ambient).getElement(1)/255;
                    image[i][j][2] = (L.getElement(2)/255>0?L.getElement(2)/255:0) + E.times(Ambient).getElement(2)/255;
                    
                    Vec3 normalizedN = n.unit();
                    
                    Vec3 reflection = d.diff(normalizedN.times(2).times(d.dot(normalizedN)));
                    
                    if (reflection.dot(planeN) != 0) {
                        float reflectionT = ((planeA.diff(hitP).dot(planeN))/(reflection.dot(planeN)));
                        if (reflectionT > 0) {
                            Vec3 planeHitP = hitP.add(reflection.times(t));
                            
                            int x = fabs(planeHitP.getElement(0));
                            x = x%288;
                            int y = fabs(planeHitP.getElement(2));
                            y = y%288;
                            
                            image[i][j][0] = image[i][j][0]*0.95 + texture_image[x][y][0]*0.05;
                            image[i][j][1] = image[i][j][1]*0.95 + texture_image[x][y][1]*0.05;
                            image[i][j][2] = image[i][j][2]*0.95 + texture_image[x][y][2]*0.05;

                        }
                        
                    }

                    
                }
            }
        }
        
        myImage->writeImage("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/Ray Tracer/testTraceSphere.ppm");
    }
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