// Copyright 2024 oldnick85

#pragma once

#include <list>
#include <unordered_set>

#include "./graph_inclusive.h"
#include "./primitives.h"
#include "./properties/all.h"

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
    /**
     * \~english
     * @brief Get node identificator
     * 
     * @return node identificator
     */
    /**
     * \~russian
     * @brief Получить идентификатор узла
     * 
     * @return идентификатор узла
     */
    const NodeId_t& Id() const { return m_node->Id(); }

    /**
     * \~english
     * @brief Add edge
     * 
     * @param edge edge
     */
    /**
     * \~russian
     * @brief Добавить ребро
     * 
     * @param edge ребро
     */
    void AddEdge(Edge_t* edge)
    {
        GRAPH_DEBUG_ASSERT(edge != nullptr, "Null edge");
        m_edges.push_back(edge);
    }

    /**
     * \~english
     * @brief Get edges for this node
     * 
     * @return edges for this node
     */
    /**
     * \~russian
     * @brief Получить рёбра этой вершины
     * 
     * @return рёбра этой вершины
     */
    const std::vector<Edge_t*>& Edges() const { return m_edges; }

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

template <typename TNode, typename TEdge, typename TDirected, typename TWeighted, typename TConnectedComponentWatch,
          typename TNamed>
class PathFindContext
{
  public:
    using PNode_t     = PathFindNode<TNode>;
    using PEdge_t     = Edge<PNode_t>;
    using Path_t      = Path<TNode>;
    using Graph_t     = GraphInclusive<TNode, TEdge, TDirected, TWeighted, TConnectedComponentWatch, TNamed>;
    using GraphWave_t = GraphInclusive<PNode_t, PEdge_t, GG::Directed<PEdge_t, true>, Weighted<PEdge_t, false>,
                                       ConnectedComponentWatch<PNode_t, PEdge_t, false>, Named<false>>;
    using Forefront_t = Forefront<TNode>;

    PathFindContext(const Graph_t* graph, TNode* start) : m_graph(graph), m_start(start)
    {
        GRAPH_DEBUG_ASSERT(m_graph != nullptr, "Null graph");
        GRAPH_DEBUG_ASSERT(m_start != nullptr, "Null start");
        auto pnode = m_wave.MakeNode(m_start, 0.0);
        m_forefront.Add(pnode);
    }

    ~PathFindContext() { m_wave.Clear(); }

    TNode* Start() const { return m_start; }

    void Step()
    {
        if (Exhausted())
            return;

        if (Graph_t::IsWeighted)
        {
            GRAPH_DEBUG_ASSERT(false, "Not implemented");
        }
        else
        {
            StepWaveAlgorithm();
        }
    }

    void StepWaveAlgorithm()
    {
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
                auto new_pnode = m_wave.MakeNode(node2, dist);
                new_forefront.Add(new_pnode);
                m_wave.MakeEdge(new_pnode, pnode, true);
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

    void SpreadWave()
    {
        while (not Exhausted())
        {
            Step();
        }
    }

    std::vector<TNode*> WaveNodes()
    {
        std::vector<TNode*> nodes;
        auto pnodes = m_wave.Nodes();
        nodes.reserve(pnodes.size());
        for (const auto& pnode_it : pnodes)
            nodes.push_back(pnode_it.second->BaseNode());
        return nodes;
    }

    Path_t FindPathTo(TNode* target)
    {
        Path_t path;
        if (m_graph->SurelyNotConnected(m_start, target))
            return path;
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
        std::string str{"PathFindContext\n"};
        if (Exhausted())
            str += "EXHAUSTED\n";
        str += m_wave.ToStr();
        str += m_forefront.ToStr();
        return str;
    }

    std::string ToDOT() const
    {
        auto node_printer = [&](TNode* node) -> std::string {
            const auto id = node->Id();
            auto pnode    = m_wave.Find(id);
            std::string str;
            str += Id2Str(id);
            if (pnode != nullptr)
                str += std::string(" d=") + std::to_string(pnode->Distance());
            return str;
        };
        return m_graph->ToDOT(node_printer);
    }

  private:
    const Graph_t* m_graph = nullptr;
    TNode* m_start         = nullptr;

    GraphWave_t m_wave;
    Forefront_t m_forefront;
};

}  // namespace GG