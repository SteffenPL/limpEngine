#ifndef LUA_INTERFACE_HPP
#define LUA_INTERFACE_HPP

#include <string>
#include <map>
#include <tuple>
#include <functional>

#include "lua.hpp"

#include "luaError.hpp"
#include "luaTypes.hpp"
#include "luaFunctions.hpp"
#include "luaStack.hpp"
#include "luaTable.hpp"
#include "luaArray.hpp"
#include "luaClass.hpp"
#include "luaLib.hpp"

class LuaError;
class LuaState;
class LuaStack;
template< typename T >
class LuaClass;
using LuaCFunction = int (*)(lua_State*);

using LuaLib = const luaL_Reg ;


class LuaState
{

public:
    LuaState():
        m_reference(false)
    {
        m_pLua = luaL_newstate();
        luaL_openlibs( m_pLua );
    }

    LuaState(lua_State* pLua):
        m_pLua( pLua ),
        m_reference(true)
    {}

    LuaState( LuaState& lua ):
        m_pLua( lua ),
        m_reference(true)
    {}

    std::string getError()
    {
        return std::string( lua_tostring(m_pLua, -1) , lua_objlen(m_pLua , - 1) );
    }

    LuaError loadProgramm( const std::string& fileName )
    {
        return luaL_loadfile( m_pLua , fileName.c_str() );
    }

    LuaError executeString( const std::string& lua_code )
    {
        return luaL_dostring( m_pLua , lua_code.c_str() );
    }
    LuaError execute( int argCount = 0 , int returnCount = 0 , int errorFunc = 0 )
    {
        return lua_pcall( m_pLua , argCount , returnCount , errorFunc );
    }

    template< typename T>
    T get( const std::string& name )
    {
        lua_getglobal( m_pLua , name.c_str() );
        return m_stack.get<T>(-1);
    }



    template<typename T>
    void set( const std::string& name , const T& value )
    {
        m_stack.push<T>( value );
        lua_setglobal( m_pLua , name.c_str() );
    }

    template< typename R , typename... Args >
    R callFunction( const std::string& name , Args... args )
    {
        lua_getglobal( m_pLua , name.c_str() );
        m_stack.push( args... );

        int argCount = (int) sizeof...(Args);

        execute( argCount , 1 , 0 );

        R result = m_stack.get<R>( -1 );
        m_stack.pop(1);
        return result;
    }

    template<const char* Name , typename R , typename... Args>
    std::function<R(Args...)> getFunction( const std::string& name )
    {
        auto func = [name,this](Args... args)
            {
                return this->callFunction<R,Args...>(name,args...);
            };
        return func;
    }


    void registerFunction( const std::string&  name , LuaCFunction function)
    {
        lua_register( m_pLua , name.c_str() , function );
    }

    template< class T >
    void registerClass()
    {
        T::registerClass( *this );
    }

    operator lua_State* ()
    {
        return m_pLua;
    }

    lua_State* getState()	{	return m_pLua;	}

    LuaStack&	getStack()	{	return m_stack;	}


    ~LuaState()
    {
        if( !m_reference )
            lua_close(m_pLua);
    }

    private:

        union
        {
            lua_State* 	m_pLua;
            LuaStack	m_stack;
        };
        bool		m_reference;

};



#endif // LUA_INTERFACE_HPP
