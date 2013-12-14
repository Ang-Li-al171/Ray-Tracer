//
//  main.cpp
//  C++_2
//
//  Created by Ang Li on 10/18/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include <unistd.h>
#include <iostream>
#include <math.h>
#include "ImageIO.h"
#include "Texture.h"
#include "Filter.h"
#include "RayTracer.h"
#include "Vec3.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

float win_width = 512;
float win_height = 512;

ImageIO* current_image;

void init( void ){
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
	
}

void idle( void ){
    
    //glutPostRedisplay();
    
}

void drawImage( void ){
    
    if(current_image != NULL){
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2i(0, 0);
        glDrawPixels(current_image->getWidth(), current_image->getHeight(), GL_RGB,
                     GL_FLOAT, current_image->getImageDisplayArray());
    }
    
}

void display( void )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0, win_width, 0, win_height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawImage();
    
    glFlush();
}

void reshape( int w, int h )
{
    win_width = w;
    win_height = h;
    glViewport( 0, 0, win_width, win_height );
    
    glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) {
    switch(key) {
        case 27: // Escape key
            exit(0);
            break;
    }
}

const char* appendWithCWD(const char* fileName){
    char buffer[200];
    getcwd( buffer, 200);
    std::string file = std::string(buffer) + fileName;
    const char * filePath = file.c_str();
    return filePath;
}

int main(int argc, char * argv[])
{
    
    // start reading data in from a file
    FILE* file;
    int projectionType = 1;
    file = fopen(appendWithCWD("/dataIn.txt"), "r");
    if(!file){
        fprintf(stderr, "Unable to open file %s", appendWithCWD("/dataIn.txt"));
        exit(1);
    }
    
    // read in objection type and 
    char buff[32];
    int x=0, y=0, z=0;
    fscanf(file, "%s", buff);
    if (strcmp(buff, "Perspective") == 0){
        projectionType = 0;
        fscanf(file, "%d %d %d", &x, &y, &z);
    }
    
    Vec3 eyeLocation = Vec3(x, y, z);
    
    // size of list of objects
    int objListSize = 0;
    fscanf(file, "%d", &objListSize);
    
    TObject** objectList = new TObject*[objListSize];
    ImageIO** texList = new ImageIO*[objListSize];
    int index = 0;
    
    // each line for an object from now on
    while(index < objListSize){
        char objName[32];
        fscanf(file, "%s", objName);
        
        if (strcmp(objName, "Sphere") == 0){
            int ca, cb, cc, r, shadow;
            float refl, refr;
            char texName[32];
            fscanf(file, "%d %d %d %d %f %f %s %d",
                   &ca, &cb, &cc, &r, &refl, &refr, texName, &shadow);
            
            bool shadowB = shadow == 1 ? true: false;
            texList[index] = new ImageIO(appendWithCWD(texName));
            objectList[index] = new Sphere(Vec3(ca, cb, cc), r, Vec3(0,0,0), Vec3(0,0,0),
                                           refl, refr, texList[index]->getImage(),
                                           texList[index]->getWidth(), texList[index]->getHeight(),
                                           shadowB);
        }
        
        else if (strcmp(objName, "Plane") == 0){
            int ca, cb, cc, na, nb, nc, xa, xb, xc, ya, yb, yc, shadow;
            float refl, refr;
            char texName[32];
            
            fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %f %f %s %d",
                   &ca, &cb, &cc, &na, &nb, &nc, &xa, &xb, &xc,
                   &ya, &yb, &yc, &refl, &refr, texName, &shadow);
            bool shadowB = shadow == 1 ? true: false;
            
            texList[index] = new ImageIO(appendWithCWD(texName));
            objectList[index] = new Plane(Vec3(ca, cb, cc), Vec3(na, nb, nc),
                                          Vec3(xa, xb, xc), Vec3(ya, yb, yc),
                                          texList[index]->getWidth(), texList[index]->getHeight(),
                                          Vec3(0, 0, 0), Vec3(0, 0, 0),
                                          refl, refr, texList[index]->getImage(),
                                          shadowB);
        }
        
        else if (strcmp(objName, "Cylinder") == 0){
            int ca, cb, cc, r, h, da, db, dc, shadow;
            float refl, refr;
            char texName[32];
            fscanf(file, "%d %d %d %d %d %d %d %d %f %f %s %d", &ca, &cb, &cc, &r, &h, &da, &db, &dc, &refl, & refr, texName, &shadow);
            bool shadowB = shadow == 1 ? true: false;
            
            texList[index] = new ImageIO(appendWithCWD(texName));
            objectList[index] = new Cylinder(Vec3(ca, cb, cc), r, h, Vec3(da, db, dc),
                                             Vec3(0, 0, 0), Vec3(0, 0, 0),
                                             refl, refr, new Texture(texList[index]),
                                             shadowB);
        }
        
        index++;
        memset(objName, 0, 32);
    }
    
    fclose(file);
    
    // apply ray tracing and write output image to a file
    RayTracer* trial1 = new RayTracer();
    trial1->render(projectionType, eyeLocation,
                   appendWithCWD("/TracedImage.ppm"),
                   objectList, objListSize,
                   0, 5, 400, true);
    
    // read the ray traced image back and display it
    ImageIO * the_image = new ImageIO(appendWithCWD("/TracedImage.ppm"));
    current_image = the_image;
    
    win_height = current_image->getHeight();
    win_width = current_image->getWidth();
    
    glutInit( &argc, argv );
    
    glutInitDisplayMode( GLUT_RGB );
    glutInitWindowSize( win_width, win_height );
	
    glutCreateWindow( "Image" );
    
    init();
	
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( idle );
    
    
    glutMainLoop();
    
    delete the_image;
    delete current_image;
    for (int i=0; i<objListSize; i++){
        delete objectList[i];
        delete texList[i];
    }
    delete trial1;
    return 0;
}
