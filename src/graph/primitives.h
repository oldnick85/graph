// Copyright 2024 oldnick85

#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

namespace GG
{

template <typename TNodeId>
class Node;
template <typename TNode>
class Edge;

std::string Id2Str(int id)
{
    return std::to_string(id);
}

std::string Id2Str(const std::string& id)
{
    return id;
}

template <typename TNodeId>
class Node
{
  public:
    using NodeId_t = TNodeId;

    explicit Node(const TNodeId& id) : m_id(id) {}
    void AddEdge(Edge<Node>* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        m_edges.push_back(edge);
    }

    void DelEdge(Edge<Node>* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        std::vector<Edge<Node>*> edges;
        edges.reserve(m_edges.size());
        for (auto edge_ : m_edges)
        {
            if (edge_ != edge)
                edges.push_back(edge_);
        }
        m_edges = std::move(edges);
    }

    const std::vector<Edge<Node>*>& Edges() const { return m_edges; }
    const TNodeId& Id() const { return m_id; }

    std::string ToStr() const { return Id2Str(m_id); }

  private:
    TNodeId m_id;
    std::vector<Edge<Node>*> m_edges;
};

template <typename TNode>
class Edge
{
  public:
    using Node_t = TNode;

    explicit Edge(std::pair<Node_t*, Node_t*>& nodes) : m_nodes(nodes) {}
    Edge(Node_t* node1, Node_t* node2, bool directed = false)
        : m_nodes(std::make_pair(node1, node2)), m_directed(directed)
    {
        GRAPH_DEBUG_ASSERT(m_nodes.first != nullptr, "Null node");
        GRAPH_DEBUG_ASSERT(m_nodes.second != nullptr, "Null node");
    }

    const std::pair<Node_t*, Node_t*>& Nodes() const { return m_nodes; }
    float Weight() const { return m_weight; }
    bool Directed() const { return m_directed; }

  private:
    std::pair<Node_t*, Node_t*> m_nodes;
    float m_weight  = 1.0;
    bool m_directed = false;
};

}  // namespace GG