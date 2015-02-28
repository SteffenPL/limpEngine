#ifndef LUA_ARRAY_HPP
#define LUA_ARRAY_HPP

template< typename T , const char* TypeName >
class LuaArray
{
	public:
		LuaArray():
			m_data( nullptr ),
			m_size( -1 )
			{}
	
	static int create( lua_State *pLua) 
	{
		int m_size = luaL_checkint(pLua, 1);
		size_t nbytes = m_size * sizeof(T);
		T* m_data = (T*)lua_newuserdata(pLua, nbytes);
      
		return 1;  /* new userdatum is already on the stack */
    }
	
	static int set( lua_State *pLua )
	{
		return 1;
	}
	
	
	private:
		T* m_data;
		int m_size;
};

#endif // LUA_ARRAY_HPP
