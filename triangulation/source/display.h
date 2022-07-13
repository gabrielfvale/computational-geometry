#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <string>

class Display
{
public:
  int width;
  int height;
  Display(int width, int height, const std::string &title);
  void Clear();
  void Update(bool *h_toggle = nullptr, bool *t_toggle = nullptr);
  bool IsClosed();
  virtual ~Display();

private:
  SDL_Window *m_window;
  SDL_GLContext m_glContext;
  bool m_isClosed;
};

#endif
