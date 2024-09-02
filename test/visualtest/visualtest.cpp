// Copyright 2024 oldnick85

#include <array>

#include <area.h>
#include <graph_inclusive.h>
#include <path_find.h>
#include <primitives.h>

// NOLINTBEGIN
void Test_GraphInclusive_Base()
{
    using Node_t = GG::Node<int>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::DirectedFalse<Edge_t>, GG::WeightedFalse<Edge_t>, GG::NamedFalse> graph;
    auto* node1 = new Node_t(1);
    graph.Add(node1);
    auto* node2 = new Node_t(2);
    graph.Add(node2);
    auto* edge12 = new Edge_t(node1, node2);
    graph.Add(edge12);
    node1->AddEdge(edge12);
    node2->AddEdge(edge12);
    printf("%s\n", graph.ToStr().c_str());
}

void Test_GraphInclusive_DOT()
{
    using Node_t = GG::Node<std::string>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::DirectedFalse<Edge_t>, GG::WeightedFalse<Edge_t>, GG::NamedFalse> graph;
    auto* node_a = new Node_t("a");
    graph.Add(node_a);
    auto* node_b = new Node_t("b");
    graph.Add(node_b);
    graph.MakeEdge(node_a, node_b);
    auto* node_c = new Node_t("c");
    graph.Add(node_c);
    auto* node_d = new Node_t("d");
    graph.Add(node_d);
    graph.MakeEdge(node_c, node_b);
    graph.MakeEdge(node_b, node_d);
    printf("%s\n", graph.ToDOT().c_str());
}

void Test_GraphInclusive_BasePathFind()
{
    using Node_t = GG::Node<int>;
    using Edge_t = GG::Edge<Node_t>;
    /*
    *          2 - 9
    *          |
    *  0 - 3 - 1 - 5
    *        \ |     \
    *          4 - 6 - 7 - 8
    */
    GG::GraphInclusive<Node_t, Edge_t, GG::DirectedFalse<Edge_t>, GG::WeightedFalse<Edge_t>, GG::NamedFalse> graph;
    std::array<Node_t*, 10> node;
    for (int i = 0; i < static_cast<int>(node.size()); ++i)
    {
        node[i] = new Node_t(i);
        graph.Add(node[i]);
    }

    auto* edge03 = new Edge_t(node[0], node[3]);
    graph.Add(edge03);
    node[0]->AddEdge(edge03);
    node[3]->AddEdge(edge03);

    auto* edge12 = new Edge_t(node[1], node[2]);
    graph.Add(edge12);
    node[1]->AddEdge(edge12);
    node[2]->AddEdge(edge12);

    auto* edge13 = new Edge_t(node[1], node[3]);
    graph.Add(edge13);
    node[1]->AddEdge(edge13);
    node[3]->AddEdge(edge13);

    auto* edge14 = new Edge_t(node[1], node[4]);
    graph.Add(edge14);
    node[1]->AddEdge(edge14);
    node[4]->AddEdge(edge14);

    auto* edge15 = new Edge_t(node[1], node[5]);
    graph.Add(edge15);
    node[1]->AddEdge(edge15);
    node[5]->AddEdge(edge15);

    auto* edge29 = new Edge_t(node[2], node[9]);
    graph.Add(edge29);
    node[2]->AddEdge(edge29);
    node[9]->AddEdge(edge29);

    auto* edge34 = new Edge_t(node[3], node[4]);
    graph.Add(edge34);
    node[3]->AddEdge(edge34);
    node[4]->AddEdge(edge34);

    auto* edge46 = new Edge_t(node[4], node[6]);
    graph.Add(edge46);
    node[4]->AddEdge(edge46);
    node[6]->AddEdge(edge46);

    auto* edge57 = new Edge_t(node[5], node[7]);
    graph.Add(edge57);
    node[5]->AddEdge(edge57);
    node[7]->AddEdge(edge57);

    auto* edge67 = new Edge_t(node[6], node[7]);
    graph.Add(edge67);
    node[6]->AddEdge(edge67);
    node[7]->AddEdge(edge67);

    auto* edge78 = new Edge_t(node[7], node[8]);
    graph.Add(edge78);
    node[7]->AddEdge(edge78);
    node[8]->AddEdge(edge78);

    printf("%s\n", graph.ToStr().c_str());

    GG::PathFindContext path_find_context{&graph, node[1]};
    printf("%s\n", path_find_context.ToStr().c_str());

    path_find_context.Step();
    printf("%s\n", path_find_context.ToStr().c_str());

    path_find_context.Step();
    printf("%s\n", path_find_context.ToStr().c_str());

    path_find_context.Step();
    printf("%s\n", path_find_context.ToStr().c_str());

    path_find_context.Step();
    printf("%s\n", path_find_context.ToStr().c_str());

    auto path = path_find_context.PathTo(node[8]);
    printf("%s\n", path.ToStr().c_str());
}

void Test_Area2D_BaseMoore()
{
    using Node_t = GG::Node<GG::Coord2D>;
    GG::Area2D<Node_t, GG::NeighborhoodMoore> area(GG::Range2D(GG::Coord2D(4, 3)));
    area.SetPassable({1, 2}, true);
    area.SetPassable({2, 3}, true);
    area.SetPassable({2, 2}, true);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());
    area.SetPassable({2, 3}, false);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());

    area.SetPassableAll(true);
    area.SetPassable({0, 2}, false);
    area.SetPassable({1, 2}, false);
    area.SetPassable({3, 2}, false);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());
    GG::PathFindContext path_find_context{&(area.Graph()), area.Graph().Find(GG::Coord2D(0, 0))};
    auto path = path_find_context.FindPathTo(area.Graph().Find(GG::Coord2D(4, 3)));
    printf("%s\n", path.ToStr().c_str());
    printf("%s\n", area.ToStrASCII(&path_find_context).c_str());
}

void Test_Area2D_BaseVonNeumann()
{
    using Node_t = GG::Node<GG::Coord2D>;
    GG::Area2D<Node_t, GG::NeighborhoodVonNeumann> area(GG::Range2D(GG::Coord2D(4, 3)));
    area.SetPassable({1, 2}, true);
    area.SetPassable({2, 3}, true);
    area.SetPassable({2, 2}, true);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());
    area.SetPassable({2, 3}, false);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());

    area.SetPassableAll(true);
    area.SetPassable({0, 2}, false);
    area.SetPassable({1, 2}, false);
    area.SetPassable({3, 2}, false);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());
    GG::PathFindContext path_find_context{&(area.Graph()), area.Graph().Find(GG::Coord2D(0, 0))};
    auto path = path_find_context.FindPathTo(area.Graph().Find(GG::Coord2D(4, 3)));
    printf("%s\n", path.ToStr().c_str());
    printf("%s\n", area.ToStrASCII(&path_find_context).c_str());
}

void Test_Area2D_BaseHex()
{
    using Node_t = GG::Node<GG::Coord2D>;
    GG::Area2D<Node_t, GG::NeighborhoodHex> area(GG::Range2D(GG::Coord2D(4, 3)));
    area.SetPassable({1, 2}, true);
    area.SetPassable({2, 3}, true);
    area.SetPassable({2, 2}, true);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());
    area.SetPassable({2, 3}, false);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());

    area.SetPassableAll(true);
    area.SetPassable({0, 2}, false);
    area.SetPassable({1, 2}, false);
    area.SetPassable({3, 2}, false);
    printf("%s\n", area.ToStrASCII().c_str());
    printf("%s\n", area.Graph().ToStr().c_str());
    GG::PathFindContext path_find_context{&(area.Graph()), area.Graph().Find(GG::Coord2D(0, 0))};
    auto path = path_find_context.FindPathTo(area.Graph().Find(GG::Coord2D(4, 3)));
    printf("%s\n", path.ToStr().c_str());
    printf("%s\n", area.ToStrASCII(&path_find_context).c_str());
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv)
{
    Test_GraphInclusive_Base();
    Test_GraphInclusive_DOT();
    Test_GraphInclusive_BasePathFind();
    Test_Area2D_BaseMoore();
    Test_Area2D_BaseVonNeumann();
    Test_Area2D_BaseHex();
    return 0;
}
// NOLINTEND