#ifndef LUA_TABLE_HPP
#define LUA_TABLE_HPP

#include "lua.hpp"

#include "luaStack.hpp"

template< typename KeyType = const char* >
class LuaTable 
{
	public:
		LuaTable( lua_State* pLua  , const std::string& name , const bool createNew = true ):
			m_pLua( pLua ),
			m_name(name)
		{
			if( createNew )
			{
				lua_newtable( m_pLua );
				lua_setglobal( m_pLua , name.c_str() );
			}
		}
			
		template< typename T>
		void set( const KeyType& key , const T& value )
		{
			lua_getglobal( m_pLua , m_name.c_str() );
			m_stack.push<KeyType>( key );
			m_stack.push<T>( value );			
			lua_settable( m_pLua , -3 );
		}
		
		template< typename T>
		T get( const KeyType& key )
		{
			lua_getglobal( m_pLua , m_name.c_str() );			
			m_stack.push<KeyType>( key );
			lua_gettable(m_pLua, -2);
			
			T result = LuaTypeTraits<T>::get( m_pLua , -1 );
			
			m_stack.pop(1);
			return result;
		}
		
		std::string getName()
		{
			return m_name;
		}
		
		lua_State* getState()
		{
			return m_pLua;
		}
		
		
	protected:		
		union
		{
			lua_State* 	m_pLua;
			LuaStack	m_stack;
		};
		
		const std::string& m_name;
};

/*
template< typename KeyType = const char* >
class LuaMetatable 
{
	public:
		LuaMetatable( lua_State* pLua , const std::string& name , const bool createNew = true ):
			m_pLua( pLua ),
			m_name(name)
		{
			if( createNew )
			{
				luaL_newmetatable( m_pLua  , name.c_str() );
				m_stack.pop(1);
			}
		}
			
		template< typename T>
		void set( const KeyType& key , const T& value )
		{
			luaL_getmetatable( m_pLua , m_name.c_str() );
			m_stack.push<KeyType>( key );
			m_stack.push<T>( value );
		
			lua_settable( m_pLua , -3 );
		}
		
		template< typename T>
		T get( const KeyType& key )
		{
			luaL_getmetatable( m_pLua , m_name.c_str() );		
			m_stack.push<KeyType>( key );
			lua_gettable(m_pLua, -2);
			
			T result = LuaTypeHelpers<T>::get( m_pLua , -1 );
			
			m_stack.pop(1);
			return result;
		}
		
		std::string getName()
		{
			return m_name;
		}
		
		lua_State* getState()
		{
			return m_pLua;
		}
		
		void setDefaultIndex()
		{
			luaL_getmetatable( m_pLua , m_name.c_str() );
			
			m_stack.push<const char*>( "__index" );
			m_stack.copy( -2 );
					
			lua_settable( m_pLua , -3 );
		}	
		
		
	protected:		
		union
		{
			lua_State* 	m_pLua;
			LuaStack	m_stack;
		};
		
		const std::string& m_name;
			
};

*/

#endif //LUA_TABLE_HPP
