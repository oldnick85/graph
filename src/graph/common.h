// Copyright 2024 oldnick85

#pragma once

#include <unordered_set>

#undef GRAPH_DEBUG_CHECKS
#define GRAPH_DEBUG_CHECKS

#ifdef GRAPH_DEBUG_CHECKS
#include <cassert>
#define GRAPH_DEBUG_ASSERT(exp, msg) assert(((void)msg, exp));
#else
#define GRAPH_DEBUG_ASSERT(exp, msg) \
    {}
#endif

namespace GG
{}  // namespace GG