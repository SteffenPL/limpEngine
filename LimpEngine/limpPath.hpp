#ifndef LIMP_PATH_HPP
#define LIMP_PATH_HPP

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace Limp
{
    using std::string;
    using std::vector;

    using Path = boost::filesystem::path;

    class MPath
    {
        /*
        public:
            Path( const char* cStr ):
                Path( string(cStr) )
            {}

            Path( const string& path )
            {
                string tmp = "";
                for( char c : path )
                {
                    tmp += c;
                    if( c == '/' ||c == '\\' )
                    {
                        m_path.push_back(tmp);
                        tmp = "";
                    }
                }

                bool parsingName = true;
                for( char c : tmp )
                {

                    if( c == '.' )
                        parsingName = true;

                    if( parsingName )
                        m_fileName += c;
                    else
                        m_fileType += c;
                }

            }

            Path& operator=(const string& str )
            {
                *this = Path(str);
                return *this;
            }

            Path& operator=(const char* c_str )
            {
                *this = Path( std::string(c_str) );
                return *this;
            }

            operator string() const
            {
                return string(getPath() + m_fileName + m_fileType);
            }

            const char* c_str() const
            {
                return string(getPath() + m_fileName + m_fileType).c_str();
            }

            string getPath() const
            {
                string tmp;
                for( const string& str : m_path )
                    tmp += str;
                return tmp;
            }

            const string& getFileName() const
            {
                return m_fileName;
            }

            const string& getFileType() const
            {
                return m_fileType;
            }

        private:
            vector<string> m_path;
            string m_fileName;
            string m_fileType;*/

    };
}

#endif //LIMP_PATH_HPP
