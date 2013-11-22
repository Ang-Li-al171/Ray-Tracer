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
    
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(80, GLfloat(win_width)/win_height, 1, 40);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
//    glClear(GL_COLOR_BUFFER_BIT);
    
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
    
    // the checkerboard image used for texturing
    ImageIO * texture = new ImageIO(appendWithCWD("/WoodenFloor.ppm"));
    
    
    int objListSize = 6;
    
    TObject** objectList = new TObject*[objListSize];
    
    objectList[0] = new Sphere(Vec3(0, 0, -150), 90, Vec3(0, 1, 0), Vec3(0, 0, 1), 1.0, 0);
    objectList[1] = new Sphere(Vec3(-20, -150, 0), 30, Vec3(0, 0, 1), Vec3(0, 0, 1), 0.0, 0.8);
    objectList[2] = new Sphere(Vec3(160, 50, -80), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.1, 0);
    objectList[3] = new Sphere(Vec3(-80, -80, -80), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.1, 0);
    objectList[4] = new Sphere(Vec3(120, -80, -120), 50, Vec3(1, 1, 0), Vec3(0, 0, 1), 0.3, 0);
    objectList[5] = new Plane(Vec3(0, -200, 0), Vec3(0, 1, 0.1),
                              Vec3(1, 0, 0), Vec3(0, 0.1, -1),
                              texture->getWidth(), texture->getHeight(),
                              Vec3(0, 0, 0), Vec3(0, 0, 0),
                              0.0, 0.0, texture->getImage());
    
    // apply ray tracing and write output image to a file
    RayTracer trial1 = RayTracer();
    trial1.render(1, appendWithCWD("/testTraceSphere.ppm"), objectList, objListSize, 0, 5);
    
    // read the ray traced image back and display it
    ImageIO * the_image = new ImageIO(appendWithCWD("/testTraceSphere.ppm"));
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
    
    return 0;
}
