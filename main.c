#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "chess.h"

void setColor(char c){
  switch( c ){
    case '_': glColor3f(1.0f, 1.0f, 1.0f); break; // white
    case '=': glColor3f(0.5f, 0.5f, 0.5f); break; // gray
    case '.': glColor3f(1.0f, 1.0f, 1.0f); break; // white
    case '@': glColor3f(0.0f, 0.0f, 0.0f); break; // black
    case '#': glColor3f(0.2f, 0.2f, 0.2f); break; // dark gray
    default: glColor3f(0.0f, 1.0f, 0.0f); break; // black
  }
}

void interpreter(char** fig){
  glClear( GL_COLOR_BUFFER_BIT); // Clear the color buffer with current clearing color
  glBegin(GL_QUADS); // Each set of 4 vertices form a quad

  int j = 0;
  float pixel = 0.211;
  while(fig[j]){
    int i = 0;
    while(fig[j][i]){
      setColor(fig[j][i]);
      float x = 1.0 + i * pixel;
      float y = 1.0 + j * pixel;
      glVertex2f(x, y); 
      glVertex2f(x + pixel, y); 
      glVertex2f(x + pixel, y + pixel); 
      glVertex2f(x, y + pixel);
      i++;
    }
    j++;
  }
 
  glEnd(); 
  glFlush(); 
}

int main(int argc, char **argv) { 
  glutInit(&argc, argv); 
  glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowPosition(100,100); //origin on the window system
  glutInitWindowSize(300,300);  // window´s size
  glutCreateWindow ("Chess");

  glClearColor(0.5, 0.35, 0.05, 0.0);         // black background 
  glMatrixMode(GL_PROJECTION);              // setup viewing projection 
  glLoadIdentity();                           // start with identity matrix 
  glOrtho(0.0, 100.0, 100.0, 0.0, -1.0, 1.0);   // setup a 100x100x2 viewing world

  glutDisplayFunc(display); 
  glutMainLoop();

  return 0; 
}


