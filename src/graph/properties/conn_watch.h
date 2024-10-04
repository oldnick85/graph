// Copyright 2024 oldnick85

#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

namespace GG
{

template <typename TNode, typename TEdge, bool IsWatching>
class ConnectedComponentWatch
{};

template <typename TNode, typename TEdge>
class ConnectedComponentWatch<TNode, TEdge, true>
{
  public:
    static constexpr int ComponentIdNone = -1;
    using NodesSet_t                     = std::unordered_set<TNode*>;
    using Components_t                   = std::unordered_map<int, NodesSet_t>;

    int ConnectedComponentsCount() const { return m_connected_components.size(); }

    bool SurelyConnected(TNode* node1, TNode* node2) const
    {
        GRAPH_DEBUG_ASSERT(node1 != nullptr, "Null node 1");
        GRAPH_DEBUG_ASSERT(node2 != nullptr, "Null node 2");
        auto component_it = FindComponent(node1);
        return component_it->second.contains(node2);
    }

    bool SurelyNotConnected(TNode* node1, TNode* node2) const
    {
        GRAPH_DEBUG_ASSERT(node1 != nullptr, "Null node 1");
        GRAPH_DEBUG_ASSERT(node2 != nullptr, "Null node 2");
        auto component_it = FindComponent(node1);
        return (not component_it->second.contains(node2));
    }

  protected:
    static constexpr bool Watching = true;

    /**
     * \~english
     * @brief Handling node deletion event
     * 
     * @param edge removed node
     * 
     * @remark the function must be called immediately after deleting a vertex and all edges incident to the vertex
     */
    /**
     * \~russian
     * @brief Обработка события удаления вершины
     * 
     * @param edge удалённая вершина
     * 
     * @remark функция должна вызываться непосредственно после удаления вершины и всех ребер, инцидентных вершине
     */
    void onDel(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        GRAPH_DEBUG_ASSERT(node->Edges().empty(), "Deleted node must not have edges");
        auto component_it = FindComponent(node);
        GRAPH_DEBUG_ASSERT(component_it != m_connected_components.end(), "Node not in components");
        DelComponent(component_it->first);
    }

    /**
     * \~english
     * @brief Handling edge deletion event
     * 
     * @param edge removed edge
     * 
     * @remark function must be called immediately after an edge is removed from its endpoints and edge list
     */
    /**
     * \~russian
     * @brief Обработка события удаления ребра
     * 
     * @param edge удалённое ребро
     * 
     * @remark функция должна вызываться непосредственно после удаления ребра из его концов и списка рёбер
     */
    void onDel(TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        auto node1         = edge->Nodes().first;
        auto node2         = edge->Nodes().second;
        auto component1_it = FindComponent(node1);
        GRAPH_DEBUG_ASSERT(component1_it != m_connected_components.end(), "Unknown edge");
        auto component2_it = FindComponent(node2);
        GRAPH_DEBUG_ASSERT(component2_it != m_connected_components.end(), "Unknown edge");
        GRAPH_DEBUG_ASSERT(component1_it->first == component2_it->first, "Connection between disconnected");

        auto connected1 = GetConnectedWith(node1, node2);
        if (connected1.contains(node2))
            return;
        DelComponent(component1_it->first);
        auto new_component1_it    = AddComponent();
        new_component1_it->second = std::move(connected1);
        auto connected2           = GetConnectedWith(node2, nullptr);
        auto new_component2_it    = AddComponent();
        new_component2_it->second = std::move(connected2);
    }

    void onAdd(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        GRAPH_DEBUG_ASSERT(FindComponent(node) == m_connected_components.end(), "Node already in some component");
        std::unordered_set<int> adjacent_components;
        const auto edges = node->Edges();
        for (const auto& edge : edges)
        {
            auto* node2       = edge->OtherNode(node);
            auto component_it = FindComponent(node2);
            adjacent_components.insert(component_it->first);
        }

        if (adjacent_components.empty())
        {
            auto component_it = AddComponent();
            component_it->second.insert(node);
        }
        else if (adjacent_components.size() == 1)
        {
            const auto component_id = *adjacent_components.begin();
            m_connected_components[component_id].insert(node);
            return;
        }
        else
        {
            auto new_component_it = AddComponent();
            new_component_it->second.insert(node);
            for (const auto adjacent_component_id : adjacent_components)
            {
                auto adjacent_component_it = m_connected_components.find(adjacent_component_id);
                GRAPH_DEBUG_ASSERT(adjacent_component_it != m_connected_components.end(), "Unknown component");
                for (auto moved_node : adjacent_component_it->second)
                    new_component_it->second.insert(moved_node);
                m_connected_components.erase(adjacent_component_it);
            }
        }

        return;
    }

    void onAdd(TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        auto node1         = edge->Nodes().first;
        auto node2         = edge->Nodes().second;
        auto component1_it = FindComponent(node1);
        GRAPH_DEBUG_ASSERT(component1_it != m_connected_components.end(), "Unknown edge");
        auto component2_it = FindComponent(node2);
        GRAPH_DEBUG_ASSERT(component2_it != m_connected_components.end(), "Unknown edge");
        if (component1_it->first == component2_it->first)
            return;
        auto new_component_it = AddComponent();
        for (auto moved_node : component1_it->second)
            new_component_it->second.insert(moved_node);
        m_connected_components.erase(component1_it);
        for (auto moved_node : component2_it->second)
            new_component_it->second.insert(moved_node);
        m_connected_components.erase(component2_it);
    }

    void Clear() { m_connected_components.clear(); };

  private:
    NodesSet_t GetConnectedWith(TNode* node, TNode* stop_node)
    {
        NodesSet_t connected;
        connected.insert(node);
        NodesSet_t front;
        front.insert(node);
        while (not front.empty())
        {
            NodesSet_t new_front;
            for (auto front_node : front)
            {
                auto neighbour_edges = front_node->Edges();
                for (auto neighbour_edge : neighbour_edges)
                {
                    auto neighbour_node = neighbour_edge->OtherNode(front_node);
                    if (connected.contains(neighbour_node))
                        continue;
                    connected.insert(neighbour_node);
                    if (neighbour_node == stop_node)
                        return connected;
                    new_front.insert(neighbour_node);
                }
            }
            front = std::move(new_front);
        }
        return connected;
    }

    auto AddComponent()
    {
        ++m_component_id;
        auto empl = m_connected_components.emplace(std::make_pair(m_component_id, NodesSet_t{}));
        return empl.first;
    }

    void DelComponent(int component_id) { m_connected_components.erase(component_id); }

    auto FindComponent(TNode* node) const
    {
        auto component_it = m_connected_components.begin();
        while (component_it != m_connected_components.end())
        {
            if (component_it->second.contains(node))
            {
                return component_it;
            }
            ++component_it;
        }
        return component_it;
    }

    int m_component_id = ComponentIdNone;
    std::unordered_map<int, NodesSet_t> m_connected_components;
};

template <typename TNode, typename TEdge>
class ConnectedComponentWatch<TNode, TEdge, false>
{
  public:
    int ConnectedComponentsCount() const { return -1; }

    bool SurelyConnected([[maybe_unused]] TNode* node1, [[maybe_unused]] TNode* node2) const { return false; }
    bool SurelyNotConnected([[maybe_unused]] TNode* node1, [[maybe_unused]] TNode* node2) const { return false; }

  protected:
    static constexpr bool Watching = false;

    void onAdd([[maybe_unused]] TNode* node) {}
    void onAdd([[maybe_unused]] TEdge* edge) {}
    void onDel([[maybe_unused]] TNode* node) {}
    void onDel([[maybe_unused]] TEdge* edge) {}

    void Clear() {};
};

}  // namespace GG