// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/ct/source_location.hpp>
#include <array>

using namespace boost;

template<ct::source_location Loc>
struct loc
{
    constexpr static auto get() {return Loc; }
};

static_assert(loc<ct::source_location::current()>::get().line() == 17);
static_assert(loc<ct::source_location::current()>::get().line() == 18);

static_assert(ct::file_name<loc<ct::source_location::current()>::get()>.view().ends_with("source_location.cpp"));


template<ct::located_string Loc>
struct ls
{
    constexpr static auto get() {return Loc; }
};

static_assert(ls<"test-thingy">::get() == "test-thingy");
static_assert(ct::file_name<ls<"test-thingy">::get().location>.view().ends_with("source_location.cpp"));


template<ct::located_array Loc>
struct as
{
    constexpr static auto get() {return Loc; }
};

static_assert(as<std::array{1,2,3}>::get().at(1) == 2);
static_assert(as<std::array{1,2,3}>::get().location.line() == 40);
