#ifndef LUA_STACK_HPP
#define LUA_STACK_HPP

#include "luaTypes.hpp"
#include "lua.hpp"

class LuaStack
{
    public:
        LuaStack( lua_State* pLua ):
            m_pLua(pLua )
            {}

        ~LuaStack()
            {}

        int size()
        {
            return 0;
        }

        void clear()
        {
            lua_settop( m_pLua , 0 );
        }

        void copy( const int index )
        {
            lua_pushvalue( m_pLua , index );
        }

        void push(){}

        template<typename T>
        void push( const T& value )
        {
            LuaTypeTraits<T>::push( m_pLua , value );
        }

        template<typename T , typename... Args>
        void push( const T& value , Args&... args )
        {
            push<T>( value );
            push( args... );
        }

        void pop( int count )
        {
            lua_pop( m_pLua , count );
        }

        LuaType getType(int index = -1)
        {
            LuaType type = (LuaType)lua_type( m_pLua , index );

            if( type == LuaType::Number )
            {
                double value = get<double>( index );
                int intValue = value;

                if( value == (double)intValue )
                    return LuaType::Integer;
            }
            return type;
        }

        template< typename T>
        T get( int index )
        {
            return LuaTypeTraits<T>::get( m_pLua , index );
        }



        template<typename T>
        T check( int index )
        {
            return (T)LuaTypeTraits<T>::check( m_pLua , index );
        }

    private:
        lua_State* m_pLua;
};

#endif //LUA_STACK_HPP
