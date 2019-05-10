/*
 * Copyright (c) 2019, F.S.. All rights reserved.
 */

#pragma once

#include "ns.h"
#include <vector>
#include "debug.h"

FS_SUN_NS_BEGIN

template<typename T>
class flexible_vector
{
public:
    flexible_vector(const std::size_t size):
        _data(size),
        _beginItr(0),
        _endItr(size)
    {}
public:
    std::size_t size() const
    {
        return _endItr - _beginItr;
    }
    T * data()
    {
        return _data.data() + _beginItr;
    }
    const T * data() const
    {
        return const_cast<flexible_vector*>(this)->data();
    }
    void set_begin(const std::size_t new_begin)
    {
        FS_SUN_ASSERT(new_begin < _data.size());
        _beginItr = new_begin;
    }
    void begin_increase(const std::size_t begin_offset)
    {
        _beginItr += begin_offset;
        FS_SUN_ASSERT(_beginItr < _data.size());
    }

    void set_end(const std::size_t new_end)
    {
        FS_SUN_ASSERT(new_end < _data.size() && new_end > _beginItr);
        _endItr = new_end;
    }

private:
    std::vector<T> _data;
    std::size_t _beginItr, _endItr;
};

FS_SUN_NS_END
