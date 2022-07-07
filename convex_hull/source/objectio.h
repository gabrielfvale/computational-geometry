#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>

#include <SDL2/SDL.h>

static std::vector<std::vector<SDL_FPoint>> load_obj(const char *file_name)
{
  std::stringstream ss;
  std::ifstream in_file(file_name);
  std::string line = "";
  std::string prefix = "";
  std::string part = "";
  SDL_FPoint temp;
  int temp_ind;

  std::vector<std::vector<SDL_FPoint>> parts;
  std::vector<SDL_FPoint> v;

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
      temp.x = std::stof(coordx);
      temp.y = std::stof(coordy);

      v.push_back(temp);
    }
    else
    {
      if (v.size() > 0)
      {
        parts.push_back(v);
        v.clear();
      }
      continue;
    }
  }

  // Cases when file ends without new lines
  if (v.size() > 0)
  {
    parts.push_back(v);
    v.clear();
  }

  return parts;
}

static std::tuple<SDL_FPoint *, int> load_legacy(const char *file_name, int w, int h)
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
    else
    {
      continue;
    }
  }

  SDL_FPoint *result = new SDL_FPoint[v.size()];
  memcpy(result, &v.front(), v.size() * sizeof(SDL_FPoint));

  return std::make_tuple(result, v.size());
}

static int write_obj(std::vector<std::vector<SDL_FPoint>> &object, std::string file_name)
{
  std::string out_path("out/");
  std::string path = out_path + file_name;

  std::cout << "Writing " << path << std::endl;

  std::ofstream output;
  output.open(path.c_str());

  for (auto part : object)
  {
    for (auto point : part)
    {
      output << "v " << point.x << " " << point.y << " "
             << "0.0\n";
    }
    output << "s\n";
  }

  output.close();
  return 0;
}
