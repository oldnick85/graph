// Copyright 2024 oldnick85

#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "./common.h"
#include "./graph_properties.h"

namespace GG
{

template <typename R, typename... Args>
R return_type_of(R (*)(Args...));

/**
 * @brief Graph class that stores nodes and edges
 * 
 * @tparam TNodeId node id type
 * @tparam TNode node type
 * @tparam TEdge edge type
 * @tparam TDirected directed graph property
 * @tparam TWeighted weighted graph property
 */
template <typename TNode, typename TEdge, typename TDirected, typename TWeighted>
class GraphInclusive : private TDirected, private TWeighted
{
  public:
    using TNodeId = TNode::NodeId_t;
    //using TNodeId = decltype(return_type_of(TNode::Id));

    /**
     * @brief Add node
     * 
     * @param node node
     */
    void Add(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        m_nodes.emplace(node->Id(), node);
    }

    /**
     * @brief Add edge
     * 
     * @param edge edge
     */
    void Add(TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        m_edges.push_back(edge);
    }

    /**
     * @brief Find node by id
     * 
     * @param id node id
     * @return node found or nullptr
     */
    TNode* Find(const TNodeId& id) const
    {
        const auto n_it = m_nodes.find(id);
        if (n_it == m_nodes.end())
            return nullptr;
        return n_it->second;
    }

    /**
     * @brief Check graph correctness
     * 
     * @return true graph correct
     * @return false graph incorrect
     */
    bool CheckCorrect() const
    {
        for (const auto& node_el : m_nodes)
        {
            const auto node = node_el.second;
            if (node->Id() != node_el.first)
                return false;
            for (const auto edge : node->Edges())
            {
                if (std::find(m_edges.begin(), m_edges.end(), edge) == m_edges.end())
                    return false;
            }
        }

        for (const auto& edge : m_edges)
        {
            const auto node1 = edge->Nodes().first;
            if (node1 == nullptr)
                return false;
            const auto node2 = edge->Nodes().second;
            if (node2 == nullptr)
                return false;
            if (m_nodes.find(node1->Id()) == m_nodes.end())
                return false;
            if (m_nodes.find(node2->Id()) == m_nodes.end())
                return false;
        }
        return true;
    }

    void DeleteAll()
    {
        for (const auto& node_el : m_nodes)
        {
            const auto& node = node_el.second;
            delete node;
        }
        m_nodes.clear();
        for (const auto& edge : m_edges)
        {
            delete edge;
        }
        m_edges.clear();
    }

    std::string ToStr() const
    {
        std::string str{"GraphInclusive\n"};
        std::array<char, 256> strbuf;
        for (const auto& node_el : m_nodes)
        {
            const auto& node = node_el.second;
            snprintf(strbuf.data(), strbuf.size(), "Node %s (%p)\n", node->ToStr().c_str(), node);
            str.append(strbuf.data());
        }
        for (const auto& edge : m_edges)
        {
            snprintf(strbuf.data(), strbuf.size(), "Edge %s --%s %s%s (%p)\n", edge->Nodes().first->ToStr().c_str(),
                     (TDirected::GetDirected(edge)) ? ">" : "-", edge->Nodes().second->ToStr().c_str(),
                     (TWeighted::GetWeight(edge) == 1.0 ? "" : std::to_string(TWeighted::GetWeight(edge)).c_str()),
                     edge);
            str.append(strbuf.data());
        }
        return str;
    }

  private:
    std::unordered_map<TNodeId, TNode*> m_nodes;
    std::vector<TEdge*> m_edges;
};

}  // namespace GG