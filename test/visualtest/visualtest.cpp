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
    GG::GraphInclusive<Node_t, Edge_t, GG::DirectedFalse<Edge_t>, GG::WeightedFalse<Edge_t>,
                       GG::ConnectedComponentWatchFalse<Node_t, Edge_t>, GG::NamedFalse>
        graph;
    graph.MakeNode(1);
    graph.MakeNode(2);
    graph.MakeEdge(1, 2);
    printf("%s\n", graph.ToStr().c_str());
}

void Test_GraphInclusive_ConnectionComponent()
{
    using Node_t = GG::Node<int>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::DirectedFalse<Edge_t>, GG::WeightedFalse<Edge_t>,
                       GG::ConnectedComponentWatchTrue<Node_t, Edge_t>, GG::NamedFalse>
        graph;
    graph.MakeNode(1);
    graph.MakeNode(2);
    graph.MakeEdge(1, 2);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.MakeNode(3);
    graph.MakeNode(4);
    graph.MakeEdge(3, 4);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.MakeEdge(1, 3);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.MakeEdge(2, 4);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.DelEdgesBetween(3, 4);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.Del(2);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.DelEdgesBetween(3, 1);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.Del(4);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.Del(3);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());

    graph.Del(1);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());
}

void Test_GraphInclusive_DOT()
{
    using Node_t = GG::Node<std::string>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::DirectedFalse<Edge_t>, GG::WeightedFalse<Edge_t>,
                       GG::ConnectedComponentWatchFalse<Node_t, Edge_t>, GG::NamedFalse>
        graph;
    graph.MakeNode("a");
    graph.MakeNode("b");
    graph.MakeEdge("a", "b");
    graph.MakeNode("c");
    graph.MakeNode("d");
    graph.MakeEdge("c", "b");
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
    GG::GraphInclusive<Node_t, Edge_t, GG::DirectedFalse<Edge_t>, GG::WeightedFalse<Edge_t>,
                       GG::ConnectedComponentWatchFalse<Node_t, Edge_t>, GG::NamedFalse>
        graph;
    std::array<Node_t*, 10> node;
    for (int i = 0; i < static_cast<int>(node.size()); ++i)
    {
        node[i] = graph.MakeNode(i);
    }

    graph.MakeEdge(node[0], node[3]);
    graph.MakeEdge(node[1], node[2]);
    graph.MakeEdge(node[1], node[3]);
    graph.MakeEdge(node[1], node[4]);
    graph.MakeEdge(node[1], node[5]);
    graph.MakeEdge(node[2], node[9]);
    graph.MakeEdge(node[3], node[4]);
    graph.MakeEdge(node[4], node[6]);
    graph.MakeEdge(node[5], node[7]);
    graph.MakeEdge(node[6], node[7]);
    graph.MakeEdge(node[7], node[8]);

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
    Test_GraphInclusive_ConnectionComponent();
    return 0;
}
// NOLINTEND