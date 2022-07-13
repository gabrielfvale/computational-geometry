#include "display.h"
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

Display::Display(int width, int height, const std::string &title)
{
  this->width = width;
  this->height = height;

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL_Init() Error: " << SDL_GetError() << std::endl;
    return;
  }

  // Don't stop window compositor
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

  m_window = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width, height,
      SDL_WINDOW_OPENGL);

  if (m_window == nullptr)
  {
    std::cerr << "SDL_CreateWindow() Error: " << SDL_GetError() << std::endl;
    return;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  m_glContext = SDL_GL_CreateContext(m_window);

  float half_w = width / 2;
  float half_h = height / 2;

  glClearColor(0.f, 0.f, 0.f, 0.f);
  glViewport(0, 0, width, height);

  // Set orthographic projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-half_w, half_w, -half_h, half_h, -1, 1);
  glMatrixMode(GL_MODELVIEW);
}

Display::~Display()
{
  SDL_GL_DeleteContext(m_glContext);

  SDL_DestroyWindow(m_window);

  SDL_Quit();
}

void Display::Clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

bool Display::IsClosed()
{
  return m_isClosed;
}

void Display::Update(bool *h_toggle, bool *t_toggle)
{

  SDL_GL_SwapWindow(m_window);
  SDL_Event e;

  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
    case SDL_QUIT:
      m_isClosed = true;
      break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym)
      {
      case SDLK_h:
        if (h_toggle != nullptr)
          *h_toggle = !(*h_toggle);
        break;
      case SDLK_t:
        if (t_toggle != nullptr)
          *t_toggle = !(*t_toggle);
        break;
      case SDLK_ESCAPE:
        m_isClosed = true;
        break;
      }
      break;
    default:
      break;
    }
  }
}
