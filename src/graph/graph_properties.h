// Copyright 2024 oldnick85

#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

namespace GG
{

template <typename TEdge>
class DirectedTrue
{
  public:
    static inline bool Directed() { return true; }
    static inline bool GetDirected(const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return edge->Directed();
    }
};

template <typename TEdge>
class DirectedFalse
{
  public:
    static inline bool Directed() { return false; }
    static inline bool GetDirected([[maybe_unused]] const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return false;
    }
};

template <typename TEdge>
class WeightedTrue
{
  public:
    static inline float GetWeight(const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return edge->Weight();
    }
};

template <typename TEdge>
class WeightedFalse
{
  public:
    static inline float GetWeight([[maybe_unused]] const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return 1.0;
    }
};

class NamedTrue
{
  public:
    NamedTrue() = default;
    NamedTrue(const std::string& name) : m_name(name) { GRAPH_DEBUG_ASSERT(not m_name.empty(), "Empty graph name"); }
    std::string GetName() const { return m_name; }

  private:
    std::string m_name;
};

class NamedFalse
{
  public:
    NamedFalse() = default;
    NamedFalse(__attribute__((unused)) const std::string& name) {}
    std::string GetName() const
    {
        std::array<char, 16> str;
        snprintf(str.data(), str.size(), "%p", this);
        return str.data();
    }
};

}  // namespace GG