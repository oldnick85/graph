// Copyright 2024 oldnick85

#pragma once

#include <string>
#include <vector>

#include <path_find.h>

namespace GG
{

class Coord2D
{
  public:
    Coord2D(int x, int y) : m_x(x), m_y(y) {}

    int X() const { return m_x; }
    int Y() const { return m_y; }

    std::string ToStr() const
    {
        std::string res;
        res += "(";
        res += std::to_string(m_x);
        res += ";";
        res += std::to_string(m_y);
        res += ")";
        return res;
    }

  private:
    int m_x = 0;
    int m_y = 0;
};

class Area2D
{
  public:
    Area2D() = default;
    Area2D(Coord2D& max_coord) : m_max_coord(max_coord)
    {
        m_map.resize((m_max_coord.X() + 1) * (m_max_coord.Y() + 1));
        for (auto& pnt : m_map)
            pnt = 0;
    }

  private:
    Coord2D m_max_coord;
    std::vector<int> m_map;
};
}  // namespace GG