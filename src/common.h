/*
 * Copyright (c) 2019, F.S.. All rights reserved.
 */

#pragma once
#include <cassert>
#include <iostream>

///////////////////////
// Sun useful macros //
///////////////////////

#define FS_SUN_SINGLETON(x)                     \
    public:					\
    static inline x& Instance()			\
    {						\
	static x _instance;			\
	return _instance;			\
    }						\
private:					\
inline x(){};					\
inline x(x const&)   = delete;			\
inline void operator = (x const&){}

#define FS_SUN_SINGLETON_NO_CTORDEF(x)          \
    public:					\
    static inline x& Instance()			\
    {						\
	static x _instance;			\
	return _instance;			\
    }						\
private:					\
x();						\
inline x(x const&)   = delete;			\
inline void operator = (x const&){}

#define FS_SUN_SINGLETON_NO_CTORDCLR(x)         \
    public:					\
    static inline x& Instance()			\
    {						\
	static x _instance;			\
	return _instance;			\
    }						\
private:					\
inline x(x const&)   = delete;			\
inline void operator = (x const&){}

#define FS_SUN_FRIEND_BINARY_OPERATOR_DECLARE(return_t, operator_, operand_1_t, operand_2_t) \
    friend return_t operator operator_ (operand_1_t, operand_2_t);      \
    friend return_t operator operator_ (operand_2_t, operand_1_t);      \

//iff it's commutative
#define FS_SUN_FRIEND_BINARY_OPERATOR_DECLARE_SYMMETRICALLY(return_t, operator_, operand_1_t, operand_2_t) \
    friend return_t operator operator_ (operand_1_t, operand_2_t);      \
    inline friend return_t operator operator_ (operand_2_t o2, operand_1_t o1) \
    {                                                                   \
	return o1 operator_  o2;                                        \
    }

#define FS_SUN_SAFE_DELETE_ARRAY(x)             \
    delete [] x;				\
    x = nullptr;				\

#define FS_SUN_SAFE_DELETE(x)                   \
    delete x;                                   \
    x = nullptr;				\

#define FS_SUN_EXPAND(...) __VA_ARGS__
#define FS_SUN_MERGE(a,    b) a##b
#define _FS_SUN_MACRO_CALL_(func,  param) func param

// using variadic macro for the type contains comma, such as std::map<int, int>
#define FS_SUN_PROPERTY_R(acc_spec, name, ...)                          \
    acc_spec:    __VA_ARGS__ _##name;                                   \
public:                                                                 \
inline __VA_ARGS__ const & Get##name()const&                            \
{                                                                       \
    return _##name;                                                     \
}                                                                       \
inline __VA_ARGS__& Get##name()&                                        \
{                                                                       \
    return _##name;                                                     \
}                                                                       \
inline __VA_ARGS__&& Get##name()&&                                      \
{                                                                       \
    return std::move(_##name);                                          \
}                                                                       \
private:			// back to default access specifier

#define FS_SUN_PROPERTY_W(acc_spec, name, ...)	\
    acc_spec: __VA_ARGS__ _##name;		\
public:						\
inline void Set##name(__VA_ARGS__ const & val)	\
{						\
    _##name = val;				\
}						\
inline void Set##name(__VA_ARGS__&& val)	\
{						\
    _##name = std::move(val);			\
}						\
private:


#define FS_SUN_PROPERTY(acc_spec, name, ...)	\
    acc_spec: __VA_ARGS__ _##name;		\
public:						\
inline void Set##name(__VA_ARGS__ const & val)	\
{						\
    _##name = val;				\
}						\
inline void Set##name(__VA_ARGS__&& val)	\
{						\
    _##name = std::move(val);			\
}						\
inline __VA_ARGS__ const & Get##name()const&	\
{						\
    return _##name;				\
}						\
inline __VA_ARGS__& Get##name()&		\
{						\
    return _##name;				\
}						\
inline __VA_ARGS__&& Get##name()&&		\
{						\
    return std::move(_##name);			\
}						\
private:					

#define _FS_SUN_EXCLUDE_FIRST_ARG_(first, ...) __VA_ARGS__
#ifdef _MSC_VER
//MS preprocessor issue. see https://stackoverflow.com/questions/48088834/how-to-implement-exclude-first-argument-macro-in-msvc
#define FS_SUN_EXCLUDE_FIRST_ARG(...)     _FS_SUN_MACRO_CALL_(_FS_SUN_EXCLUDE_FIRST_ARG_, (__VA_ARGS__))
#else
#define FS_SUN_EXCLUDE_FIRST_ARG(...)     _FS_SUN_EXCLUDE_FIRST_ARG_(__VA_ARGS__)
#endif


#define _FS_SUN_GET32TH_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,	\
                              _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                              _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
                              _31, n, ...) n

#ifdef _MSC_VER
#define FS_SUN_GET32TH_ARGS(...) _FS_SUN_MACRO_CALL_(_FS_SUN_GET32TH_ARGS_, (__VA_ARGS__))
#else
#define FS_SUN_GET32TH_ARGS(...) _FS_SUN_GET32TH_ARGS_(__VA_ARGS__)
#endif


#define FS_SUN_GET32TH_ARGS_FROM_2ND(...) FS_SUN_GET32TH_ARGS(FS_SUN_EXCLUDE_FIRST_ARG(__VA_ARGS__))

// trailing comma suppressing
//https://msdn.microsoft.com/en-us/library/ms177415.aspx
//https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#ifdef _MSC_VER
#define FS_SUN_COMMA__VA_ARGS__(...) , __VA_ARGS__
#elif defined(__GNUC__) || defined(__clang)
#define FS_SUN_COMMA__VA_ARGS__(...) , ##__VA_ARGS__
#endif

#define FS_SUN_SMART_GET32TH_FROM_COMMA__VA_ARGS__OTHER(other, ...) FS_SUN_GET32TH_ARGS_FROM_2ND \
    (FS_SUN_COMMA__VA_ARGS__(__VA_ARGS__), other)

/**********************************/
// FS_SUN__VA_ARGS__
#define _FS_SUN__VA_ARGS__PARAM_EMPTY_  
#define _FS_SUN__VA_ARGS__PARAM_(param, empty)	param, param,	\
	param, param, param, param, param,			\
	param, param, param, param, param,			\
	param, param, param, param, param,			\
	param, param, param, param, param,			\
	param, param, param, param, param,			\
	param, param, param, param, empty			\

// expand to param  while count of __VA_ARGS__ greater than zero
#define FS_SUN__VA_ARGS__(param, ...) FS_SUN_SMART_GET32TH_FROM_COMMA__VA_ARGS__OTHER \
    (_FS_SUN__VA_ARGS__PARAM_(param, _FS_SUN__VA_ARGS__PARAM_EMPTY_), __VA_ARGS__)

// exception for comma(,)
#define _FS_SUN__VA_ARGS__COMMA_(...) FS_SUN_SMART_GET32TH_FROM_COMMA__VA_ARGS__OTHER \
    (_FS_SUN__VA_ARGS__PARAM_(_FS_SUN_COMMA_, _FS_SUN_COMMA_EMPTY_), __VA_ARGS__)

#define _FS_SUN_COMMA_REAL_ ,
#define _FS_SUN_COMMA_EMPTY_REAL_

#define _FS_SUN__VA_ARGS__COMMA_MERGE_(a) FS_SUN_MERGE(a, REAL_)

#ifdef _MSC_VER
#define _FS_SUN__VA_ARGS__COMMA_MERGE_MSC_WRAPPER_(a) _FS_SUN__VA_ARGS__COMMA_MERGE_(a)
#define FS_SUN__VA_ARGS__COMMA(...)                   _FS_SUN__VA_ARGS__COMMA_MERGE_MSC_WRAPPER_ \
    (_FS_SUN__VA_ARGS__COMMA_(__VA_ARGS__)) 
#else
#define FS_SUN__VA_ARGS__COMMA(...)                   _FS_SUN__VA_ARGS__COMMA_MERGE_ \
    (_FS_SUN__VA_ARGS__COMMA_(__VA_ARGS__)) 
#endif
/**********************************/

/**********************************/
// FS_SUN_ARGC, support 0-31 args

#define _FS_SUN_ARGC_GET32TH_PRESET_ARGS_ 31, 30, 	\
	29, 28, 27, 26, 25, 24, 23, 22, 21, 20,         \
	19, 18, 17, 16, 15, 14, 13, 12, 11, 10,         \
	9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FS_SUN_ARGC(...) FS_SUN_SMART_GET32TH_FROM_COMMA__VA_ARGS__OTHER \
    (_FS_SUN_ARGC_GET32TH_PRESET_ARGS_, __VA_ARGS__)

static_assert( FS_SUN_ARGC()     == 0, "FS_SUN_ARGC error 0");
static_assert( FS_SUN_ARGC(a)    == 1, "FS_SUN_ARGC error 1");
static_assert( FS_SUN_ARGC(a, a) == 2, "FS_SUN_ARGC error 2");
static_assert(FS_SUN_ARGC(a, a, a, a, a, a, a, a, a, a,
                          a, a, a, a, a, a, a, a, a, a,
                          a, a, a, a, a, a, a, a, a, a,
                          a)     == 31, "FS_SUN_ARGC error 31");

/**********************************/
#ifdef NDEBUG
#define FS_SUN_ASSERT(condition, ...) 
#else

#define FS_SUN_ASSERT(condition, ...)					\
    static_assert(FS_SUN_ARGC(__VA_ARGS__) == 0 || FS_SUN_ARGC(__VA_ARGS__) == 1, \
		  "FS_SUN_ASSERT can take only zero or one MSG arg");	\
    if(!(condition))							\
    {									\
	std::cerr << "**************** FS_SUN_ASSERT FAILED ****************" \
		  << std::endl;						\
	std::cerr << "CONDITION@ " << #condition << std::endl;		\
	std::cerr << "LINE@ " << __LINE__ << std::endl			\
		  << "FILE@ " << __FILE__ << std::endl;			\
	std::cerr << "MSG@ " FS_SUN__VA_ARGS__(<< ,__VA_ARGS__)  __VA_ARGS__ \
		  << std::endl;						\
	std::cerr << "**************** FS_SUN_ASSERT FAILED ****************" \
		  << std::endl;						\
	assert(false);							\
    }
#endif

///////////////////////////////
// call a function verbosely //
///////////////////////////////
#ifdef FS_SUN_VERBOSE
#define FS_SUN_CALL_V(function, ...)                                    \
    {                                                                   \
    fs::Sun::string msg("calling @function: " #function);               \
    if(FS_SUN_ARGC(__VA_ARGS__) > 0)                                    \
    {                                                                   \
        msg += "@param";                                                \
        msg = msg.concat_with_delimiter(" : " FS_SUN_COMMA__VA_ARGS__(__VA_ARGS__)); \
    }                                                                   \
    fs::Sun::logger::Instance().log(msg);                               \
    }                                                                   \
        function(__VA_ARGS__);
#else
#define FS_SUN_CALL_V(function, ...)            \
    function(__VA_ARGS__);
#endif


////////////////////////////////
// type tarits
////////////////////////////////
#include <type_traits>
namespace fs
{
    namespace Sun
    {
        template <typename T>
        struct rm_cv_ref { using                                  type = T; };
        // specialization
#define _FS_SUN_RM_CV_REF_SPEC_(cv_ref_t)                               \
        template <typename T> struct rm_cv_ref< cv_ref_t> { using type = T; };

        // const | non-const, volatile | non-volatile, l-ref(&) | r-ref(&&) | non-ref. 2*2*3 = 12
        _FS_SUN_RM_CV_REF_SPEC_(const volatile T &);
        _FS_SUN_RM_CV_REF_SPEC_(const volatile T &&);
        _FS_SUN_RM_CV_REF_SPEC_(const volatile T);
        _FS_SUN_RM_CV_REF_SPEC_(const T &);
        _FS_SUN_RM_CV_REF_SPEC_(const T &&);
        _FS_SUN_RM_CV_REF_SPEC_(const          T);
        _FS_SUN_RM_CV_REF_SPEC_(volatile T &);
        _FS_SUN_RM_CV_REF_SPEC_(volatile T &&);
        _FS_SUN_RM_CV_REF_SPEC_(volatile       T);
        _FS_SUN_RM_CV_REF_SPEC_(T &);
        _FS_SUN_RM_CV_REF_SPEC_(T &&);
        //    _FS_SUN_RM_RV_REF_SPEC_();
    
        template <typename T>
        struct is_std_string: std::false_type {};
        template <> struct is_std_string <std::string> : std::true_type {};

    }
}


////////////////////////////////
// misc
////////////////////////////////
namespace fs
{
    namespace Sun
    {
	template<typename To, typename From>
	inline To* cast(From* from)
	{
#ifdef NDEBUG
	    return static_cast<To*>(from);
#else
	    To* to = dynamic_cast<To*>(from);
	    assert(to != nullptr);
	    return to;
#endif    
	}

	template<typename To, typename From>
	inline To& cast(From && from)
	{
#ifdef NDEBUG
	    return static_cast<To&&>(from);
#else
	    return dynamic_cast<To&&>(from);
#endif    
	}

	template <typename T>
	struct less_ptr
	{
	    bool operator()(const T* l, const T* r) const
            {
                return (*l) < (*r);
            }
	};
    }
}
