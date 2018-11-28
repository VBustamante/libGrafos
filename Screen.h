//
// Created by vbustamante on 11/27/18.
//

#ifndef LIBGRAFOS_SCREEN_H
#define LIBGRAFOS_SCREEN_H
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "sleep.h"
#include "EuclidianGraph.h"

class Screen {
public:
  explicit Screen(EuclidianGraph *graph);
  ~Screen ();

private:
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;
  GLFWwindow *window = nullptr;
  EuclidianGraph *graph;

  GLuint nodeVBO = 0;
  GLuint nodeShaders = 0;
  void drawNodes();

  void drawPath();

};


#endif //LIBGRAFOS_SCREEN_H
