// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_CT_SOURCE_LOCATION_HPP
#define BOOST_CT_SOURCE_LOCATION_HPP

#include <boost/ct/string.hpp>

#include <array>
#include <cstddef>
#include <cstdint>

namespace boost::ct
{

#if !defined(BOOST_CT_SOURCE_LOCATION_FILENAME_LENGTH)
#define BOOST_CT_SOURCE_LOCATION_FILENAME_LENGTH 4096
#endif

#if !defined(BOOST_CT_SOURCE_LOCATION_FUNCTION_LENGTH)
#define BOOST_CT_SOURCE_LOCATION_FUNCTION_LENGTH 256
#endif


struct source_location
{
    constexpr source_location() noexcept = default;
    constexpr source_location(const source_location &other) = default;
    source_location(source_location &&other) noexcept = default;

    constexpr source_location &
    operator=(const source_location &other)
    {
        line_   = other.line_;
        column_ = other.column_;

        std::copy(std::begin(other.file_name_), std::end(other.file_name_), std::begin(file_name_));
        std::copy(std::begin(other.function_name_), std::end(other.function_name_), std::begin(function_name_));
        return *this;
    }

    constexpr static source_location
    current(std::uint_least32_t line          = __builtin_LINE(),
#if __has_builtin(__builtin_COLUMN)
            std::uint_least32_t column        = __builtin_COLUMN(),
#endif
            const char         *file_name     = __builtin_FILE(),
            const char         *function_name = __builtin_FUNCTION())
    {
        source_location res;
        res.line_                          = line;
#if __has_builtin(__builtin_COLUMN)
        res.column_                        = column;
#endif
        const std::size_t file_len = std::min<std::size_t>(
                BOOST_CT_SOURCE_LOCATION_FILENAME_LENGTH - 1u,
                std::char_traits< char >::length(file_name));

        const std::size_t func_len = std::min<std::size_t>(
                BOOST_CT_SOURCE_LOCATION_FUNCTION_LENGTH - 1u,
                std::char_traits< char >::length(function_name));

        res.file_name_    [file_len] = '\0';
        res.function_name_[func_len] = '\0';
        std::copy_n(file_name,     file_len, res.file_name_);
        std::copy_n(function_name, func_len, res.function_name_);

        return res;
    }

    /// return the line number represented by this object
    constexpr std::uint_least32_t
    line() const noexcept
    {
        return line_;
    }
    /// return the column number represented by this object
    constexpr std::uint_least32_t
    column() const noexcept
    {
        return column_;
    }

    /// return the file name represented by this object
    constexpr const char *
    file_name() const noexcept
    {
        return file_name_;
    }
    /// return the name of the function represented by this object, if any
    constexpr const char *
    function_name() const noexcept
    {
        return function_name_;
    }

    std::uint_least32_t line_   = 0;
    std::uint_least32_t column_ = 0;

    char file_name_[BOOST_CT_SOURCE_LOCATION_FILENAME_LENGTH] = { };
    char function_name_[BOOST_CT_SOURCE_LOCATION_FUNCTION_LENGTH] = { };

    friend auto
    operator<=>(const source_location &lhs, const source_location &rhs) = default;
};

template<source_location loc>
constexpr auto file_name{string(loc.file_name_).template substr<0, ct::strlen(loc.file_name_) + 1>()};

template<source_location loc>
constexpr auto function_name{string(loc.file_name_).template substr<0, ct::strlen(loc.function_name_) + 1>()};

template < std::size_t Size, typename Char >
struct located_string : string<Size, char>
{
    constexpr located_string(const Char (&raw)[Size],
                             source_location location = source_location::current())
            : string<Size, char>(raw),  location(location)
    {
    }

    constexpr located_string(const string< Size, Char > &ts,
                             source_location                location = source_location::current())
            : string<Size, char>(ts), location(location)
    {
    }
    source_location location;
};


template < std::size_t Size, typename T>
struct located_array : std::array<T, Size>
{
    constexpr located_array(const T (&raw)[Size],
                             source_location location = source_location::current())
            : std::array<T, Size>(raw),  location(location)
    {
    }

    constexpr located_array(T  (&&raw)[Size],
                            source_location location = source_location::current())
            : std::array<T, Size>(std::move(raw)),  location(location)
    {
    }

    constexpr located_array(const std::array<T, Size> & raw,
                            source_location location = source_location::current())
            : std::array<T, Size>(raw),  location(location)
    {
    }

    constexpr located_array(std::array<T, Size> && raw,
                            source_location location = source_location::current())
            : std::array<T, Size>(std::move(raw)),  location(location)
    {
    }

    source_location location;
};

}


#endif //BOOST_CT_SOURCE_LOCATION_HPP
