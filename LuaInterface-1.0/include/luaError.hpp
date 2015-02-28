#ifndef LUA_ERROR_HPP
#define LUA_ERROR_HPP

class LuaError
{
public:
	LuaError():
		m_error( 0 )
		{}
		
	LuaError(int error):
		m_error( error )
		{}
		
	inline operator bool() const
	{
		return m_error != 0;
	}
		
	inline operator int() const
	{
		return m_error;
	}
	
private:
	int m_error;
};


#endif // LUA_ERROR_HPP
