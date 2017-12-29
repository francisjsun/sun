#include "luatable.h"
#include "logger.h"

using namespace gb::utils;

luatable_mapper::luatable_mapper(const char* file, luastate& config_luastate):
    _l(config_luastate.getstate())
{
    assert(_l != nullptr);
    
    if(!config_luastate.dofile(file))
	logger::Instance().error(string("luatable::luatable config_luastate->dofile error@ ") + file);
}

bool luatable_mapper::validate()const
{
    return lua_type(_l, -1) == LUA_TTABLE;
}

size_t luatable_mapper::objlen()const
{
    return lua_objlen(_l, -1);
}

#define _GB_UTILS_LUATABLE_MAPPER_GETTER_DEF(ret_type, name, type, lua_to_func, default_value) \
    ret_type luatable_mapper::get_##name##_by_key(const char* key) const \
    {									\
	assert(key != nullptr);						\
	lua_getfield(_l, -1, key);					\
	if(lua_type(_l, -1) == type)					\
	{								\
	    ret_type ret(lua_to_func(_l, -1));				\
	    lua_pop(_l, 1);						\
	    return ret;							\
	}								\
	else								\
	{								\
	    logger::Instance().warning(string("luatable_mapper::get_" #name "_by_key unmatched type@") + key); \
	    lua_pop(_l, 1);						\
	    return default_value;					\
	}								\
    }									\
    std::vector<ret_type> luatable_mapper::get_##name##s() const	\
    {									\
	std::vector<ret_type> ret;					\
	const size_t len = lua_objlen(_l, -1);				\
	for(size_t i = 1; i <= len; i++)				\
	{								\
	    ret.push_back(get_##name##_by_idx(i));			\
	}								\
	return ret;							\
    }									\
    ret_type luatable_mapper::get_##name##_by_idx(const size_t idx) const \
    {									\
	assert(idx >= 1);						\
	lua_rawgeti(_l, -1, idx);					\
	if(lua_type(_l, -1) == type)					\
	{								\
	    ret_type ret(lua_to_func(_l, -1));				\
	    lua_pop(_l, 1);						\
	    return ret;							\
	}								\
	else								\
	{								\
	    logger::Instance().warning(string("luatable_mapper::get_" #name "_by_idx unmatched type@") + idx); \
	    lua_pop(_l, 1);						\
	    return default_value;					\
	}								\
    }

_GB_UTILS_LUATABLE_MAPPER_GETTER_DEF(lua_Number, number, LUA_TNUMBER, lua_tonumber, 0);
_GB_UTILS_LUATABLE_MAPPER_GETTER_DEF(lua_Integer, integer, LUA_TNUMBER, lua_tointeger, 0);
_GB_UTILS_LUATABLE_MAPPER_GETTER_DEF(string, string, LUA_TSTRING, lua_tostring, string());

void luatable_mapper::get_table_by_key(const char* key, luatable& table) const
{
    assert(key != nullptr);
    lua_getfield(_l, -1, key);
    if(lua_type(_l, -1) == LUA_TTABLE)
    {
	table.from_lua(*this);
	lua_pop(_l, 1);
    }
    else
    {
	lua_pop(_l, 1);
    }
}
    
void luatable_mapper::get_table_by_idx(const size_t idx, luatable& table) const
{
    assert(idx >= 1);
    lua_rawgeti(_l, -1, idx);
    if(lua_type(_l, -1) == LUA_TTABLE)
    {
	table.from_lua(*this);
	lua_pop(_l, 1);
    }
    else
    {
	lua_pop(_l, 1);
    }
}

void luatable_mapper::for_each(std::function<void(const size_t idx)> func) const
{
    const size_t len = lua_objlen(_l, -1);
    for(size_t i = 1; i <= len; i++)
    {
	func(i);
    }
}