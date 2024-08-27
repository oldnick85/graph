// Copyright 2024 oldnick85

#pragma once

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
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

    void Del(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        const auto node_it = m_nodes.find(node->Id());
        if (node_it == m_nodes.end())
            return;
        GRAPH_DEBUG_ASSERT(node == node_it->second, "Wrong nodes in graph");
        for (auto edge : node->Edges())
        {
            auto node2 = (edge->Nodes().first != node) ? edge->Nodes().first : edge->Nodes().second;
            node2->DelEdge(edge);
            m_edges.erase(edge);
        }
        m_nodes.erase(node_it);
        delete node;
    }

    /**
     * @brief Add edge
     * 
     * @param edge edge
     */
    void Add(TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        m_edges.insert(edge);
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
            snprintf(strbuf.data(), strbuf.size(), "Node %s ", node->ToStr().c_str());
            str.append(strbuf.data());
            str.append(ToStrNodeEdges(node));
            str += "\n";
        }
        /*
        for (const auto& edge : m_edges)
        {
            snprintf(strbuf.data(), strbuf.size(), "Edge %s --%s %s%s (%p)\n", edge->Nodes().first->ToStr().c_str(),
                     (TDirected::GetDirected(edge)) ? ">" : "-", edge->Nodes().second->ToStr().c_str(),
                     (TWeighted::GetWeight(edge) == 1.0 ? "" : std::to_string(TWeighted::GetWeight(edge)).c_str()),
                     edge);
            str.append(strbuf.data());
        }
        */
        return str;
    }

  private:
    std::string ToStrNodeEdges(TNode* node) const
    {
        std::string str{"edges "};
        for (const auto& edge : node->Edges())
        {
            if (edge->Nodes().first == node)
            {
                str += TDirected::GetDirected(edge) ? "->" : "";
                str += edge->Nodes().second->ToStr();
                str += TWeighted::GetWeight(edge) == 1.0 ? "" : std::to_string(TWeighted::GetWeight(edge));
            }
            else
            {
                str += TDirected::GetDirected(edge) ? "<-" : "";
                str += edge->Nodes().first->ToStr();
                str += TWeighted::GetWeight(edge) == 1.0 ? "" : std::to_string(TWeighted::GetWeight(edge));
            }
            str += " ";
        }
        return str;
    }

    std::unordered_map<TNodeId, TNode*> m_nodes;
    std::unordered_set<TEdge*> m_edges;
};

}  // namespace GG