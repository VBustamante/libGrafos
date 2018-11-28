//
// Created by vbustamante on 11/27/18.
//

#include "Screen.h"

using namespace std;

Screen::Screen(EuclidianGraph *graph) : graph(graph) {
  if(!glfwInit()){
    cerr << "ERROR: Could not start glfw" << endl;
    return;
  }

  glfwWindowHint(GLFW_RESIZABLE, true);
  glfwWindowHint(GLFW_FOCUSED, true);
//  glfwWindowHint(GLFW_VISIBLE, false);
  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LibGrafos", nullptr, nullptr);

  if (!window) {
    cerr << "Failed to open window with GLFW3" << endl;
    return;
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


  // APPLICATION VARIABLES
  double intervalTimer;

  GLuint debugVao;
  glGenVertexArrays(1, &debugVao);
  glBindVertexArray(debugVao);

  while(!glfwWindowShouldClose(window)){
    intervalTimer = glfwGetTime();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    glClearColor(0.2, 0.2, 0.2, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    double frametime = glfwGetTime() - intervalTimer;
    auto sleepTime = static_cast<DWORD>((1/60.0f - frametime) * 1000);

    if(graph->getPath() != nullptr) drawPath();
    drawNodes();

    glfwPollEvents();
    glfwSwapBuffers(window);

    if(frametime < 1/60.0f && sleepTime < 100){
      lg_sleep(sleepTime);
    }
  }

}



Screen::~Screen() {
  glfwTerminate();
}

GLfloat bringToRange(float value, float sourceMin, float sourceMax){
  GLfloat x = (value - sourceMin)/(sourceMax - sourceMin);
  return (x*1.8f) - 0.9f;
}

void Screen::drawNodes() {
  GLuint VBO;
  GLuint shader_program;
  // Lazy start VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  const char* vertex_shader =
      "#version 400\n"
      "in vec3 vp;"
      "void main() {"
      "  gl_Position = vec4(vp, 1.0);"
      "}";
  const char* fragment_shader =
      "#version 400\n"
      "out vec4 frag_color;"
      "void main() {"
      "  frag_color = vec4(0.5, 0.0, 0.5, 1.0);"
      "}";
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, nullptr);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, nullptr);
  glCompileShader(fs);
  shader_program = glCreateProgram();
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

  glUseProgram(shader_program);


  for(int i = 0; i < graph->getN(); i++){
    auto node = (*(graph->getNodes()))[i];
    GLfloat x = bringToRange(node.x, graph->getMinX(), graph->getMaxX());
    GLfloat y = bringToRange(node.y, graph->getMinY(), graph->getMaxY());

    GLfloat vertices[] = {
        x-.025f, y-.03f,  0.0f,
        x,       y+.025f, 0.0f,
        x+.025f, y-.03f,  0.0f
    };
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

}

void Screen::drawPath() {
  GLuint VBO;
  GLuint shader_program;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  const char* vertex_shader =
      "#version 400\n"
      "in vec3 vp;"
      "void main() {"
      "  gl_Position = vec4(vp, 1.0);"
      "}";
  const char* fragment_shader =
      "#version 400\n"
      "out vec4 frag_color;"
      "void main() {"
      "  frag_color = vec4(0.0, 0.5, 0.0, 1.0);"
      "}";
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, nullptr);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, nullptr);
  glCompileShader(fs);
  shader_program = glCreateProgram();
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

  glUseProgram(shader_program);

  auto path = graph->getPath();
  for(int i = 0; i < graph->getN(); i++){
    auto src = (*path)[i];
    auto dst = (*path)[(i+1) % graph->getN()];
    GLfloat srcX = bringToRange(src.x, graph->getMinX(), graph->getMaxX());
    GLfloat srcY = bringToRange(src.y, graph->getMinY(), graph->getMaxY());
    GLfloat dstX = bringToRange(dst.x, graph->getMinX(), graph->getMaxX());
    GLfloat dstY = bringToRange(dst.y, graph->getMinY(), graph->getMaxY());

    GLfloat vertices[] = {
        srcX, srcY,  0.0f,
        dstX, dstY,  0.0f
    };
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_LINES, 0, 2);
  }

}
