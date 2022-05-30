#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>

#include <SDL2/SDL.h>

static std::tuple<SDL_FPoint *, int> load_obj(const char *file_name, int w, int h, std::vector<std::vector<int>> &faces)
{
  std::stringstream ss;
  std::ifstream in_file(file_name);
  std::string line = "";
  std::string prefix = "";
  SDL_FPoint temp;
  int temp_ind;

  std::vector<SDL_FPoint> v;
  int half_w = w / 2;
  int half_h = h / 2;

  if (!in_file.is_open())
  {
    throw "Obj loader failed";
  }

  while (std::getline(in_file, line))
  {
    ss.clear();
    ss.str(line);
    ss >> prefix;

    std::string coordx = "";
    std::string coordy = "";

    if (prefix == "v")
    {
      ss >> coordx >> coordy;
      temp.x = std::stof(coordx) + half_w;
      temp.y = std::stof(coordy) * -1 + half_h;

      v.push_back(temp);
    }
    else if (prefix == "f")
    {
      std::vector<int> indexes;
      while (ss >> temp_ind)
        indexes.push_back(temp_ind - 1);
      faces.push_back(indexes);
    }
    else
    {
      continue;
    }
  }

  SDL_FPoint *result = new SDL_FPoint[v.size()];
  memcpy(result, &v.front(), v.size() * sizeof(SDL_FPoint));

  return std::make_tuple(result, v.size());
}
