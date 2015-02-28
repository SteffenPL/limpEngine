#ifndef LIMP_LOG_HPP
#define LIMP_LOG_HPP

#include <string>
#include <fstream>
#include <iostream>

#include "luaInterface.hpp"


namespace Limp
{

    class Log : public std::ofstream
    {
        //singelton pattern
    public:
        static Log& getInstance()
        {
            static Log instance;
            return instance;
        }

    private:
        Log(){}

        Log(Log const&) = delete;
        void operator=(Log const&);

    public:

        void openFile(const std::string fileName)
        {
            if( !is_open() )
                open(fileName);
        }


        template< typename T >
        void write( std::string modul , T value )
        {
            if( good() )
                *this << modul << " :\t" << value << std::endl;
            std::cout << modul << " :\t" << value << std::endl;
        }

        template< typename T >
        void write( std::string modul , T badValue , bool ok  )
        {
            if(!ok )
                write(modul,badValue);
        }

        template< typename T >
        void write( std::string modul , T goodValue , T badValue , bool ok  )
        {
            if( ok )
                write(modul,goodValue);
            else
                write(modul,badValue);
        }

        ~Log()
        {
            if( is_open() )
                close();
        }

        static constexpr const char* Name = "LimpLog";

        static void registerClass( LuaState& lua )
        {
            static LuaLib methods[] =
            {
                { "writeBool" , LuaMethod( Log , void(std::string,bool) , write<bool> ) },
                { "writeInteger" , LuaMethod( Log , void(std::string,int) , write<int> ) },
                { "writeNumber" , LuaMethod( Log , void(std::string,float) , write<float> ) },
                { "writeString" , LuaMethod( Log , void(std::string,std::string) , write<std::string> ) },
                libEnd
            };

            static LuaLib functions[] =
            {
                { "new" , LuaSingeltonConstructor<Log>::LuaCFunction },
                libEnd
            };




            LuaClass<Log> luaClass( lua );
            luaClass.registerMethods(methods);
            luaClass.registerFunctions(functions);
        }

    };

}

#endif //LIMP_LOG_HPP
