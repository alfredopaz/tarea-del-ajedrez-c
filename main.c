#include "chess.h"
#include "chess_internal.h"
#include "gc.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef CHESS_HEADLESS
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#else
static const char* outputPath = "chess.ppm";
static int outputStatus = EXIT_FAILURE;
#endif

static unsigned char shade(char c){
  switch(c){
    case '_': case '.': return 255;
    case '@': return 0;
    case '#': return 51;
    default: return 128;
  }
}

void interpreter(char** fig){
  size_t rows, cols;
  if(!chessDimensions(fig, &rows, &cols)){
    fprintf(stderr, "No se puede dibujar: imagen nula, vacía o irregular.\n");
    return;
  }
#ifdef CHESS_HEADLESS
  FILE* output = fopen(outputPath, "wb");
  if(!output){
    perror(outputPath);
    return;
  }
  int failed = fprintf(output, "P6\n%zu %zu\n255\n", cols, rows) < 0;
  for(size_t i = 0; i < rows && !failed; i++){
    for(size_t j = 0; j < cols && !failed; j++){
      unsigned char value = shade(fig[i][j]);
      unsigned char rgb[] = {value, value, value};
      failed = fwrite(rgb, 1, sizeof rgb, output) != sizeof rgb;
    }
  }
  if(fclose(output) != 0)
    failed = 1;
  if(failed)
    fprintf(stderr, "No se pudo escribir la imagen completa.\n");
  else
    outputStatus = EXIT_SUCCESS;
#else
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /* Ajuste al tamaño de la imagen conservando la proporción de sus píxeles. */
  double width = (double)cols;
  double height = (double)rows;
  int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
  int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
  double aspect = windowHeight > 0 ? (double)windowWidth / windowHeight : 1.0;
  if(width / height < aspect)
    width = height * aspect;
  else
    height = width / aspect;
  glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glBegin(GL_QUADS);
  for(size_t i = 0; i < rows; i++){
    for(size_t j = 0; j < cols; j++){
      unsigned char value = shade(fig[i][j]);
      glColor3ub(value, value, value);
      double x = (double)j, y = (double)i;
      glVertex2d(x, y);
      glVertex2d(x + 1.0, y);
      glVertex2d(x + 1.0, y + 1.0);
      glVertex2d(x, y + 1.0);
    }
  }
  glEnd();
  glutSwapBuffers();
#endif
}

/* El alumno solo construye y dibuja; el motor gestiona la vida de las imágenes. */
static void render(void){
  display();
  garbageCollector();
}

#ifndef CHESS_HEADLESS
static void reshape(int width, int height){
  glViewport(0, 0, width, height);
  glutPostRedisplay();
}
#endif

int main(int argc, char** argv){
  if(atexit(garbageCollector) != 0){
    fprintf(stderr, "No se pudo registrar la limpieza de memoria.\n");
    return EXIT_FAILURE;
  }
#ifdef CHESS_HEADLESS
  if(argc > 2){
    fprintf(stderr, "Uso: %s [salida.ppm]\n", argv[0]);
    return EXIT_FAILURE;
  }
  if(argc == 2)
    outputPath = argv[1];
  render();
  return outputStatus;
#else
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(640, 640);
  glutCreateWindow("Chess");
  glClearColor(0.5f, 0.35f, 0.05f, 1.0f);
  glutDisplayFunc(render);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return EXIT_SUCCESS;
#endif
}
