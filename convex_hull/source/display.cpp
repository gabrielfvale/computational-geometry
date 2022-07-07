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

  m_glContext = SDL_GL_CreateContext(m_window);

  glClearColor(1.f, 1.f, 1.f, 0.f);
  glViewport(0, 0, width, height);
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

void Display::Update()
{

  SDL_GL_SwapWindow(m_window);
  SDL_Event e;

  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      m_isClosed = true;
  }
}
