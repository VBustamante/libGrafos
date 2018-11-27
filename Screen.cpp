//
// Created by vbustamante on 11/27/18.
//

#include "Screen.h"

using namespace std;

Screen::Screen() {
  if(!glfwInit()){
    cerr << "ERROR: Could not start glfw" << endl;
  }

  glfwWindowHint(GLFW_RESIZABLE, true);
  glfwWindowHint(GLFW_FOCUSED, false);
//  glfwWindowHint(GLFW_VISIBLE, false);
  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LibGrafos", nullptr, nullptr);

  if (!window) {
    cerr << "Failed to open window with GLFW3" << endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)->void{
    std::cout << "Changed Screen Size" << std::endl;
    glViewport(0, 0, width, height);
  });

  if (gl3wInit()) {
    cerr << "Failed to initialize OpenGL" << endl;
    return;
  }
  if (!gl3wIsSupported(3, 2)) {
    cerr << "OpenGL 3.2 not supported" << endl;
  }

  // INITIAL CONFIGURATIONS
  glfwSwapInterval(1);
//  glEnable(GL_DEPTH_TEST);
//  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  {
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version: " << version << endl;
  }
  glClearColor(0.2, 0.2, 0.2, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);


  lg_sleep(5000);
}

Screen::~Screen() {
  glfwTerminate();
}
