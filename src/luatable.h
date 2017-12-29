#pragma once
#include "ns.h"
#include "string.h"
#include "luastate.h"
#include <vector>
#include <functional>

#define _GB_UTILS_LUATABLE_MAPPER_GETTER_DECL(ret_type, name)	\
    ret_type get_##name##_by_key(const char* key) const;	\
    ret_type get_##name##_by_idx(const size_t idx) const;	\
    std::vector<ret_type> get_##name##s() const;		

GB_UTILS_NS_BEGIN

class luatable_mapper;

struct luatable
{
    inline virtual ~luatable() {}
    virtual void from_lua(const luatable_mapper& mapper) = 0;
    inline virtual void to_lua(const
#if defined(__GNUC__) || defined(__clang__)
			       __attribute__((unused))
#endif
			       luatable_mapper& mapper) const {}

    
};

class luatable_mapper
{
public:
    luatable_mapper(const char* file, luastate& config_luastate);
public:
    bool validate()const;
    size_t objlen()const;
    _GB_UTILS_LUATABLE_MAPPER_GETTER_DECL(lua_Number, number);
    _GB_UTILS_LUATABLE_MAPPER_GETTER_DECL(lua_Integer, integer);
    _GB_UTILS_LUATABLE_MAPPER_GETTER_DECL(gb::utils::string, string);
    
    void get_table_by_key(const char* key, luatable& table) const;
    void get_table_by_idx(const size_t idx, luatable& table) const;

    void for_each(std::function<void(const size_t)> func) const;

    // scene::instantiate(const char* luafile)
    // 	{
    // 	    luatable_mapper lt(luafile);
    // 	    lt.get_cpps([](lua_State* l)
    // 			{
    // 			    Entity e;
    // 			    e.instantiate(l);//may also instantiate(luafile)//prefab instantiate
    // 			    _mpEntitys.insert(e.name, e);
    // 			});
    // 	}
    // void get_cpps(func)
    // 	{
    // 	    size_t len = lua_objlen(_l, -1);
    // 	    for(size_t i = 1; i <= len; i++)
    // 	    {
    // 		lua_rawgeti(_l, -1, i);
    // 		func(_l);

    // 		lua_pop(_l, 1);
    // 	    }
    // 	}
private:
    lua_State* _l;
};

GB_UTILS_NS_END