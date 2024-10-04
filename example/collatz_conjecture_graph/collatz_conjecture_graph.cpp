// Copyright 2024 oldnick85

#include <array>
#include <cstdint>

#include "./graph_inclusive.h"
#include "./primitives.h"
#include "./properties/all.h"

using Node_t  = GG::Node<uint64_t>;
using Edge_t  = GG::Edge<Node_t>;
using Graph_t = GG::GraphInclusive<Node_t, Edge_t, GG::Directed<Edge_t, true>, GG::Weighted<Edge_t, false>,
                                   GG::ConnectedComponentWatch<Node_t, Edge_t, false>, GG::Named<true>>;

void CheckNumber(uint64_t num, Graph_t& graph)
{
    if ((num < 1) or (num % 2 == 0))
        return;

    auto* node = graph.Find(num);
    if (node != nullptr)
        return;

    if (num == 1)
    {
        graph.MakeNode(num);
        return;
    }

    auto* prev_node = node;
    while (num != 1)
    {
        if (num % 2 == 0)
        {
            num = num / 2;
        }
        else
        {
            node = graph.Find(num);
            if (node != nullptr)
            {
                graph.MakeEdge(prev_node, node, true);
                return;
            }
            auto* node = graph.MakeNode(num);
            if (prev_node != nullptr)
                graph.MakeEdge(prev_node, node, true);
            prev_node = node;
            num       = num * 3 + 1;
        }
    }
    node = graph.Find(num);
    graph.MakeEdge(prev_node, node, true);
}

void CheckNumbers(uint64_t max_num, Graph_t& graph)
{
    for (uint64_t i = 1; i < max_num; i += 2)
    {
        CheckNumber(i, graph);
    }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv)
{
    if (argc != 2)
    {
        printf("Program requires one argument: count of numbers. But %d is given.", argc);
        return 1;
    }

    const uint64_t num = std::stoll(argv[1]);
    Graph_t graph{"COLLATZ"};
    CheckNumbers(num, graph);
    printf("%s\n", graph.ToDOT().c_str());
    return 0;
}