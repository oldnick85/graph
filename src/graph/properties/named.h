// Copyright 2024 oldnick85

#pragma once

#include <array>
#include <string>

#include "../common.h"

namespace GG
{

template <bool IsNamed>
class Named
{};

template <>
class Named<true>
{
  public:
    Named() = default;
    Named(const std::string& name) : m_name(name) { GRAPH_DEBUG_ASSERT(not m_name.empty(), "Empty graph name"); }
    std::string GetName() const { return m_name; }

  private:
    std::string m_name;
};

template <>
class Named<false>
{
  public:
    Named() = default;
    Named(__attribute__((unused)) const std::string& name) {}
    std::string GetName() const
    {
        //std::array<char, 16> str;
        //snprintf(str.data(), str.size(), "%p", this);
        //return str.data();
        return std::to_string(reinterpret_cast<u_int64_t>(this));
    }
};

}  // namespace GG