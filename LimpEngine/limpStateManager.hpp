#ifndef LIMP_STATE_MANAGER_HPP
#define LIMP_STATE_MANAGER_HPP

#include "luaInterface.hpp"

namespace Limp
{
	
enum class State : int
{
	Null = 0,
	
};
	
class StateManager
{
	public:
		void registerClasses();
		
		void 	setState( State s );
		State 	getState();
		
	private:

	State m_state;
	
	
};
	
}

#endif // LIMP_STATE_MANAGER_HPP