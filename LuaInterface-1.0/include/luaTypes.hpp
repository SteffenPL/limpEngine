#ifndef LUA_TYPES_HPP
#define LUA_TYPES_HPP


#include <string>


using LuaFunction = int (*)(lua_State*);

/*
#define LUA_TNONE		(-1)
#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8
*/


enum class LuaType : int
{
    None = -1,
    Nil = 0,
    Boolean = 1,
    LightUserData = 2,
    Number = 3,
    CString = 4,
    Table = 5,
    CFunction = 6,
    UserData = 7,
    Empty = 8,
    Integer = 9
};

template< typename T1 >
struct PointerTraits
{
    using Type = T1;
};

template< typename T >
struct PointerTraits<T*>
{
    using Type = T;
};


template<typename T>
struct LuaTypeTraits
{

    static void push( lua_State* pLua , const T& value )
    {
        T* newData = (T*)lua_newuserdata( pLua , sizeof(T) );
        luaL_getmetatable( pLua , PointerTraits<T>::Type::Name );
        lua_setmetatable( pLua , -2 );

        *newData = value;
    }

    static T& check( lua_State* pLua , int index )
    {
        T* tmp = (T*)luaL_checkudata( pLua , index , PointerTraits<T>::Type::Name );
        std::string error = PointerTraits<T>::Type::Name;
        error = "Falscher Parameter. '" + error + "' erwartet.";
        luaL_argcheck( pLua , tmp != NULL , index , error.c_str() );
        return *tmp;
    }

    static T get( lua_State* pLua , int index )
    {
        return *(getPointer(pLua,index));
    }

    static T* getPointer( lua_State* pLua , int index )
    {
        return (T*)lua_topointer(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        if( luaL_checkudata( pLua , index , PointerTraits<T>::Type::Name ) != NULL )
            return true;
        return false;
    }

};


template<>
struct LuaTypeTraits<bool>
{
    static constexpr LuaType Type = LuaType::Boolean;

    static void push( lua_State* pLua , bool value )
    {
        lua_pushboolean(pLua , value );
    }

    static bool check( lua_State* pLua , int index )
    {
        if( lua_isboolean( pLua , index ) )
            return lua_toboolean( pLua , index );
        else
            return false;
    }

    static bool get( lua_State* pLua , int index )
    {
        return lua_toboolean(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return lua_isboolean( pLua , index );
    }
};


template<>
struct LuaTypeTraits<void*>
{
    static constexpr LuaType Type = LuaType::LightUserData;

    static void push( lua_State* pLua , void* value )
    {
        lua_pushlightuserdata( pLua , value );
    }

    static const void* check( lua_State* pLua , int index )
    {
        if( isType( pLua , index ) )
            return get( pLua , index );
        else
            return NULL;
    }

    static const void* get( lua_State* pLua , int index )
    {
        return lua_topointer(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return lua_islightuserdata( pLua , index );
    }
};

template<>
struct LuaTypeTraits<int>
{
    static constexpr LuaType Type = LuaType::Integer;

    static void push( lua_State* pLua , int value )
    {
        lua_pushinteger(pLua , value );
    }

    static int check( lua_State* pLua , int index )
    {
        return luaL_checkinteger( pLua , index );
    }

    static int get( lua_State* pLua , int index )
    {
        return lua_tointeger(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return true;
    }
};

template<>
struct LuaTypeTraits<float>
{
    static constexpr LuaType Type = LuaType::Number;

    static void push( lua_State* pLua , float value )
    {
        lua_pushnumber(pLua , (double)value );
    }

    static float check( lua_State* pLua , int index )
    {
        return luaL_checknumber( pLua , index );
    }

    static float get( lua_State* pLua , int index )
    {
        return lua_tonumber(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return lua_isnumber( pLua , index );
    }
};

template<>
struct LuaTypeTraits<double>
{
    static constexpr LuaType Type = LuaType::Number;

    static void push( lua_State* pLua , double value )
    {
        lua_pushnumber(pLua , value );
    }

    static double check( lua_State* pLua , int index )
    {
        return luaL_checknumber( pLua , index );
    }

    static double get( lua_State* pLua , int index )
    {
        return lua_tonumber(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return lua_isnumber( pLua , index );
    }
};


template<>
struct LuaTypeTraits<const char*>
{
    static constexpr LuaType Type = LuaType::CString;

    static void push( lua_State* pLua , const char* value )
    {
        lua_pushstring(pLua , value );
    }

    static const char* check( lua_State* pLua , int index )
    {
        return luaL_checkstring( pLua , index );
    }

    static const char* get( lua_State* pLua , int index )
    {
        return lua_tostring(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return lua_isstring( pLua , index );
    }
};


template<>
struct LuaTypeTraits<std::string>
{
    static constexpr LuaType Type = LuaType::CString;

    static void push( lua_State* pLua , const std::string& value )
    {
        lua_pushstring(pLua , value.c_str() );
    }

    static std::string check( lua_State* pLua , int index )
    {
        return std::string( luaL_checkstring( pLua , index ) , lua_objlen( pLua , index ) );
    }

    static std::string get( lua_State* pLua , int index )
    {
        return std::string(  lua_tostring(pLua , index ) , lua_objlen( pLua , index ) );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return lua_isstring( pLua , index );
    }
};


template<>
struct LuaTypeTraits<LuaFunction>
{
    static constexpr LuaType Type = LuaType::CFunction;

    static void push( lua_State* pLua , LuaFunction value )
    {
        lua_pushcfunction(pLua , value );
    }

    static LuaFunction check( lua_State* pLua , int index )
    {
        if( lua_iscfunction( pLua , index ) )
            return get( pLua , index );
        else
            return NULL;
    }

    static LuaFunction get( lua_State* pLua , int index )
    {
        return lua_tocfunction(pLua , index );
    }

    static bool isType( lua_State* pLua , int index )
    {
        return lua_iscfunction( pLua , index );
    }
};



#endif // LUA_TYPES_HPP
