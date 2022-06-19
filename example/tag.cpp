// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/ct/string.hpp>
#include <cassert>

template<boost::ct::string Id>
int tagged_value = 0;


int main(int argc, char * argv[])
{
    //overload based on a string
    tagged_value<"foo">++;
    tagged_value<"foo">++;
    tagged_value<"bar"> = 42;

    assert(tagged_value<"foo"> == 2);
    assert(tagged_value<"bar"> == 42);
    assert(tagged_value<"foobar"> == 0);

    return 0;
}