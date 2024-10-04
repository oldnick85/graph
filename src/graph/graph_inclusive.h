// Copyright 2024 oldnick85

#pragma once

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "./common.h"

namespace GG
{

template <typename R, typename... Args>
R return_type_of(R (*)(Args...));

/**
 * \~english
 * @brief Graph class that stores nodes and edges
 * 
 * @tparam TNodeId node id type
 * @tparam TNode node type
 * @tparam TEdge edge type
 * @tparam TDirected directed graph property
 * @tparam TWeighted weighted graph property
 */
/**
 * \~russian
 * @brief Класс графа, хранящий в себе вершигны и рёбра
 * 
 * @tparam TNodeId тип идентификатора вершины
 * @tparam TNode тип вершины
 * @tparam TEdge тип ребра
 * @tparam TDirected свойство направленности графа
 * @tparam TWeighted свойство взвешенности графа
 */
template <typename TNode, typename TEdge, typename TDirected, typename TWeighted, typename TConnectedComponentWatch,
          typename TNamed>
class GraphInclusive : public TDirected, public TWeighted, public TNamed, public TConnectedComponentWatch
{
  public:
    using TNodeId = TNode::NodeId_t;

    GraphInclusive() = default;

    GraphInclusive(const std::string& name) : TNamed(name) {}

    template <class... Args>
    TNode* MakeNode(Args... args)
    {
        auto node = new TNode(args...);
        Add(node);
        return node;
    }

    const std::unordered_map<TNodeId, TNode*>& Nodes() const { return m_nodes; }

    TEdge* MakeEdge(TNodeId node1_id, TNodeId node2_id, bool directed = false)
    {
        auto node1 = Find(node1_id);
        if (node1 == nullptr)
            return nullptr;
        auto node2 = Find(node2_id);
        if (node2 == nullptr)
            return nullptr;
        return MakeEdge(node1, node2, directed);
    }

    TEdge* MakeEdge(TNode* node1, TNode* node2, bool directed = false)
    {
        auto* edge = new TEdge(node1, node2, directed);
        node1->AddEdge(edge);
        node2->AddEdge(edge);
        Add(edge);
        return edge;
    }

    void Del(const TNodeId& id)
    {
        const auto node_it = m_nodes.find(id);
        if (node_it == m_nodes.end())
            return;
        Del(node_it->second);
    }

    void Del(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        const auto node_it = m_nodes.find(node->Id());
        if (node_it == m_nodes.end())
            return;
        GRAPH_DEBUG_ASSERT(node == node_it->second, "Wrong nodes in graph");
        std::vector<TEdge*> edges = node->Edges();
        for (auto edge : edges)
        {
            Del(edge);
        }
        m_nodes.erase(node_it);
        TConnectedComponentWatch::onDel(node);
        delete node;
    }

    void Del(TEdge* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        auto node1 = edge->Nodes().first;
        GRAPH_DEBUG_ASSERT(node1 != nullptr, "Null node in edge");
        GRAPH_DEBUG_ASSERT(Find(node1->Id()) != nullptr, "No node id in graph");
        GRAPH_DEBUG_ASSERT(Find(node1->Id()) == node1, "No node in graph");
        node1->DelEdge(edge);
        auto node2 = edge->Nodes().second;
        GRAPH_DEBUG_ASSERT(node2 != nullptr, "Null node in edge");
        GRAPH_DEBUG_ASSERT(Find(node2->Id()) != nullptr, "No node id in graph");
        GRAPH_DEBUG_ASSERT(Find(node2->Id()) == node2, "No node in graph");
        node2->DelEdge(edge);
        m_edges.erase(edge);
        TConnectedComponentWatch::onDel(edge);
        delete edge;
    }

    void DelEdgesTo(TNodeId node_from_id, TNodeId node_to_id)
    {
        auto node_from = Find(node_from_id);
        if (node_from == nullptr)
            return;
        auto node_to = Find(node_to_id);
        if (node_to == nullptr)
            return;
        DelEdgesTo(node_from, node_to);
    }

    void DelEdgesTo(TNode* node_from, TNode* node_to)
    {
        GRAPH_DEBUG_ASSERT(node_from != nullptr, "Null node from");
        GRAPH_DEBUG_ASSERT(node_to != nullptr, "Null node to");
        auto edges = node_from->Edges();
        std::vector<TEdge> edges_to_delete;
        edges_to_delete.reserve(edges.size());
        for (auto edge : edges)
        {
            if (TDirected::GetDirected(edge))
            {
                if (edge->Nodes().second == node_to)
                    edges_to_delete.push_back(edge);
            }
            else
            {
                if ((edge->Nodes().second == node_from) or (edge->Nodes().second == node_to))
                    edges_to_delete.push_back(edge);
            }
        }
        for (auto edge : edges_to_delete)
        {
            Del(edge);
        }
    }

    void DelEdgesBetween(TNodeId node1_id, TNodeId node2_id)
    {
        auto node1 = Find(node1_id);
        if (node1 == nullptr)
            return;
        auto node2 = Find(node2_id);
        if (node2 == nullptr)
            return;
        DelEdgesBetween(node1, node2);
    }

    void DelEdgesBetween(TNode* node1, TNode* node2)
    {
        GRAPH_DEBUG_ASSERT(node1 != nullptr, "Null node 1");
        GRAPH_DEBUG_ASSERT(node2 != nullptr, "Null node 2");
        auto edges = node1->Edges();
        std::vector<TEdge*> edges_to_delete;
        edges_to_delete.reserve(edges.size());
        for (auto edge : edges)
        {
            if (((edge->Nodes().first == node1) and (edge->Nodes().second == node2)) or
                ((edge->Nodes().first == node2) and (edge->Nodes().second == node1)))
                edges_to_delete.push_back(edge);
        }
        for (auto edge : edges_to_delete)
        {
            Del(edge);
        }
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

    void Clear()
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
        TConnectedComponentWatch::Clear();
    }

    std::string ToDOT_Body() const
    {
        std::string str;
        for (const auto& node_el : m_nodes)
        {
            const auto& node = node_el.second;
            str += std::to_string(reinterpret_cast<u_int64_t>(node)) + " [label=\"" + Id2Str(node->Id()) + "\"];\n";
        }
        for (const auto& edge : m_edges)
        {
            const auto node1           = edge->Nodes().first;
            const auto node2           = edge->Nodes().second;
            const std::string edge_str = TDirected::GetDirected(edge) ? " -> " : " -- ";
            str += std::to_string(reinterpret_cast<u_int64_t>(node1)) + edge_str +
                   std::to_string(reinterpret_cast<u_int64_t>(node2)) + ";\n";
        }
        return str;
    }

    std::string ToDOT_Body(std::function<std::string(TNode*)> node_printer) const
    {
        std::string str;
        for (const auto& node_el : m_nodes)
        {
            const auto& node = node_el.second;
            str += std::to_string(reinterpret_cast<u_int64_t>(node)) + " [label=\"" + node_printer(node) + "\"];\n";
        }
        for (const auto& edge : m_edges)
        {
            const auto node1           = edge->Nodes().first;
            const auto node2           = edge->Nodes().second;
            const std::string edge_str = TDirected::GetDirected(edge) ? " -> " : " -- ";
            str += std::to_string(reinterpret_cast<u_int64_t>(node1)) + edge_str +
                   std::to_string(reinterpret_cast<u_int64_t>(node2)) + ";\n";
        }
        return str;
    }

    std::string ToDOT() const
    {
        std::string str;
        if (TDirected::IsDirected)
            str += "di";
        str += "graph \"";
        str += TNamed::GetName();
        str += "\" {\n";
        str += ToDOT_Body();
        str += "}\n";
        return str;
    }

    std::string ToDOT(std::function<std::string(TNode*)> node_printer) const
    {
        std::string str;
        if (TDirected::IsDirected)
            str += "di";
        str += "graph \"";
        str += TNamed::GetName();
        str += "\" {\n";
        str += ToDOT_Body(node_printer);
        str += "}\n";
        return str;
    }

    std::string ToLatexDOT() const
    {
        std::string str;
        /*
        if (TDirected::IsDirected)
            str += R"GG(\digraph{)GG";
        else
            str += R"GG(\graph{)GG";
        */
        str += R"GG(\digraph{)GG";
        str += TNamed::GetName() + std::to_string(abs(rand()));
        str += "}{\n";
        str += "rankdir=TB;\n";
        str += ToDOT_Body();
        str += "}\n";

        const std::string replace_word = " -- ";
        const std::string replace_by   = " -> ";
        size_t pos                     = str.find(replace_word);
        while (pos != std::string::npos)
        {
            str.replace(pos, replace_word.size(), replace_by);
            pos = str.find(replace_word, pos + replace_by.size());
        }

        return str;
    }

    std::string ToLatexDOT(std::function<std::string(TNode*)> node_printer) const
    {
        std::string str;
        if (TDirected::IsDirected)
            str += R"GG(\digraph{)GG";
        else
            str += R"GG(\graph{)GG";
        str += TNamed::GetName();
        str += "{\n";
        str += "rankdir=LR;\n";
        str += ToDOT_Body(node_printer);
        str += "}\n";
        return str;
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
        std::string str{"GraphInclusive("};
        str += TNamed::GetName();
        str += ")\n";
        std::array<char, 256> strbuf;
        for (const auto& node_el : m_nodes)
        {
            const auto& node = node_el.second;
            snprintf(strbuf.data(), strbuf.size(), "Node %s ", node->ToStr().c_str());
            str.append(strbuf.data());
            str.append(ToStrNodeEdges(node));
            str += "\n";
        }
        return str;
    }

  private:
    /**
     * @brief Add node
     * 
     * @param node node
     */
    void Add(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        m_nodes.emplace(node->Id(), node);
        TConnectedComponentWatch::onAdd(node);
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
        TConnectedComponentWatch::onAdd(edge);
    }

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