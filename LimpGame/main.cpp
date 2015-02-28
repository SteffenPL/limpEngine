#include "limpEngine.hpp"
#include "luaInterface.hpp"

#include "game.hpp"

#include <iostream>


using namespace Limp;
using namespace std;


class Box
{
public:
    Box():boxCount(0),pos(nullptr){}

    Box(int n):boxCount(n),pos( new Limp::Vector3D[n]){for(int i = 0 ; i < n ; ++i ) pos[i] = Limp::Vector3D();}
    ~Box()
    {

        //cout << "\nDeleted: Box " << boxCount << endl;
        delete[] pos;
        pos = nullptr;
        boxCount = 0;
    }

    void init(int n)
    {
        delete[] pos;

        boxCount =n;
        pos = new Limp::Vector3D[n];
        for(int i = 0 ; i < n ; ++i )
            pos[i] = Limp::Vector3D();
    }

    int hello(){cout<<"\nHello\n";return 0;}

    float getXPos(int i){return pos[i].x;}
    float getYPos(int i){return pos[i].y;}
    float getZPos(int i){return pos[i].z;}

    void setXPos(int i, float x){pos[i].x=x;}
    void setYPos(int i, float y){pos[i].y=y;}
    void setZPos(int i, float z){pos[i].z=z;}


    void setPos( int i,float x,float y,float z){pos[i] = Limp::Vector3D(x,y,z);}

    int getBoxCount(){return boxCount;}

    Box* newBox(int n )
    {
        return new Box(n);
    }

    static constexpr const char* Name = "Box";

    static void registerClass( LuaState& lua )
    {
        static LuaLib functions[] =
        {
            { "new" , LuaConstructor<Box(int)>::LuaCFunction },
            libEnd
        };

        static LuaLib methods[] =
        {
            { "hello" , LuaMethod( Box , int(void) , hello ) },
            { "newBox", LuaMethod( Box , Box*(int) , newBox ) },
            { "init" , LuaMethod( Box , void(int) , init ) },
            { "getX" , LuaMethod( Box , float(int) , getXPos ) },
            { "getY" , LuaMethod( Box , float(int) , getYPos ) },
            { "getZ" , LuaMethod( Box , float(int) , getZPos ) },
            { "setX" , LuaMethod( Box , void(int,float) , setXPos ) },
            { "setY" , LuaMethod( Box , void(int,float) , setYPos ) },
            { "setZ" , LuaMethod( Box , void(int,float) , setZPos ) },
            { "setPos" , LuaMethod( Box , void(int,float,float,float) , setPos ) },
            { "getBoxCount" , LuaMethod( Box , int(void) , getBoxCount ) },
            { "__gc" , LuaDestructor< Box >::LuaCFunction },
            libEnd
        };


        LuaClass< Box > luaClass( lua );
        luaClass.registerFunctions(functions);
        luaClass.registerMethods(methods);
    }

    int boxCount;
    Limp::Vector3D* pos;

};

int main()
{
    Game g;

    bool ok = g.init();

    if( ok )
        g.run();

    return ok ? 0 : 1;
}
