// Copyright 2024 oldnick85

#pragma once

namespace GG
{

template <typename TEdge, bool IsWeighted>
class Weighted
{};

template <typename TEdge>
class Weighted<TEdge, true>
{
  public:
    static constexpr bool IsWeighted = true;

  protected:
    /**
     * \~english 
     * @brief Get edge weight
     * 
     * @param edge edge
     * @return weight
     */
    /**
     * \~russian 
     * @brief Получить вес ребра
     * 
     * @param edge ребро
     * @return вес
     */
    static inline float GetWeight(const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return edge->Weight();
    }
};

template <typename TEdge>
class Weighted<TEdge, false>
{
  public:
    static constexpr bool IsWeighted = false;

  protected:
    /**
     * \~english 
     * @brief Get edge weight
     * 
     * @param edge edge
     * @return weight
     */
    /**
     * \~russian 
     * @brief Получить вес ребра
     * 
     * @param edge ребро
     * @return вес
     */
    static inline float GetWeight([[maybe_unused]] const TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        return 1.0;
    }
};

}  // namespace GG