#include "limpConfigManager.hpp"
/*

void Limp::trim(std::string& str)
{
    while( str[0] == 9 || str[0] == 32)
        str.erase(0,1);

    while( str.back() == 9 || str.back() == 32 )
        str.erase( str.end() - 1 );
}

bool Limp::ConfigManager::readFile(const std::string& fileName, bool clearAll )
{
    std::ifstream file;
    file.open(fileName.c_str());

    if( !file.is_open() )
        return false;

    if( clearAll )
        m_content.clear();

    std::string line;
    std::string key;
    std::string value;
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

bool        Limp::ConfigManager::writeFile(const std::string& fileName)
{
    std::ofstream file(fileName);

    if( !file.is_open() )
        return false;

    for( auto it = m_content.begin() ; it != m_content.end() ; ++it)
        file << it->first << "=" << it->second << std::endl;

    file.close();
    return true;
}

*/