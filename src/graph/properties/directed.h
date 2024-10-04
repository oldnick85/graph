// Copyright 2024 oldnick85

#pragma once

namespace GG
{

template <typename TEdge, bool IsDirected>
class Directed
{};

template <typename TEdge>
class Directed<TEdge, true>
{
  public:
    static constexpr bool IsDirected = true;

  protected:
    static inline bool GetDirected(const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return edge->Directed();
    }
};

template <typename TEdge>
class Directed<TEdge, false>
{
  public:
    static constexpr bool IsDirected = false;

  protected:
    static inline bool GetDirected([[maybe_unused]] const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return false;
    }
};

}  // namespace GG