// Copyright 2024 oldnick85

#include <array>

#include <gtest/gtest.h>

#include "./area.h"
#include "./graph_inclusive.h"
#include "./path_find.h"
#include "./primitives.h"

// NOLINTBEGIN
TEST(GraphInclusive, Base)
{
    using Node_t = GG::Node<int>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::Directed<Edge_t, false>, GG::Weighted<Edge_t, false>,
                       GG::ConnectedComponentWatch<Node_t, Edge_t, false>, GG::Named<false>>
        graph;
    auto* node1 = graph.MakeNode(1);
    ASSERT_EQ(graph.Find(1), node1);
    ASSERT_EQ(graph.Find(1)->Id(), 1);
    auto* node2 = graph.MakeNode(2);
    ASSERT_EQ(graph.Find(2), node2);
    ASSERT_EQ(graph.Find(2)->Id(), 2);
    graph.MakeEdge(node1, node2);
    ASSERT_TRUE(graph.CheckCorrect());
}

TEST(GraphInclusive, ConnectionComponent)
{
    using Node_t = GG::Node<int>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::Directed<Edge_t, false>, GG::Weighted<Edge_t, false>,
                       GG::ConnectedComponentWatch<Node_t, Edge_t, true>, GG::Named<false>>
        graph;
    ASSERT_EQ(graph.ConnectedComponentsCount(), 0);
    /*
     *  1
     */
    graph.MakeNode(1);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 1);
    /*
     *  1   2
     */
    graph.MakeNode(2);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 2);
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    /*
     *  1 - 2
     */
    graph.MakeEdge(1, 2);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 1);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    /*
     *  1 - 2
     *
     *  3
     */
    graph.MakeNode(3);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 2);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(2), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(2), graph.Find(3)));
    /*
     *  1 - 2
     *
     *  3   4
     */
    graph.MakeNode(4);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 3);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(2), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(2), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(2), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(2), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(3), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(3), graph.Find(4)));
    /*
     *  1 - 2
     *
     *  3 - 4
     */
    graph.MakeEdge(3, 4);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 2);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(2), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(2), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(2), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(2), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(3), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(3), graph.Find(4)));
    /*
     *  1 - 2
     *  |
     *  3 - 4
     */
    graph.MakeEdge(1, 3);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 1);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(2), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(2), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(2), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(2), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(3), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(3), graph.Find(4)));
    /*
     *  1 - 2
     *  |   |
     *  3 - 4
     */
    graph.MakeEdge(2, 4);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 1);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(2), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(2), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(2), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(2), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(3), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(3), graph.Find(4)));
    /*
     *  1 - 2
     *  |   |
     *  3   4
     */
    graph.DelEdgesBetween(3, 4);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 1);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(2)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(2)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(2), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(2), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(2), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(2), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(3), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(3), graph.Find(4)));
    /*
     *  1
     *  |
     *  3   4
     */
    graph.Del(2);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 2);
    ASSERT_TRUE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(3), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(3), graph.Find(4)));
    /*
     *  1
     * 
     *  3   4
     */
    graph.DelEdgesBetween(3, 1);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 3);
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(4)));
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(3), graph.Find(4)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(3), graph.Find(4)));
    /*
     *  1
     * 
     *  3
     */
    graph.Del(4);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 2);
    ASSERT_FALSE(graph.SurelyConnected(graph.Find(1), graph.Find(3)));
    ASSERT_TRUE(graph.SurelyNotConnected(graph.Find(1), graph.Find(3)));
    /*
     *  1
     */
    graph.Del(3);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 1);
    /*
     *  
     */
    graph.Del(1);
    ASSERT_EQ(graph.ConnectedComponentsCount(), 0);
}

TEST(GraphInclusive, BasePathFind)
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
    GG::GraphInclusive<Node_t, Edge_t, GG::Directed<Edge_t, false>, GG::Weighted<Edge_t, false>,
                       GG::ConnectedComponentWatch<Node_t, Edge_t, false>, GG::Named<false>>
        graph;
    std::array<Node_t*, 10> node;
    for (int i = 0; i < static_cast<int>(node.size()); ++i)
    {
        node[i] = graph.MakeNode(i);
    }

    for (int i = 0; i < static_cast<int>(node.size()); ++i)
    {
        ASSERT_EQ(graph.Find(i), node[i]);
        ASSERT_EQ(graph.Find(i)->Id(), i);
    }

    graph.MakeEdge(0, 3);
    graph.MakeEdge(1, 2);
    graph.MakeEdge(1, 3);
    graph.MakeEdge(1, 4);
    graph.MakeEdge(1, 5);
    graph.MakeEdge(2, 9);
    graph.MakeEdge(3, 4);
    graph.MakeEdge(4, 6);
    graph.MakeEdge(5, 7);
    graph.MakeEdge(6, 7);
    graph.MakeEdge(7, 8);

    ASSERT_TRUE(graph.CheckCorrect());

    GG::PathFindContext path_find_context{&graph, node[1]};
    ASSERT_FALSE(path_find_context.Exhausted());

    path_find_context.Step();
    ASSERT_FALSE(path_find_context.Exhausted());

    path_find_context.Step();
    ASSERT_FALSE(path_find_context.Exhausted());

    path_find_context.Step();
    ASSERT_FALSE(path_find_context.Exhausted());

    path_find_context.Step();
    ASSERT_TRUE(path_find_context.Exhausted());

    auto path = path_find_context.PathTo(node[8]);
    ASSERT_EQ(path.Nodes().size(), 4);
    auto path_it = path.Nodes().begin();
    ASSERT_EQ(*path_it, node[1]);
    ++path_it;
    ASSERT_EQ(*path_it, node[5]);
    ++path_it;
    ASSERT_EQ(*path_it, node[7]);
    ++path_it;
    ASSERT_EQ(*path_it, node[8]);
}

TEST(Area2D, BaseMoore)
{
    using Node_t = GG::Node<GG::Coord2D>;
    GG::Area2D<Node_t, GG::NeighborhoodMoore> area(GG::Range2D(GG::Coord2D(4, 3)));
    area.SetPassable({1, 2}, true);
    area.SetPassable({2, 3}, true);
    area.SetPassable({2, 2}, true);
    area.SetPassable({2, 3}, false);

    area.SetPassableAll(true);
    area.SetPassable({0, 2}, false);
    area.SetPassable({1, 2}, false);
    area.SetPassable({3, 2}, false);
    GG::PathFindContext path_find_context{&(area.Graph()), area.Graph().Find(GG::Coord2D(0, 0))};
    auto path = path_find_context.FindPathTo(area.Graph().Find(GG::Coord2D(4, 3)));
    ASSERT_EQ(path.Length(), 5.0);
}

TEST(Area2D, BaseVonNeumann)
{
    using Node_t = GG::Node<GG::Coord2D>;
    GG::Area2D<Node_t, GG::NeighborhoodVonNeumann> area(GG::Range2D(GG::Coord2D(4, 3)));
    area.SetPassable({1, 2}, true);
    area.SetPassable({2, 3}, true);
    area.SetPassable({2, 2}, true);
    area.SetPassable({2, 3}, false);

    area.SetPassableAll(true);
    area.SetPassable({0, 2}, false);
    area.SetPassable({1, 2}, false);
    area.SetPassable({3, 2}, false);
    GG::PathFindContext path_find_context{&(area.Graph()), area.Graph().Find(GG::Coord2D(0, 0))};
    auto path = path_find_context.FindPathTo(area.Graph().Find(GG::Coord2D(4, 3)));
    ASSERT_EQ(path.Length(), 8.0);
}

TEST(Area2D, BaseHex)
{
    using Node_t = GG::Node<GG::Coord2D>;
    GG::Area2D<Node_t, GG::NeighborhoodHex> area(GG::Range2D(GG::Coord2D(4, 3)));
    area.SetPassable({1, 2}, true);
    area.SetPassable({2, 3}, true);
    area.SetPassable({2, 2}, true);
    area.SetPassable({2, 3}, false);

    area.SetPassableAll(true);
    area.SetPassable({0, 2}, false);
    area.SetPassable({1, 2}, false);
    area.SetPassable({3, 2}, false);
    GG::PathFindContext path_find_context{&(area.Graph()), area.Graph().Find(GG::Coord2D(0, 0))};
    auto path = path_find_context.FindPathTo(area.Graph().Find(GG::Coord2D(4, 3)));
    ASSERT_EQ(path.Length(), 7.0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
// NOLINTEND