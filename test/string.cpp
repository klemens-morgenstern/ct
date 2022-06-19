// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/ct/string.hpp>

using namespace boost;

template<ct::string Str>
struct harness
{
    constexpr static auto get() {return Str; }
};

static_assert(harness<"foobar">::get() == "foobar");
static_assert(harness<"foobar">::get().size() == 6);
static_assert(harness<"foobar">::get().at(0) == 'f');
static_assert(harness<"foobar">::get().at(1) == 'o');
static_assert(harness<"foobar">::get().at(2) == 'o');
static_assert(harness<"foobar">::get().at(3) == 'b');
static_assert(harness<"foobar">::get().at(4) == 'a');
static_assert(harness<"foobar">::get().at(5) == 'r');
static_assert(harness<"foobar">::get().substr<1>() == "oobar");
static_assert(harness<"foobar">::get().substr<1, 3>() == "oob");
static_assert(harness<ct::string("foo") + "bar">::get() == "foobar");
