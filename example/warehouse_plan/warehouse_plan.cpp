// Copyright 2024 oldnick85

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>

#include <area.h>
#include <graph_inclusive.h>
#include <primitives.h>

using Coord_t = GG::Coord2D;
using Node_t  = GG::Node<Coord_t>;

template <typename TNeighborhood>
uint CalcInTouch(const GG::Area2D<Node_t, TNeighborhood>& area, const Coord_t& max_coord, const Coord_t& start_coord)
{
    GG::PathFindContext path_find_context{&(area.Graph()), area.Graph().Find(start_coord)};
    path_find_context.SpreadWave();
    auto nodes = path_find_context.WaveNodes();
    std::unordered_set<Coord_t> in_touch;
    for (auto* const node : nodes)
    {
        auto coord            = node->Id();
        auto neighbour_coords = TNeighborhood::NeighbourCoordinates(coord, GG::Range2D(max_coord));
        for (auto neighbour_coord : neighbour_coords)
        {
            if (not area.Passable(neighbour_coord))
                in_touch.insert(neighbour_coord);
        }
    }
    return in_touch.size();
}

Coord_t NextCoord(const Coord_t& coord, const Coord_t& max_coord)
{
    Coord_t next_coord = coord;
    if (next_coord.X() >= max_coord.X())
    {
        next_coord = Coord_t(0, next_coord.Y() + 1);
    }
    else
    {
        next_coord = Coord_t(next_coord.X() + 1, next_coord.Y());
    }
    return next_coord;
}

template <typename TNeighborhood>
bool Next(GG::Area2D<Node_t, TNeighborhood>& area, const Coord_t& coord, const Coord_t& max_coord,
          const Coord_t& start_coord, uint& impassables)
{
    if (coord == start_coord)
    {
        Coord_t next_coord = NextCoord(coord, max_coord);
        return Next(area, next_coord, max_coord, start_coord, impassables);
    }

    if (area.Passable(coord))
    {
        area.SetPassable(coord, false);
        impassables++;
    }
    else
    {
        Coord_t next_coord = NextCoord(coord, max_coord);
        if ((coord == max_coord) or ((start_coord == max_coord) and (next_coord == start_coord)))
            return false;

        area.SetPassable(coord, true);
        impassables--;

        return Next(area, next_coord, max_coord, start_coord, impassables);
    }
    return true;
}

template <typename TNeighborhood>
void Search(const Coord_t& max_coord, const Coord_t& start_coord)
{
    GG::Range2D range(max_coord);
    GG::Area2D<Node_t, TNeighborhood> area(range);
    area.SetPassableAll(true);
    uint max_in_touch     = 0;
    uint cycle_count      = 0;
    const auto time_start = std::chrono::steady_clock::now();
    uint impassables      = 0;
    while (true)
    {
        cycle_count++;
        Coord_t coord(0, 0);
        if (not Next<TNeighborhood>(area, coord, max_coord, start_coord, impassables))
            break;
        if (impassables < max_in_touch)
            continue;
        const auto in_touch = CalcInTouch<TNeighborhood>(area, max_coord, start_coord);
        if (in_touch >= max_in_touch)
        {
            max_in_touch        = in_touch;
            const auto time_end = std::chrono::steady_clock::now();
            const uint time_ms  = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
            constexpr uint MsInSec             = 1000;
            const uint speed_cycles_per_second = (cycle_count * MsInSec) / (time_ms == 0 ? 1 : time_ms);
            printf("Found new best position: in_touch=%i; cycles=%d c; time=%d s;speed=%d cps;\n", in_touch,
                   cycle_count, time_ms / MsInSec, speed_cycles_per_second);
            printf("%s\n", area.ToStrASCII().c_str());
        }
    }
    const auto time_end    = std::chrono::steady_clock::now();
    const uint time_ms     = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    constexpr uint MsInSec = 1000;
    const uint speed_cycles_per_second = (cycle_count * MsInSec) / time_ms;
    printf("\nTOTAL: cycles=%d c; time=%d s;speed=%d cps;\n", cycle_count, time_ms / MsInSec, speed_cycles_per_second);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv)
{
    std::string desc;
    desc += "  -h,--help     print usage information and exit\n";
    desc += "  -max-x X      max X coordinate\n";
    desc += "  -max-y Y      max Y coordinate\n";
    desc += "  -start-x X    start X coordinate (0 by default)\n";
    desc += "  -start-y Y    start Y coordinate (0 by default)\n";
    desc += "  -mesh MESH    area mesh type: n - Von Neumann (by default), m - Moore, h - hex\n";
    int max_x   = 3;
    int max_y   = 3;
    int start_x = 0;
    int start_y = 0;
    int arg_i   = 1;
    char mesh   = 'n';
    while (arg_i < argc)
    {
        auto* arg = argv[arg_i];
        printf("%d: %s\n", arg_i, arg);
        if ((std::strcmp(arg, "--help") == 0) or (std::strcmp(arg, "-h") == 0))
        {
            printf("%s/n", desc.c_str());
            return 0;
        }
        if (std::strcmp(arg, "-max-x") == 0)
        {
            ++arg_i;
            if (arg_i >= argc)
            {
                printf("Incomplete argument '-max-x': exit\n");
                return 1;
            }
            arg   = argv[arg_i];
            max_x = std::stoi(arg);
        }
        else if (std::strcmp(arg, "-max-y") == 0)
        {
            ++arg_i;
            if (arg_i >= argc)
            {
                printf("Incomplete argument '-max-y': exit\n");
                return 1;
            }
            arg   = argv[arg_i];
            max_y = std::stoi(arg);
        }
        else if (std::strcmp(arg, "-start-x") == 0)
        {
            ++arg_i;
            if (arg_i >= argc)
            {
                printf("Incomplete argument '-start-x': exit\n");
                return 1;
            }
            arg     = argv[arg_i];
            start_x = std::stoi(arg);
        }
        else if (std::strcmp(arg, "-start-y") == 0)
        {
            ++arg_i;
            if (arg_i >= argc)
            {
                printf("Incomplete argument '-start-y': exit\n");
                return 1;
            }
            arg     = argv[arg_i];
            start_y = std::stoi(arg);
        }
        else if (std::strcmp(arg, "-mesh") == 0)
        {
            ++arg_i;
            if (arg_i >= argc)
            {
                printf("Incomplete argument '-mesh': exit\n");
                return 1;
            }
            arg  = argv[arg_i];
            mesh = arg[0];
        }
        ++arg_i;
    }

    if ((max_x <= 0) or (max_y <= 0))
    {
        printf("Incorrect max coordinates: exit\n");
        return 1;
    }

    switch (mesh)
    {
        case 'm':
            Search<GG::NeighborhoodMoore>(Coord_t(max_x, max_y), Coord_t(start_x, start_y));
            break;
        case 'h':
            Search<GG::NeighborhoodHex>(Coord_t(max_x, max_y), Coord_t(start_x, start_y));
            break;
        default:
            Search<GG::NeighborhoodVonNeumann>(Coord_t(max_x, max_y), Coord_t(start_x, start_y));
            break;
    }
    return 0;
}