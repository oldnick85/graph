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

std::string Id2Str(const Coord2D& id)
{
    return id.ToStr();
}

class NeighborhoodMoore
{
  public:
    static std::vector<Coord2D> NeighbourCoordinates(const Coord2D& coord, const Coord2D& max_coord)
    {
        std::vector<Coord2D> neighbours;
        neighbours.reserve(8);
        if ((coord.X() > 0) and (coord.Y() > 0))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() - 1));
        if ((coord.X() > 0) and (coord.Y() < max_coord.Y()))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() + 1));
        if ((coord.X() < max_coord.X()) and (coord.Y() > 0))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() - 1));
        if ((coord.X() < max_coord.X()) and (coord.Y() < max_coord.Y()))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() + 1));
        if (coord.X() > 0)
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y()));
        if (coord.Y() > 0)
            neighbours.push_back(Coord2D(coord.X(), coord.Y() - 1));
        if (coord.X() < max_coord.X())
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y()));
        if (coord.Y() < max_coord.Y())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() + 1));
        return neighbours;
    }

    static bool IsHex() { return false; }

  private:
};

class NeighborhoodVonNeumann
{
  public:
    static std::vector<Coord2D> NeighbourCoordinates(const Coord2D& coord, const Coord2D& max_coord)
    {
        std::vector<Coord2D> neighbours;
        neighbours.reserve(4);
        if (coord.X() > 0)
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y()));
        if (coord.Y() > 0)
            neighbours.push_back(Coord2D(coord.X(), coord.Y() - 1));
        if (coord.X() < max_coord.X())
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y()));
        if (coord.Y() < max_coord.Y())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() + 1));
        return neighbours;
    }

    static bool IsHex() { return false; }

  private:
};

class NeighborhoodHex
{
  public:
    static std::vector<Coord2D> NeighbourCoordinates(const Coord2D& coord, const Coord2D& max_coord)
    {
        std::vector<Coord2D> neighbours;
        neighbours.reserve(8);
        if (((coord.Y() % 2) == 0) and (coord.X() > 0) and (coord.Y() > 0))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() - 1));
        if (((coord.Y() % 2) == 0) and (coord.X() > 0) and (coord.Y() < max_coord.Y()))
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y() + 1));
        if (((coord.Y() % 2) == 1) and (coord.X() < max_coord.X()) and (coord.Y() > 0))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() - 1));
        if (((coord.Y() % 2) == 1) and (coord.X() < max_coord.X()) and (coord.Y() < max_coord.Y()))
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y() + 1));
        if (coord.X() > 0)
            neighbours.push_back(Coord2D(coord.X() - 1, coord.Y()));
        if (coord.Y() > 0)
            neighbours.push_back(Coord2D(coord.X(), coord.Y() - 1));
        if (coord.X() < max_coord.X())
            neighbours.push_back(Coord2D(coord.X() + 1, coord.Y()));
        if (coord.Y() < max_coord.Y())
            neighbours.push_back(Coord2D(coord.X(), coord.Y() + 1));
        return neighbours;
    }

    static bool IsHex() { return true; }

  private:
};

template <typename TNode = Node<Coord2D>, typename TNeighborhood = NeighborhoodMoore>
class Area2D
{
  public:
    Area2D() = default;
    Area2D(const Coord2D&& max_coord) : m_max_coord(max_coord)
    {
        GRAPH_DEBUG_ASSERT(m_max_coord.X() >= 0, "Wrong X coordinate");
        GRAPH_DEBUG_ASSERT(m_max_coord.Y() >= 0, "Wrong Y coordinate");
        m_map.resize((m_max_coord.X() + 1) * (m_max_coord.Y() + 1));
        for (auto& pnt : m_map)
            pnt = 0;
    }

    void SetPassableAll(bool passable)
    {
        for (int y = 0; y <= m_max_coord.Y(); ++y)
        {
            for (int x = 0; x <= m_max_coord.X(); ++x)
            {
                SetPassable(Coord2D(x, y), passable);
            }
        }
    }

    void SetPassable(const Coord2D& coord, bool passable)
    {
        GRAPH_DEBUG_ASSERT((coord.X() >= 0) and (coord.X() <= m_max_coord.X()), "Wrong X coordinate");
        GRAPH_DEBUG_ASSERT((coord.Y() >= 0) and (coord.Y() <= m_max_coord.Y()), "Wrong Y coordinate");
        if (passable)
        {
            if (m_map[CoordToMap(coord)] != 0)
                return;
            m_map[CoordToMap(coord)] = 1;
            auto node                = new TNode(coord);
            m_graph.Add(node);

            const auto neighbours = TNeighborhood::NeighbourCoordinates(coord, m_max_coord);
            for (auto const& neighbour : neighbours)
            {
                if (m_map[CoordToMap(neighbour)] == 0)
                    continue;
                auto node2 = m_graph.Find(neighbour);
                auto edge  = new Edge<TNode>(node, node2);
                node->AddEdge(edge);
                node2->AddEdge(edge);
                m_graph.Add(edge);
            }
        }
        else
        {
            if (m_map[CoordToMap(coord)] == 0)
                return;
            m_map[CoordToMap(coord)] = 0;
            auto node                = m_graph.Find(coord);
            m_graph.Del(node);
        }
        GRAPH_DEBUG_ASSERT(m_graph.CheckCorrect(), "Incorrect graph");
    }

    std::string ToStrASCII(GG::PathFindContext<TNode, Edge<TNode>, DirectedFalse<Edge<TNode>>,
                                               WeightedFalse<Edge<TNode>>>* path_find_context = nullptr) const
    {
        std::string res;
        res.reserve(m_map.size() + m_max_coord.Y() + 1);
        res += "┌";
        for (int x = 0; x <= m_max_coord.X(); ++x)
            res += "──";
        if (TNeighborhood::IsHex())
            res += "─";
        res += "┐\n";
        for (int y = 0; y <= m_max_coord.Y(); ++y)
        {
            res += "│";
            if (TNeighborhood::IsHex() and ((y % 2) == 1))
                res += " ";
            for (int x = 0; x <= m_max_coord.X(); ++x)
            {
                if (m_map[CoordToMap(Coord2D(x, y))] == 0)
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
        for (int x = 0; x <= m_max_coord.X(); ++x)
            res += "──";
        if (TNeighborhood::IsHex())
            res += "─";
        res += "┘\n";
        return res;
    }

    const auto& Graph() const { return m_graph; }

  private:
    std::size_t CoordToMap(const Coord2D& coord) const { return (coord.X() * (m_max_coord.Y() + 1) + coord.Y()); }

    Coord2D m_max_coord;
    std::vector<int> m_map;
    GraphInclusive<TNode, Edge<TNode>, DirectedFalse<Edge<TNode>>, WeightedFalse<Edge<TNode>>> m_graph;
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