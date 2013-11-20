//
//  Light.cpp
//  Ray Tracer
//
//  Created by Ang Li on 11/8/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "Light.h"

Light::Light(void){}

Light::Light(const Vec3 &diffuse, const Vec3 &ambient, const Vec3 &specular, const Vec3 &lightsource){
    diffuseLightColor = Vec3(diffuse);
    ambientLightColor = Vec3(ambient);
    specularLightColor = Vec3(specular);
    lightSource = Vec3(lightsource);
}

Light::~Light(void){
    diffuseLightColor.~Vec3();
    ambientLightColor.~Vec3();
    specularLightColor.~Vec3();
}

Vec3 Light::getDiffuse(void){
    return diffuseLightColor;
}

Vec3 Light::getAmbient(void){
    return ambientLightColor;
}

Vec3 Light::getSpecular(void){
    return specularLightColor;
}

Vec3 Light::getLightSource(void){
    return lightSource;
}