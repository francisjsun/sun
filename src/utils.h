/*
 * Copyright (c) 2019, F.S.. All rights reserved.
 */

#pragma once

#include "ns.h"
#include <type_traits>
#include <tuple>
#include <future>

FS_SUN_NS_BEGIN

template<typename _0_t, typename _1_t>
_0_t & max(const _0_t & _0, const _1_t & _1)
{
    return _0 > _1? _0 : _1;
}

template<typename _0_t, typename _1_t, typename _2_t, typename ... other_t>
_0_t & max(const _0_t & _0, const _1_t & _1, const _2_t & _2, other_t & ... other)
{
    _0_t & _x = max(other...);
    return _0 > _x? _0 : _x;
}

template<typename T, T ... _n>
struct static_max;

template<typename T, T _0, T _1>
struct static_max<T, _0, _1>
{
    static constexpr T value = _0 > _1? _0 : _1;
};

template<typename T, T _0, T _1, T _2, T ... _n>
struct static_max<T, _0, _1, _2, _n ...>
{
    static constexpr T _x = static_max<T, _1, _2, _n ...>::value;
    static constexpr T value = _0 > _x? _0 : _x;
};

/** index sequence */
template <std::size_t ...>
struct index_sequence
{};
template <std::size_t n, std::size_t ... s>
struct make_index_sequence : make_index_sequence<n-1, n-1, s...>
{};

template <std::size_t ... s>
struct make_index_sequence<0, s...>
{
    using type = index_sequence<s...>;
};
template <typename ... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

template <typename func_t, typename tuple_t, std::size_t ... idx>
typename std::remove_reference<func_t>::type::result_type
_apply(func_t && f, tuple_t && t, index_sequence<idx...>)
{
    return std::forward<func_t>(f)(std::get<idx>(std::forward<tuple_t>(t))...);
}
template <typename func_t, typename tuple_t>
typename std::remove_reference<func_t>::type::result_type
apply(func_t && f, tuple_t && t)
{
    return _apply(std::forward<func_t>(f), std::forward<tuple_t>(t),
                  typename make_index_sequence<
                  std::tuple_size<tuple_t>::value>::type{});
}

template <typename func_t, typename tuple_t>
void apply2promise(func_t && f, tuple_t && t,
                   std::promise<typename
                   std::enable_if<std::is_void<
                   typename std::remove_reference<
                   func_t>::type::result_type>::value, typename std::remove_reference<
                   func_t>::type::result_type>::type> & promise)
{
    apply(std::forward<func_t>(f), std::forward<tuple_t>(t));
    promise.set_value();
}

template <typename func_t, typename tuple_t>
void apply2promise(func_t && f, tuple_t && t,
                   std::promise<typename
                   std::enable_if<! (std::is_void<typename std::remove_reference<
                                     func_t>::type::result_type>::value),
                   typename std::remove_reference<
                   func_t>::type::result_type>::type> & promise)
{
    promise.set_value(apply(std::forward<func_t>(f), std::forward<tuple_t>(t)));
}

/*****************/
/** index_of_seq */
/*****************/
template<typename T, typename ... SEQ>
struct _index_of_seq;

template<typename T, typename First, typename ... Other>
struct _index_of_seq<T, First, Other...>
{
    static constexpr std::uint8_t value = std::is_same<T, First>::value? sizeof...(Other) :
    _index_of_seq<T, Other...>::value;
};
template<typename T>
struct _index_of_seq<T>
{
    static constexpr std::uint8_t value = std::numeric_limits<std::uint8_t>::max();
};

/**
 *  \brief get index_of_seq of T in SEQ
 */
template<typename T, typename ... SEQ>
struct index_of_seq
{
    static constexpr std::uint8_t npos = std::numeric_limits<std::uint8_t>::max();
    static constexpr std::uint8_t _other_count = _index_of_seq<T, SEQ...>::value;
    static constexpr std::uint8_t value = _other_count != npos? sizeof...(SEQ) - _other_count - 1 : npos;
};

/****************/
/** type of seq */
/****************/
template<std::uint8_t index, std::uint8_t cur_index, typename ... SEQ>
struct _type_of_seq;

template<std::uint8_t index, std::uint8_t cur_index, typename _0_t, typename _1_t, typename ... _n_t>
struct _type_of_seq<index, cur_index, _0_t, _1_t, _n_t ...>
{
    using type = typename std::conditional<index == cur_index,
                                           _0_t,
                                           typename _type_of_seq<
                                               index, cur_index+1, _1_t, _n_t ...>::type>::type;
};

template<std::uint8_t index, std::uint8_t cur_index, typename last_t>
struct _type_of_seq<index, cur_index, last_t>
{
    using type = typename std::enable_if<index == cur_index, last_t>::type;
};

template<std::uint8_t index, typename ... SEQ>
struct type_of_seq
{
    using type = typename _type_of_seq<index, (std::uint8_t)0u, SEQ...>::type;
};

/***********************/
/** for_each for types */
/***********************/
template<template<typename> class func_t, typename last_t>
void _for_each(std::uint8_t idx)
{
    static constexpr func_t<last_t> func;
    func(idx);
}

template<template<typename> class func_t, typename _0_t, typename _1_t, typename ... _n_t>
void _for_each(std::uint8_t idx)
{
    static constexpr func_t<_0_t> func;
    func(idx);
    idx++;
    _for_each<func_t, _1_t, _n_t ...>(idx);
}

/**
 * \brief For each T in Ts do func_t<T>(std::uint8_t idx).
 */
template<template<typename> class func_t, typename _0_t, typename ... _n_t>
void for_each()
{
    _for_each<func_t, _0_t, _n_t ...>((std::uint8_t)0u);
}
/***************/
/** static_and */
/***************/
/** 
 * \brief value = (func_t<SET>::value &&)...
 */
template<template<typename> class func_t, typename ... SET>
struct static_and;
template<template<typename> class func_t, typename _0_t, typename _1_t, typename ... _n_t>
struct static_and<func_t, _0_t, _1_t, _n_t ...>
{
    static constexpr bool value = func_t<_0_t>::value && static_and<func_t, _1_t, _n_t ...>::value;
};
template<template<typename> class func_t, typename last_t>
struct static_and<func_t, last_t>
{
    static constexpr bool value = func_t<last_t>::value;
};

/**************/
/** is_one_of */
/**************/
template<typename ... T>
struct is_one_of;

template<typename T, typename _0_t, typename _1_t, typename ... _n_t>
struct is_one_of<T, _0_t, _1_t, _n_t ...>
{
    static constexpr bool value = std::is_same<T, _0_t>::value? true : is_one_of<T, _1_t, _n_t ...>::value;
};
template<typename T, typename last_t>
struct is_one_of<T, last_t>
{
    static constexpr bool value = std::is_same<T, last_t>::value;
};
FS_SUN_NS_END
