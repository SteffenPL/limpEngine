#ifndef LIMP_CONFIG_MANAGER_HPP
#define LIMP_CONFIG_MANAGER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include "luaInterface.hpp"

namespace Limp
{

using std::string;

// Löscht die Leerzeichen aus einem String
void trim(string& str);


// Läd eine Konfigurationsdatei mit .readFile( Datenname )
// Zugriff auf die Daten mit .get< Datentyp >( Name der Einstellung )
// Unterstützt: bool , Integer, Gleitkommazahlen, string
// Ändern von Werten mit .set< Datentyp >( Name der Einstellung , Wert )
// Speichern in eine Datei mit .writeFile( Datenname )
class ConfigManager
{

public:

    ConfigManager(){};
    ConfigManager( const string& fileName , bool clearAll = true )
    {
        readFile( fileName , clearAll );
    }

    bool        readFile(const string& fileName, bool clearAll = true);

    template<typename T>
    T   get(const string& key );

    template<typename T>
    T   get(const string& key , const T& defaultValue );

    template<typename T>
    void set(const string& key,const T& value);

    bool        writeFile(const string& fileName);

/*
    static constexpr const char* Name = "LimpConfigManager";

    static void registerClass( LuaState& lua )
    {
        static LuaLib functions[] =
        {
            { "new" , LuaConstructor<ConfigManager()>::LuaCFunction },
            libEnd
        };

        static LuaLib methods[] =
        {
            { "setBool"   , LuaMethod( ConfigManager , void(const string&,bool)   , ConfigManager::set<bool>   ) },
            { "setInt"    , LuaMethod( ConfigManager , void(string,int)    , ConfigManager::set<int>    ) },
            { "setStr"    , LuaMethod( ConfigManager , void(string,string) , ConfigManager::set<string> ) },
            { "setFloat"  , LuaMethod( ConfigManager , void(string,float)  , ConfigManager::set<float>  ) },
            { "setDouble" , LuaMethod( ConfigManager , void(string,double) , ConfigManager::set<double> ) },

            { "getBool"   , LuaMethod( ConfigManager , bool(string)   , ConfigManager::get<bool>   ) },
            { "getInt"    , LuaMethod( ConfigManager , int(string)    , ConfigManager::get<int>    ) },
            { "getStr"    , LuaMethod( ConfigManager , string(string) , ConfigManager::get<string> ) },
            { "getFloat"  , LuaMethod( ConfigManager , float(string)  , ConfigManager::get<float>  ) },
            { "getDouble" , LuaMethod( ConfigManager , double(string) , ConfigManager::get<double> ) },

            //{ "__gc" , LuaDestructor< ConfigManager >::LuaCFunction },
            libEnd
        };


        LuaClass< ConfigManager > luaClass( lua );
        luaClass.registerFunctions(functions);
        luaClass.registerMethods(methods);
    }
*/
private:
    std::map<string , string> m_content;
};



template<typename T>
T   ConfigManager::get(const string& key )
{
    std::stringstream ss;
    T res;
    auto it = m_content.find(key);
    if( it != m_content.end() )
        ss << it->second;
    ss >> res;
    return res;
}

template<typename T>
T   ConfigManager::get(const string& key  , const T& defaultValue)
{
    if( m_content.find(key) != m_content.end() )
        return get<T>(key);
    return defaultValue;
}

template<>
inline string ConfigManager::get<string>(const string& key )
{
    auto it = m_content.find(key);
    if( it != m_content.end() )
        return it->second;
    return "";
}

template<>
inline  bool ConfigManager::get<bool>(const string& key)
{
    auto it = m_content.find(key);
    if( it != m_content.end() )
        if( it->second == "true" || it->second == "1" )
            return true;
    return false;
}

template<typename T>
void ConfigManager::set(const string& key,const T& value)
{
    std::stringstream ss;
    ss << value;
    m_content[key] = ss.str();
}

}

void Limp::trim(string& str)
{
    while( str[0] == 9 || str[0] == 32)
        str.erase(0,1);

    while( str.back() == 9 || str.back() == 32 )
        str.erase( str.end() - 1 );
}

bool Limp::ConfigManager::readFile(const string& fileName, bool clearAll )
{
    std::ifstream file;
    file.open(fileName.c_str());

    if( !file.is_open() )
        return false;

    if( clearAll )
        m_content.clear();

    string line;
    string key;
    string value;
    size_t pos;
    while( !file.eof() )
    {
        getline(file,line);

        pos = line.find("//");
        if( pos != line.npos )
            line.erase(pos);

        pos = line.find('=');

        if( pos != line.npos )
        {
            key = line.substr(0,pos);
            value = line.substr(pos + 1);

            trim(key);
            trim(value);
            m_content[key] = value;
        }
    }

    file.close();

    return true;
}

bool        Limp::ConfigManager::writeFile(const string& fileName)
{
    std::ofstream file(fileName);

    if( !file.is_open() )
        return false;

    for( auto it = m_content.begin() ; it != m_content.end() ; ++it)
        file << it->first << "=" << it->second << std::endl;

    file.close();
    return true;
}


#endif // LIMP_CONFIG_MANAGER_HPP
