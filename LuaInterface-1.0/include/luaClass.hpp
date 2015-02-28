#ifndef LUA_CLASS_HPP
#define LUA_CLASS_HPP

#include "lua.hpp"
#include "luaTable.hpp"
#include "luaLib.hpp"

template< typename T>
class LuaClass
{
	public:	
		
		LuaClass( lua_State* pLua ):
			m_pLua( pLua  )
		{
			luaL_newmetatable(m_pLua, T::Name );
    
			lua_pushstring(m_pLua, "__index");
			lua_pushvalue(m_pLua, -2);  /* pushes the metatable */
			lua_settable(m_pLua, -3);  /* metatable.__index = metatable */
			
			lua_settop( m_pLua , 0 );
		}
		
		void registerFunctions( LuaLib* lib )
		{
			luaL_getmetatable( m_pLua , T::Name );
			luaL_register( m_pLua , T::Name , lib );
			lua_settop( m_pLua , 0 );
		}
		
		void registerMethods( LuaLib* lib )
		{
			luaL_getmetatable( m_pLua , T::Name );
			luaL_register( m_pLua , NULL , lib );
			lua_settop( m_pLua , 0 );
		}
		
	private:	
		lua_State* m_pLua;
};




#endif //LUA_CLASS_HPP
