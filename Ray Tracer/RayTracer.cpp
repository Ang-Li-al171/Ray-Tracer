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
    // ray tracing a sphere center at (0, 0, -100) with radius 100
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
                Vec3 o = Vec3(i-height/2,j-height/2,0);
                
                //check discriminant for hitting the sphere
                float D = powf(d.dot(o.diff(c)), 2)-(d.dot(d)*(o.diff(c).dot(o.diff(c))-R*R));
                if (D>0){
                    // hit the sphere
                    image[i][j][0]=((float) i)/((float) height);
                    image[i][j][1]=((float) j)/((float) width);
                    image[i][j][2]=((float) i+j)/((float) width+height);
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