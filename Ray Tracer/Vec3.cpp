//
//  Vec3.cpp
//  Ray Tracer
//
//  Created by Ang Li on 11/1/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "Vec3.h"

Vec3::Vec3(void){
    elements[0] = 0;
    elements[1] = 0;
    elements[2] = 0;
}

Vec3::Vec3(float x, float y, float z){
    elements[0]=x;
    elements[1]=y;
    elements[2]=z;
}

Vec3::Vec3(const Vec3 &v){
    elements[0] = v.elements[0];
    elements[1] = v.elements[1];
    elements[2] = v.elements[2];
}

Vec3::~Vec3(){
    //empty destructor
}

float Vec3::getElement(int index){
    return elements[index];
}


Vec3 Vec3::add(Vec3 v){
    return Vec3(getElement(0)+v.getElement(0),
                getElement(1)+v.getElement(1),
                getElement(2)+v.getElement(2));
}

Vec3 Vec3::diff(Vec3 v){
    return Vec3(getElement(0)-v.getElement(0),
                getElement(1)-v.getElement(1),
                getElement(2)-v.getElement(2));
}

Vec3 Vec3::times(float multiplier){
    return Vec3(getElement(0)*multiplier,
                getElement(1)*multiplier,
                getElement(2)*multiplier);
}

float Vec3::dot(Vec3 v){
    return getElement(0)*v.getElement(0)+getElement(1)*v.getElement(1)+getElement(2)*v.getElement(2);
}

Vec3 Vec3::unit(){
    return Vec3(getElement(0), getElement(1), getElement(2)).times(1/length());
}

Vec3 Vec3::cross(Vec3 v){
    //haven't implemented yet
    return Vec3(0,0,0);
}

float Vec3::length(){
    return sqrt(getElement(0)*getElement(0)+getElement(1)*getElement(1)+getElement(2)*getElement(2));
}