#ifndef LIMP_GAME_HPP
#define LIMP_GAME_HPP

namespace Limp
{
	class BaseGame
	{
	public:
		virtual bool isOpen();
		virtual bool init();
		virtual bool simulate();
		virtual bool update();
		
	private:
	};
	
};

#endif //LIMP_GAME_HPP
