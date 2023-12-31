// Welcome to Giovanni GL!
//
// I, your interviewer Giovanni, challenge you to find and fix all the bugs in the following
// OpenGL program.
//
// Without getting this working, you're not getting past this stage of the interview process,
// young man!

#include <cstdio>
#include <stdexcept>
#include "glfw3.h"
#include "glad.h"
#include "glm.hpp"

using namespace glm;

int width = 1152, height = 720;
uint vao = -1;
uint vbo = -1;
uint shader = -1;
int u_time_loc = -1;
float data[] =
  {
    +0.50, -0.29 /* 1.1 */, 1.0, 0.0, 0.0 /* 1.2 */,
    -0.50, -0.29 /* 2.1 */, 0.0, 1.0, 0.0 /* 2.2 */,
    +0.00, +0.58 /* 3.1 */, 0.0, 0.0, 1.0 /* 3.2 */,
  };

const char* vert_src_fmt =
R"(
#version 460

layout(location=0) in vec2 pos;
layout(location=1) in vec3 color;

out vec4 v_color;

uniform float u_time;

mat2 rotate(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  return mat2(c, -s, s, c);
}

void main() {
  v_color = vec4(color, 1.0);
  vec2 pos = pos;
  pos = rotate(u_time) * pos;
  pos.x *= %f /* aspect ratio */;
  gl_Position = vec4(pos, 0.0, 1.0);
}
)";

const char* frag_src =
R"(
#version 460

in vec4 v_color;

out vec4 f_color;

void main() {
  f_color = v_color;
}
)";

void init() {
  glEnable(GL_MULTISAMPLE);

  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);

  glVertexArrayVertexBuffer(vao, 0, vbo, 0, 5 * sizeof(float));

  glEnableVertexArrayAttrib(vao, 0);
  glEnableVertexArrayAttrib(vao, 1);

  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0 * sizeof(float));
  glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, false, 2 * sizeof(float));

  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribBinding(vao, 1, 0);

  glNamedBufferData(vbo, int(sizeof(data)), data, GL_STATIC_DRAW);

  uint vsh, fsh;
  vsh = glCreateShader(GL_VERTEX_SHADER);
  fsh = glCreateShader(GL_FRAGMENT_SHADER);
  shader = glCreateProgram();

  float asp = (float) width / (float) height;

  char* vert_src = new char[1024];

  std::snprintf(vert_src, 1024, vert_src_fmt, asp);

  glShaderSource(vsh, 1, &vert_src, nullptr);
  glShaderSource(fsh, 1, &frag_src, nullptr);

  glCompileShader(vsh);
  int status;
  glGetShaderiv(vsh, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(vsh, 512, nullptr, buffer);
    throw std::runtime_error(buffer);
  }

  glCompileShader(fsh);
  glGetShaderiv(fsh, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(fsh, 512, nullptr, buffer);
    throw std::runtime_error(buffer);
  }

  glAttachShader(shader, vsh);
  glAttachShader(shader, fsh);

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetProgramInfoLog(shader, 512, nullptr, buffer);
    throw std::runtime_error(buffer);
  }

  u_time_loc = glGetUniformLocation(shader, "u_time");
}

void loop() {
  glClear(GL_COLOR);
  glUseProgram(shader);
  glUniform1f(u_time_loc, float(glfwGetTime()));
  glBindVertexArray(vao);
  glBindBuffer(vbo, GL_ARRAY_BUFFER);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main() {
  if (!glfwInit()) throw std::runtime_error("failed to init GLFW!");
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_SAMPLES, 4);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  if (GLFWwindow* win = glfwCreateWindow(width, height, "Giovanni GL", nullptr, nullptr)) {
    glfwMakeContextCurrent(win);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) throw std::runtime_error("failed to init GLAD!");

    init();

    while (!glfwWindowShouldClose(win)) {
      loop();
      glfwPollEvents();
      glfwSwapBuffers(win);
    }

    glfwTerminate();
  } else {
    throw std::runtime_error("failed to open a GLFW window!");
  }
}