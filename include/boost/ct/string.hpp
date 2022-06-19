// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_CT_STRING_HPP
#define BOOST_CT_STRING_HPP

#include <algorithm>
#include <limits>
#include <boost/utility/string_view.hpp>

namespace boost::ct
{

template<typename Char>
constexpr std::size_t strlen(Char *c ) {return std::char_traits<Char>::length(c);}

template< std::size_t Size,
          typename Char = char>
struct string
{
    using value_type = Char;
    using size_type = std::size_t;
    constexpr static size_type npos = std::numeric_limits<size_type>::max();
    using difference_type = std::ptrdiff_t;

    using       reference = value_type&;
    using const_reference = const value_type&;
    using pointer         = Char *;

    using const_pointer = const Char * ;

    using iterator = Char;
    using const_iterator = const Char * ;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using char_type = Char;

    constexpr string() { raw_string[0] = value_type('\0'); }
    constexpr string(const Char (&raw)[Size]) { std::copy(std::begin(raw), std::end(raw), std::begin(raw_string)); }

    constexpr string(basic_string_view<Char> sv)
    {
        auto len = (std::min<std::size_t>)(sv.size(), Size - 1);
        std::copy_n(sv.data(), len , std::begin(raw_string));
        raw_string[len] = value_type('\0');
    }

    constexpr value_type& at(std::size_t idx)
    {
        if (idx >= size())
            throw std::out_of_range(("ct_string<\"" + *this + "\">").raw_string);
        return raw_string[idx];
    }

    constexpr const value_type& at(std::size_t idx) const
    {
        if (idx >= size())
            throw std::out_of_range(("ct_string<\"" + *this + "\">").raw_string);
        return raw_string[idx];
    }
    constexpr       value_type& operator[](std::size_t idx)       {return raw_string[idx];}
    constexpr const value_type& operator[](std::size_t idx) const {return raw_string[idx];}

    constexpr       value_type& front()       {return raw_string[0u];}
    constexpr const value_type& front() const {return raw_string[0u];}

    constexpr      value_type& back()       {return raw_string[size()-1u];}
    constexpr const value_type& back() const {return raw_string[size()-1u];}

    constexpr       value_type* data()       {return raw_string;}
    constexpr const value_type* data() const {return raw_string;}

    constexpr const value_type* c_str() const {return raw_string;}

    constexpr pointer begin() {return raw_string;}
    constexpr pointer   end() {return raw_string + size();}

    constexpr const_pointer begin() const {return raw_string;}
    constexpr const_pointer   end() const {return raw_string + size();}

    constexpr const_pointer cbegin() const {return raw_string;}
    constexpr const_pointer   cend() const {return raw_string + size();}

    constexpr reverse_iterator rbegin() {return raw_string + size();}
    constexpr reverse_iterator   rend() {return raw_string;}

    constexpr const_reverse_iterator rbegin() const {return raw_string + size();}
    constexpr const_reverse_iterator   rend() const {return raw_string;}

    constexpr const_reverse_iterator rcbegin() const {return raw_string + size();}
    constexpr const_reverse_iterator   rcend() const {return raw_string;}

    constexpr static std::size_t empty()    {return Size == 1; }
    constexpr static std::size_t size()     {return Size - 1; }
    constexpr static std::size_t length()   {return Size - 1; }
    constexpr static std::size_t max_size() {return Size - 1; }
    constexpr static std::size_t capacity() {return Size - 1; }


    template<typename Traits = std::char_traits<char_type>>
    constexpr basic_string_view< char_type, Traits >
    view() const
    {
        return { raw_string };
    }
    constexpr operator basic_string_view< char_type >() const { return view(); }
    constexpr explicit operator const char*() const {return raw_string;}

    template<size_type pos = 0, size_type count = npos>
    constexpr auto substr() const -> string<count == npos ? Size - pos : count + 1, char_type >
    {
        constexpr auto sz = count == npos ? size() - pos : count;
        value_type tmp[sz + 1];
        tmp[sz] = value_type('\0');
        std::copy(raw_string + pos, raw_string + pos + sz, tmp);
        return tmp;
    }

    constexpr auto
    operator==(basic_string_view< char_type > sv) const
    {
        return view() == sv;
    }

    constexpr auto
    operator<=>(basic_string_view< char_type > sv) const
    {
        return view() <=> sv;
    }

    Char raw_string[Size] {};
};

template < typename Char, std::size_t AddSize, std::size_t Size >
constexpr auto
operator+(const Char (&raw)[AddSize], string< Size, Char> r) -> string< Size + AddSize - 1, Char >
{
    Char tmp[Size + AddSize - 1] {};
    std::copy(std::begin(raw), std::end(raw), std::begin(tmp));
    std::copy(std::begin(r.raw_string), std::end(r.raw_string), std::begin(tmp) + AddSize - 1);
    return string<Size + AddSize - 1, Char> (tmp);
}
template < typename Char, std::size_t AddSize, std::size_t Size >
constexpr auto
operator+(string< Size, Char > r, const Char (&raw)[AddSize]) -> string< Size + AddSize - 1, Char >
{
    Char tmp[Size + AddSize - 1] {};
    std::copy(std::begin(r.raw_string), std::end(r.raw_string), std::begin(tmp));
    std::copy(std::begin(raw), std::end(raw) - 1, std::begin(tmp) + Size - 1);
    return string<Size + AddSize - 1, Char> (tmp);
}

template < typename Char, std::size_t AddSize, std::size_t Size >
constexpr auto
operator+(const Char raw, string< Size, Char> r) -> string< Size + 1, Char >
{
    Char tmp[Size + 1] {raw};
    std::copy(std::begin(r.raw_string), std::end(r.raw_string), std::begin(tmp) + 1);
    return string<Size + 1, Char> (tmp);
}
template < typename Char, typename Traits, std::size_t AddSize, std::size_t Size >
constexpr auto
operator+(string< Size, Char > r, const Char raw) -> string< Size + 1, Char >
{
    Char tmp[Size + 1] {};
    std::copy(std::begin(r.raw_string), std::end(r.raw_string), std::begin(tmp));
    tmp[Size] = tmp[Size - 1];
    tmp[Size - 1] = raw;
    return string<Size + 1, Char> (tmp);
}

}

#endif //BOOST_CT_STRING_HPP
