//
// Created by vbustamante on 11/27/18.
//

#ifndef LIBGRAFOS_SCREEN_H
#define LIBGRAFOS_SCREEN_H
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "sleep.h"

class Screen {
public:
  Screen ();
  ~Screen ();

private:
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;
  GLFWwindow *window = nullptr;

};


#endif //LIBGRAFOS_SCREEN_H
