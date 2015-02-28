#ifndef LIMP_UI_HPP
#define LIMP_UI_HPP

#include "limpObject.hpp"

namespace Limp
{
	
	
	class BaseUiObject : public Object
	{
	public:
		virtual void click();
		virtual void hover();

	};
	
	template< class T >
	class UiObject
	{
		
	};
	
	class BaseUI
	{
		using Slot = std::function< void(  ) >;
		
	public:
	private:
		
	};
}

#endif //LIMP_UI_HPP
