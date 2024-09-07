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

    bool operator==(const Coord2D& rhs) const { return ((X() == rhs.X()) and (Y() == rhs.Y())); }
    bool operator!=(const Coord2D& rhs) const { return !(*this == rhs); }

    int X() const { return m_x; }
    int Y() const { return m_y; }

    /**
     * \~english
     * @brief Get string description
     * 
     * @return string description
     */
    /**
     * \~russian
     * @brief Получить строковое описание
     * 
     * @return строковое описание
     */
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

class Range2D
{
  public:
    /**
     * \~english
     * @brief Constructor for Range2D object
     * 
     * @param max maximum coordinate
     * @param min minimum coordinate
     */
    /**
     * \~russian
     * @brief Конструктор объекта Range2D
     * 
     * @param max максимальная координата
     * @param min минимальная координата
     */
    Range2D(const Coord2D& max, const Coord2D& min) : m_max(max), m_min(min)
    {
        GRAPH_DEBUG_ASSERT(m_max.X() >= m_min.X(), "Wrong X coordinate");
        GRAPH_DEBUG_ASSERT(m_max.Y() >= m_min.Y(), "Wrong Y coordinate");
    }

    /**
     * \~english
     * @brief Constructor for Range2D object
     * 
     * @param max maximum coordinate
     */
    /**
     * \~russian
     * @brief Конструктор объекта Range2D
     * 
     * @param max максимальная координата
     */
    explicit Range2D(const Coord2D& max) : m_max(max), m_min(Coord2D(0, 0))
    {
        GRAPH_DEBUG_ASSERT(m_max.X() >= m_min.X(), "Wrong X coordinate");
        GRAPH_DEBUG_ASSERT(m_max.Y() >= m_min.Y(), "Wrong Y coordinate");
    }

    /**
     * \~english
     * @brief Get the number of coordinates in a range
     * 
     * @return number of coordinates in a range 
     */
    /**
     * \~russian
     * @brief Получить количество координат в диапазоне
     * 
     * @return количество координат в диапазоне
     */
    uint Count() const { return (m_max.X() - m_min.X() + 1) * (m_max.Y() - m_min.Y() + 1); }

    /**
     * \~english
     * @brief Get the maximum X coordinate
     * 
     * @return maximum X coordinate 
     */
    /**
     * \~russian
     * @brief Получить максимальную координату X
     * 
     * @return максимальная координата X
     */
    int MaxX() const { return m_max.X(); }
    /**
     * \~english
     * @brief Get the maximum Y coordinate
     * 
     * @return maximum Y coordinate 
     */
    /**
     * \~russian
     * @brief Получить максимальную координату Y
     * 
     * @return максимальная координата Y
     */
    int MaxY() const { return m_max.Y(); }
    /**
     * \~english
     * @brief Get the minimum X coordinate
     * 
     * @return minimum X coordinate 
     */
    /**
     * \~russian
     * @brief Получить минимальную координату X
     * 
     * @return минимальная координата X
     */
    int MinX() const { return m_min.X(); }
    /**
     * \~english
     * @brief Get the minimum Y coordinate
     * 
     * @return minimum Y coordinate 
     */
    /**
     * \~russian
     * @brief Получить минимальную координату Y
     * 
     * @return минимальная координата Y
     */
    int MinY() const { return m_min.Y(); }

    /**
     * \~english
     * @brief Check if a coordinate is in a range
     * 
     * @param coord coordinate
     * @return true contained
     * @return false not contained
     */
    /**
     * \~russian
     * @brief Проверить содержится ли координата в диапазоне
     * 
     * @param coord координата
     * @return true содержится
     * @return false не содержится
     */
    bool Contains(const Coord2D& coord) const
    {
        return ((coord.X() >= m_min.X()) and (coord.X() <= m_max.X()) and (coord.Y() >= m_min.Y()) and
                (coord.Y() <= m_max.Y()));
    }

    /**
     * \~english
     * @brief Convert coordinate to linear view. Traverse first by Y then by X in increasing order
     * 
     * @param coord coordinate
     * @return linear coordinate 
     */
    /**
     * \~russian
     * @brief Преобразовать координату в линейный вид. Обход сначала по Y потом по X в порядке увеличения
     * 
     * @param coord координата
     * @return линейная координата
     */
    uint CoordToLineByY(const Coord2D& coord) const
    {
        GRAPH_DEBUG_ASSERT(Contains(coord), "Wrong coordinates");
        return ((coord.X() - m_min.X()) * (m_max.Y() - m_min.Y() + 1) + coord.Y() - m_min.Y());
    }

    /**
     * \~english
     * @brief Convert coordinate to linear view. Traverse first by X then by Y in increasing order
     * 
     * @param coord coordinate
     * @return linear coordinate 
     */
    /**
     * \~russian
     * @brief Преобразовать координату в линейный вид. Обход сначала по X потом по Y в порядке увеличения
     * 
     * @param coord координата
     * @return линейная координата
     */
    uint CoordToLineByX(const Coord2D& coord) const
    {
        GRAPH_DEBUG_ASSERT(Contains(coord), "Wrong coordinates");
        return ((coord.Y() - m_min.Y()) * (m_max.X() - m_min.X() + 1) + coord.X() - m_min.X());
    }

    /**
     * \~english
     * @brief Get string description
     * 
     * @return string description
     */
    /**
     * \~russian
     * @brief Получить строковое описание
     * 
     * @return строковое описание
     */
    std::string ToStr() const
    {
        std::string res;
        res += m_max.ToStr() + "_" + m_min.ToStr();
        return res;
    }

  private:
    Coord2D m_max;
    Coord2D m_min;
};

std::string Id2Str(const Coord2D& id)
{
    return id.ToStr();
}

class NeighborhoodMoore
{
  public:
    static std::vector<Coord2D> NeighbourCoordinates(const Coord2D& coord, const Range2D& range)
    {
        std::vector<Coord2D> neighbours;
        neighbours.reserve(8);
        if ((coord.X() > range.MinX()) and (coord.Y() > range.MinY()))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() - 1));
        if ((coord.X() > range.MinX()) and (coord.Y() < range.MaxY()))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() + 1));
        if ((coord.X() < range.MaxX()) and (coord.Y() > range.MinY()))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() - 1));
        if ((coord.X() < range.MaxX()) and (coord.Y() < range.MaxY()))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() + 1));
        if (coord.X() > range.MinX())
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y()));
        if (coord.Y() > range.MinY())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() - 1));
        if (coord.X() < range.MaxX())
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y()));
        if (coord.Y() < range.MaxY())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() + 1));
        return neighbours;
    }

    static bool IsHex() { return false; }

  private:
};

class NeighborhoodVonNeumann
{
  public:
    static std::vector<Coord2D> NeighbourCoordinates(const Coord2D& coord, const Range2D& range)
    {
        std::vector<Coord2D> neighbours;
        neighbours.reserve(4);
        if (coord.X() > range.MinX())
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y()));
        if (coord.Y() > range.MinY())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() - 1));
        if (coord.X() < range.MaxX())
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y()));
        if (coord.Y() < range.MaxY())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() + 1));
        return neighbours;
    }

    static bool IsHex() { return false; }

  private:
};

class NeighborhoodHex
{
  public:
    static std::vector<Coord2D> NeighbourCoordinates(const Coord2D& coord, const Range2D& range)
    {
        std::vector<Coord2D> neighbours;
        neighbours.reserve(8);
        if (((coord.Y() % 2) == 0) and (coord.X() > range.MinX()) and (coord.Y() > range.MinY()))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() - 1));
        if (((coord.Y() % 2) == 0) and (coord.X() > range.MinX()) and (coord.Y() < range.MaxY()))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() + 1));
        if (((coord.Y() % 2) == 1) and (coord.X() < range.MaxX()) and (coord.Y() > range.MinY()))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() - 1));
        if (((coord.Y() % 2) == 1) and (coord.X() < range.MaxX()) and (coord.Y() < range.MaxY()))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() + 1));
        if (coord.X() > range.MinX())
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y()));
        if (coord.Y() > range.MinY())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() - 1));
        if (coord.X() < range.MaxX())
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y()));
        if (coord.Y() < range.MaxY())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() + 1));
        return neighbours;
    }

    static bool IsHex() { return true; }

  private:
};

template <typename TNode, typename TNeighborhood,
          typename TConnectedComponentWatch = ConnectedComponentWatchFalse<TNode, Edge<TNode>>>
class Area2D
{
  public:
    Area2D() = default;

    explicit Area2D(const Range2D& range) : m_range(range)
    {
        m_map.resize(m_range.Count());
        for (auto& pnt : m_map)
            pnt = 0;
    }

    explicit Area2D(Range2D&& range) : m_range(range)
    {
        m_map.resize(m_range.Count());
        for (auto& pnt : m_map)
            pnt = 0;
    }

    void SetPassableAll(bool passable)
    {
        for (int y = m_range.MinY(); y <= m_range.MaxY(); ++y)
        {
            for (int x = m_range.MinX(); x <= m_range.MaxX(); ++x)
            {
                SetPassable(Coord2D(x, y), passable);
            }
        }
    }

    bool Passable(const Coord2D& coord) const { return (m_map[m_range.CoordToLineByY(coord)] == 1); }

    void SetPassable(const Coord2D& coord, bool passable)
    {
        GRAPH_DEBUG_ASSERT(m_range.Contains(coord), "Wrong coordinates");
        if (passable)
        {
            if (m_map[m_range.CoordToLineByY(coord)] != 0)
                return;
            m_map[m_range.CoordToLineByY(coord)] = 1;
            auto node                            = m_graph.MakeNode(coord);

            const auto neighbours = TNeighborhood::NeighbourCoordinates(coord, m_range);
            for (auto const& neighbour : neighbours)
            {
                if (m_map[m_range.CoordToLineByY(neighbour)] == 0)
                    continue;
                auto node2 = m_graph.Find(neighbour);
                m_graph.MakeEdge(node, node2);
            }
        }
        else
        {
            if (m_map[m_range.CoordToLineByY(coord)] == 0)
                return;
            m_map[m_range.CoordToLineByY(coord)] = 0;
            auto node                            = m_graph.Find(coord);
            m_graph.Del(node);
        }
        GRAPH_DEBUG_ASSERT(m_graph.CheckCorrect(), "Incorrect graph");
    }

    std::string ToStrASCII(
        GG::PathFindContext<TNode, Edge<TNode>, DirectedFalse<Edge<TNode>>, WeightedFalse<Edge<TNode>>,
                            TConnectedComponentWatch, NamedFalse>* path_find_context = nullptr) const
    {
        std::string res;
        res.reserve(m_map.size() + m_range.MaxY() + 1);
        res += "┌";
        for (int x = m_range.MinX(); x <= m_range.MaxX(); ++x)
            res += "──";
        if (TNeighborhood::IsHex())
            res += "─";
        res += "┐\n";
        for (int y = m_range.MinY(); y <= m_range.MaxY(); ++y)
        {
            res += "│";
            if (TNeighborhood::IsHex() and ((y % 2) == 1))
                res += " ";
            for (int x = m_range.MinX(); x <= m_range.MaxX(); ++x)
            {
                if (m_map[m_range.CoordToLineByY(Coord2D(x, y))] == 0)
                {
                    res += "██";
                }
                else
                {
                    if (path_find_context == nullptr)
                    {
                        res += "  ";
                    }
                    else
                    {
                        auto dist = static_cast<int>(path_find_context->DistanceTo(m_graph.Find(Coord2D(x, y))));
                        dist %= 100;
                        if (dist < 10)
                            res += " ";
                        res += std::to_string(dist);
                    }
                }
            }
            if (TNeighborhood::IsHex() and ((y % 2) == 0))
                res += " ";
            res += "│\n";
        }
        res += "└";
        for (int x = m_range.MinX(); x <= m_range.MaxX(); ++x)
            res += "──";
        if (TNeighborhood::IsHex())
            res += "─";
        res += "┘\n";
        return res;
    }

    const auto& Graph() const { return m_graph; }

  private:
    Range2D m_range;
    std::vector<int> m_map;
    GraphInclusive<TNode, Edge<TNode>, DirectedFalse<Edge<TNode>>, WeightedFalse<Edge<TNode>>, TConnectedComponentWatch,
                   NamedFalse>
        m_graph;
};
}  // namespace GG

namespace std
{
template <>
struct hash<GG::Coord2D> {
    std::size_t operator()(const GG::Coord2D& coord) const
    {
        std::size_t result = 0;
        result ^= std::hash<int>()(coord.X());
        result ^= std::hash<int>()(coord.Y());
        return result;
    }
};
}  // namespace std