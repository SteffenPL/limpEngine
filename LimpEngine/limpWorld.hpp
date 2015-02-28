#ifndef LIMP_WORLD_HPP
#define LIMP_WORLD_HPP

#include <list>

#include "limpObject.hpp"
#include "luaInterface.hpp"

class AbstractObject;

namespace Limp
{
    class World
    {
        public:

            void simulate()
            {

            }


            void render()
            {

            }


            static constexpr const char* Name = "LimpWorld";

            static void registerClass( LuaState& lua )
            {
                static LuaLib functions[] =
                {
                    { "new" , LuaConstructor<World()>::LuaCFunction },
                    libEnd
                };

                static LuaLib methods[] =
                {
                    { "render", LuaMethod( World , void() , render ) },
                    { "__gc" , LuaDestructor< World >::LuaCFunction },
                    libEnd
                };


                LuaClass< World > luaClass( lua );
                luaClass.registerFunctions(functions);
                luaClass.registerMethods(methods);
            }

        private:

        std::list<AbstractObject*> m_objects;

    };
}

#endif //LIMP_WORLD_HPP
