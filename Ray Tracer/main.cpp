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
    
    glutPostRedisplay();
    
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
    std::cout << "Current directory is: " << getcwd( buffer, 200) << "\n";
    std::string file = std::string(buffer) + fileName;
    const char * filePath = file.c_str();
    return filePath;
}

int main(int argc, char * argv[])
{
    
    ImageIO * texture = new ImageIO(appendWithCWD("/checkerboard_red.ppm"));
    
    RayTracer trial1 = RayTracer();
    trial1.render(1, appendWithCWD("/testTraceSphere.ppm"), texture);
    
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
