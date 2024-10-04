// Copyright 2024 oldnick85

#include <array>
#include <cstring>
#include <fstream>

#include <area.h>
#include <graph_inclusive.h>
#include <path_find.h>
#include <primitives.h>

// NOLINTBEGIN
void Test_GraphInclusive_Base(std::string& latex_str)
{
    latex_str += R"GG(\section{Graph Inclusive: Base})GG";
    latex_str += "\n";
    using Node_t = GG::Node<int>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::Directed<Edge_t, false>, GG::Weighted<Edge_t, false>,
                       GG::ConnectedComponentWatch<Node_t, Edge_t, false>, GG::Named<false>>
        graph;
    graph.MakeNode(1);
    graph.MakeNode(2);
    graph.MakeEdge(1, 2);
    printf("%s\n", graph.ToStr().c_str());
    latex_str += graph.ToLatexDOT();
}

void Test_GraphInclusive_ConnectionComponent(std::string& latex_str)
{
    latex_str += R"GG(\section{Graph Inclusive: Connection Component})GG";
    latex_str += "\n";
    using Node_t = GG::Node<int>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::Directed<Edge_t, false>, GG::Weighted<Edge_t, false>,
                       GG::ConnectedComponentWatch<Node_t, Edge_t, true>, GG::Named<false>>
        graph;
    graph.MakeNode(1);
    graph.MakeNode(2);
    graph.MakeEdge(1, 2);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());
    latex_str += graph.ToLatexDOT();
    latex_str += std::string("\nconnected=") + std::to_string(graph.ConnectedComponentsCount()) + "\n";

    graph.MakeNode(3);
    graph.MakeNode(4);
    graph.MakeEdge(3, 4);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());
    latex_str += graph.ToLatexDOT();
    latex_str += std::string("\nconnected=") + std::to_string(graph.ConnectedComponentsCount()) + "\n";

    graph.MakeEdge(1, 3);
    printf("%s\n", graph.ToStr().c_str());
    printf("connected=%u\n", graph.ConnectedComponentsCount());
    latex_str += graph.ToLatexDOT();
    latex_str += std::string("\nconnected=") + std::to_string(graph.ConnectedComponentsCount()) + "\n";

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

void Test_GraphInclusive_DOT(std::string& latex_str)
{
    latex_str += R"GG(\section{Graph Inclusive: DOT})GG";
    latex_str += "\n";
    using Node_t = GG::Node<std::string>;
    using Edge_t = GG::Edge<Node_t>;
    GG::GraphInclusive<Node_t, Edge_t, GG::Directed<Edge_t, false>, GG::Weighted<Edge_t, false>,
                       GG::ConnectedComponentWatch<Node_t, Edge_t, false>, GG::Named<false>>
        graph;
    graph.MakeNode("a");
    graph.MakeNode("b");
    graph.MakeEdge("a", "b");
    graph.MakeNode("c");
    graph.MakeNode("d");
    graph.MakeEdge("c", "b");
    printf("%s\n", graph.ToDOT().c_str());
}

void Test_GraphInclusive_BasePathFind(std::string& latex_str)
{
    latex_str += R"GG(\section{Graph Inclusive: Base Path Find})GG";
    latex_str += "\n";
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
    printf("%s\n", path_find_context.ToDOT().c_str());

    path_find_context.Step();
    printf("%s\n", path_find_context.ToStr().c_str());

    path_find_context.Step();
    printf("%s\n", path_find_context.ToStr().c_str());

    latex_str += graph.ToLatexDOT();
    latex_str += std::string("\n") + "\n";

    auto path = path_find_context.PathTo(node[8]);
    printf("%s\n", path.ToStr().c_str());
}

void Test_Area2D_BaseMoore(std::string& latex_str)
{
    latex_str += R"GG(\section{Area 2D: Base Moore})GG";
    latex_str += "\n";
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
    latex_str += area.ToStrLatex();
    latex_str += std::string("\n") + "\n";
}

void Test_Area2D_BaseVonNeumann(std::string& latex_str)
{
    latex_str += R"GG(\section{Area 2D: Base Von Neumann})GG";
    latex_str += "\n";
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
    latex_str += area.ToStrLatex(&path_find_context);
    latex_str += std::string("\n") + "\n";
}

void Test_Area2D_BaseHex(std::string& latex_str)
{
    latex_str += R"GG(\section{Area 2D: Base Hex})GG";
    latex_str += "\n";
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
    latex_str += area.ToStrLatex();
    latex_str += std::string("\n") + "\n";
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv)
{
    std::string desc;
    desc += "  -h,--help            print usage information and exit\n";
    desc += "  -latex-file FILE     file for LaTeX output\n";
    int arg_i = 1;
    std::string latex_file;
    while (arg_i < argc)
    {
        auto* arg = argv[arg_i];
        printf("%d: %s\n", arg_i, arg);
        if ((std::strcmp(arg, "--help") == 0) or (std::strcmp(arg, "-h") == 0))
        {
            printf("%s/n", desc.c_str());
            return 0;
        }
        if (std::strcmp(arg, "-latex-file") == 0)
        {
            ++arg_i;
            if (arg_i >= argc)
            {
                printf("Incomplete argument '-max-x': exit\n");
                return 1;
            }
            arg        = argv[arg_i];
            latex_file = arg;
        }
        ++arg_i;
    }

    std::string latex_str;
    latex_str += R"GG(\documentclass[12pt, letterpaper]{article})GG";
    latex_str += "\n";
    latex_str += R"GG(\usepackage[pdf]{graphviz})GG";
    latex_str += "\n";
    latex_str += R"GG(\usepackage[utf8]{inputenc})GG";
    latex_str += "\n";
    latex_str += R"GG(\usepackage{tikz})GG";
    latex_str += "\n";
    latex_str += R"GG(\usetikzlibrary{shapes})GG";
    latex_str += "\n";
    latex_str += R"GG(\begin{document})GG";
    latex_str += "\n";
    Test_GraphInclusive_Base(latex_str);
    Test_GraphInclusive_DOT(latex_str);
    Test_GraphInclusive_BasePathFind(latex_str);
    Test_Area2D_BaseMoore(latex_str);
    Test_Area2D_BaseVonNeumann(latex_str);
    Test_Area2D_BaseHex(latex_str);
    Test_GraphInclusive_ConnectionComponent(latex_str);
    latex_str += R"GG(\end{document})GG";
    latex_str += "\n";
    if (latex_str.empty())
    {
        printf("\n\n%s\n", latex_str.c_str());
    }
    else
    {
        std::ofstream ofs(latex_file, std::ofstream::out);
        ofs << latex_str;
        ofs.close();
    }
    return 0;
}
// NOLINTEND
