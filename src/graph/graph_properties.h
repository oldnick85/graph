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

}  // namespace GG