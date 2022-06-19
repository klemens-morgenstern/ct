// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/ct/source_location.hpp>
#include <cassert>

template<boost::ct::located_string Str, typename ... Args>
auto log(Args && ... args)
{
    constexpr auto fn = boost::ct::file_name<Str.location>;

    constexpr auto fmt = "%s(%d): " + Str + "\n";

    std::printf(fmt.c_str(), fn.c_str(), Str.location.line(), std::forward<Args>(args)...);
}



int main(int argc, char * argv[])
{
    //overload based on a string
    log<"Hello world!">();

    log<"Next log %d">(42);

    return 0;
}