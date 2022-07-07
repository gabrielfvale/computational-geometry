#include "display.h"
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>

Display::Display(int width, int height, const std::string &title)
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL_Init() Error: " << SDL_GetError() << std::endl;
    SDL_ClearError();
  }

  // Don't stop window compositor
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

  // Set OpenGL attributes
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Create window
  m_window = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width, height,
      SDL_WINDOW_OPENGL);

  if (!m_window)
  {
    std::cerr << "SDL_CreateWindow() Error: " << SDL_GetError() << std::endl;
    return;
  }

  m_glContext = SDL_GLContext(m_window);
  if (!m_glContext)
  {
    std::cerr << "SDL_GLContext() Error: " << SDL_GetError() << std::endl;
    return;
  }

  SDL_GL_MakeCurrent(m_window, m_glContext);

  // Checks How OpenGL handled setting
  int rs, gs, bs;

  SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &rs);
  SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &gs);
  SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &bs);
  printf("Red size: %d, Green size: %d, Blue size: %d\n", rs, gs, bs);
}

Display::~Display()
{
  SDL_GL_DeleteContext(m_glContext);

  SDL_DestroyWindow(m_window);

  SDL_Quit();
}
