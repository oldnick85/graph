// Copyright 2024 oldnick85

#pragma once

#include <list>
#include <unordered_set>

#include "./graph_inclusive.h"
#include "./primitives.h"

namespace GG
{

template <typename TNode>
class Path
{
  public:
    void push_back(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        m_nodes.push_back(node);
    }

    void push_front(TNode* node)
    {
        GRAPH_DEBUG_ASSERT(node != nullptr, "Null node");
        m_nodes.push_front(node);
    }

    float Length() const { return m_nodes.size(); }

    const std::list<TNode*>& Nodes() const { return m_nodes; }

    std::string ToStr() const
    {
        std::string str{"Path"};
        str += "(len=" + std::to_string(Length()) + ")";
        std::array<char, 256> strbuf;
        for (const auto node : m_nodes)
        {
            snprintf(strbuf.data(), strbuf.size(), " %s", node->ToStr().c_str());
            str.append(strbuf.data());
        }
        return str;
    }

  private:
    std::list<TNode*> m_nodes;
};

template <typename TNode>
class PathFindNode
{
  public:
    using NodeId_t = TNode::NodeId_t;
    using Edge_t   = Edge<PathFindNode<TNode>>;

    template <typename TNode_>
    PathFindNode(TNode_* node, float distance) : m_node(node), m_distance(distance)
    {
        GRAPH_DEBUG_ASSERT(m_node != nullptr, "Null node");
    }

    TNode* BaseNode() const { return m_node; }
    float Distance() const { return m_distance; }

  private:
    TNode* m_node    = nullptr;
    float m_distance = 0.0;

  public:
    const NodeId_t& Id() const { return m_node->Id(); }

    void AddEdge(Edge_t* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        m_edges.push_back(edge);
    }

    const std::vector<Edge_t*>& Edges() const { return m_edges; }

    std::string ToStr() const { return m_node->ToStr(); }

  private:
    std::vector<Edge_t*> m_edges;
};

template <typename TNode>
class Forefront
{
  public:
    using PNode = PathFindNode<TNode>;

    const std::unordered_set<PNode*>& Nodes() const { return m_pnodes; }

    void Add(PNode* pnode)
    {
        GRAPH_DEBUG_ASSERT(pnode != nullptr, "Null pnode");
        m_pnodes.insert(pnode);
        m_max_distance = std::max(m_max_distance, pnode->Distance());
    }

    float MaxDistance() const { return m_max_distance; }

    bool Empty() const { return m_pnodes.empty(); }

    std::string ToStr() const
    {
        std::string str{"Forefront\n"};
        std::array<char, 256> strbuf;
        for (const auto pnode : m_pnodes)
        {
            const auto node = pnode->BaseNode();
            const auto dist = pnode->Distance();
            snprintf(strbuf.data(), strbuf.size(), "Node %s (%p) %f\n", node->ToStr().c_str(), node, dist);
            str.append(strbuf.data());
        }
        return str;
    }

  private:
    std::unordered_set<PNode*> m_pnodes;
    float m_max_distance = 0.0;
};

template <typename TNode, typename TEdge, typename TDirected, typename TWeighted, typename TNamed>
class PathFindContext
{
  public:
    using PNode_t     = PathFindNode<TNode>;
    using PEdge_t     = Edge<PNode_t>;
    using Path_t      = Path<TNode>;
    using Graph_t     = GraphInclusive<TNode, TEdge, TDirected, TWeighted, TNamed>;
    using GraphWave_t = GraphInclusive<PNode_t, PEdge_t, DirectedTrue<PEdge_t>, WeightedFalse<PEdge_t>, NamedFalse>;
    using Forefront_t = Forefront<TNode>;

    PathFindContext(const Graph_t* graph, TNode* start) : m_graph(graph), m_start(start)
    {
        GRAPH_DEBUG_ASSERT(m_graph != nullptr, "Null graph");
        GRAPH_DEBUG_ASSERT(m_start != nullptr, "Null start");
        auto pnode = new PNode_t{m_start, 0.0};
        m_forefront.Add(pnode);
        m_wave.Add(pnode);
    }

    ~PathFindContext() { m_wave.DeleteAll(); }

    TNode* Start() const { return m_start; }

    void Step()
    {
        if (Exhausted())
            return;

        Forefront_t new_forefront;
        const float dist = m_forefront.MaxDistance() + 1.0;
        for (PNode_t* pnode : m_forefront.Nodes())
        {
            auto node = pnode->BaseNode();
            for (auto edge : node->Edges())
            {
                auto node2 = edge->Nodes().first;
                if (node2 == node)
                    node2 = edge->Nodes().second;
                if (m_wave.Find(node2->Id()) != nullptr)
                    continue;
                auto new_pnode = new PNode_t{node2, dist};
                new_forefront.Add(new_pnode);
                m_wave.Add(new_pnode);
                auto new_edge = new PEdge_t{new_pnode, pnode, true};
                pnode->AddEdge(new_edge);
                new_pnode->AddEdge(new_edge);
                m_wave.Add(new_edge);
            }
        }
        m_forefront = std::move(new_forefront);
    }

    bool Exhausted() const { return m_forefront.Empty(); }

    float DistanceTo(TNode* target) const
    {
        auto pnode = m_wave.Find(target->Id());
        if (pnode == nullptr)
            return 0.0;
        return pnode->Distance();
    }

    Path_t FindPathTo(TNode* target)
    {
        Path_t path;
        while (not Exhausted())
        {
            Step();
            path = std::move(PathTo(target));
            if (path.Length() > 0)
                break;
        }
        return path;
    }

    Path_t PathTo(TNode* target) const
    {
        auto ptarget = m_wave.Find(target->Id());
        Path_t path;
        if (ptarget == nullptr)
            return path;
        path.push_front(ptarget->BaseNode());
        while (ptarget->BaseNode() != m_start)
        {
            auto pedge = ptarget->Edges().front();
            ptarget    = pedge->Nodes().second;
            path.push_front(ptarget->BaseNode());
        }
        return path;
    }

    std::string ToStr() const
    {
        std::string str{"PathFindContext\n"};
        if (Exhausted())
            str += "EXHAUSTED\n";
        str += m_wave.ToStr();
        str += m_forefront.ToStr();
        return str;
    }

  private:
    const Graph_t* m_graph = nullptr;
    TNode* m_start         = nullptr;

    GraphWave_t m_wave;
    Forefront_t m_forefront;
};

}  // namespace GG