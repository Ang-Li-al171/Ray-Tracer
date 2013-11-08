//
//  Light.h
//  Ray Tracer
//
//  Created by Ang Li on 11/8/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__Light__
#define __Ray_Tracer__Light__

#include <iostream>
#include "Vec3.h"

using namespace std;

class Light{
    
public:
    Light(void);
    Light(const Vec3 &diffuse, const Vec3 &ambient, const Vec3 &specular, const Vec3 &lightsource);
    ~Light(void);
    Vec3* getDiffuse(void);
    Vec3* getAmbient(void);
    Vec3* getSpecular(void);
    Vec3* getLightSource(void);
    
private:
    Vec3 diffuseLightColor;
    Vec3 ambientLightColor;
    Vec3 specularLightColor;
    Vec3 lightSource;
};

#endif /* defined(__Ray_Tracer__Light__) */
