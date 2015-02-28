#ifndef LIMP_SHADER_HPP
#define LIMP_SHADER_HPP

#include <list>
#include <fstream>
#include <iostream>

#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include "limpLog.hpp"
#include "limpTypes.hpp"

namespace Limp
{

    class Shader;
    class Program;
    class ShaderManager;


    enum class ShaderType : unsigned int
    {
        None = 0,
        VertexShader = GL_VERTEX_SHADER,
        GeometrieShader = GL_GEOMETRY_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER
    };

    using std::cout;
    using std::endl;
    class Shader
    {
    public:

        Shader(): m_id(0) {}

        Shader(ShaderType type, const std::string& fileName )
        {
            compileFromFile(type,fileName);
        }

        void compileFromStr( ShaderType type, const std::string& str )
        {
             const char* fileData = str.c_str();
             GLint lenght = str.length();
             glShaderSource(m_id, 1, &(fileData) , &lenght );

             glCompileShader( m_id );

        }

        void compileFromFile( ShaderType type, const std::string& fileName )
        {
            Log::getInstance().write("OpenGL" , "'" + fileName + "' wird geladen...");
            m_id = glCreateShader( (GLenum)type );
            m_type = type;

            if( !isInit() )
                clear();

            std::ifstream file(fileName.c_str());

            if( !file.is_open() )
                return;

            std::string fileStr((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());

            compileFromStr( type , fileStr );
            Log::getInstance().write("OpenGL" , "'" + fileName + "': " + getError() );
        }

        bool getStatus() const
        {
            if( !isInit() )
                return false;

            GLint compiled;
            glGetShaderiv(m_id, GL_COMPILE_STATUS, &compiled);

            if( compiled )
                return true;

            return false;
        }

        std::string getError() const
        {
            std::string error;
            if( !isInit() )
                error = " Shader wurde nicht initialisiert.";
            else
            {
                if(getStatus())
                    error = "Erfolgreich compiliert.";
                else
                {
                    GLint infoLogLength;
                    glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

                    GLchar* infoLog = new GLchar[infoLogLength];
                    glGetShaderInfoLog(m_id, infoLogLength, NULL , infoLog);

                    error = std::string( infoLog , infoLogLength);
                    delete[] infoLog;
                }
            }

            switch( m_type )
            {
                case ShaderType::VertexShader: error = "VertexShader: " + error ;break;
                case ShaderType::GeometrieShader: error = "GeometrieShader: " + error ;break;
                case ShaderType::FragmentShader: error = "FragmentShader: " + error ;break;
                default:break;
            }

            return error;
        }

        GLuint getID() const
        {
            return m_id;
        }

        GLenum getType() const
        {
            return (GLenum)m_type;
        }

        bool isInit() const
        {
            return ( m_id != 0 ) && ( m_type != ShaderType::None );
        }

        void clear()
        {
            if( m_id != 0 )
            {
                glDeleteShader( m_id );
                m_id = 0;
                m_type = ShaderType::None;
            }
        }

        ~Shader()
        {
            clear();
        }

        private:

        GLuint 		m_id;
        ShaderType 	m_type;
    };


    class Program
    {
        public:

            Program( const std::list<Shader*>& shaderList )
            {
                loadShaderList(shaderList);
            }

            Program(): m_id(0)
            {
            }

            bool loadShaderList( const std::list<Shader*>& shaderList )
            {
                clear();

                m_id = glCreateProgram();

                for( Shader* const & x : shaderList )
                    glAttachShader( m_id , x->getID() );

                glLinkProgram( m_id );

                for( Shader* const& x : shaderList )
                    glDetachShader(m_id, x->getID() );

                Log::getInstance().write("OpenGL" , "Program: " + getError() );
                return true;
            }

            bool getStatus() const
            {

                GLint status;
                glGetProgramiv (m_id, GL_LINK_STATUS, &status);

                if (status)
                    return true;
                return false;
            }

            std::string getError() const
            {

                GLint infoLogLength;
                glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

                GLchar *infoLog = new GLchar[infoLogLength];
                glGetProgramInfoLog(m_id, infoLogLength, &infoLogLength , infoLog);

                std::string error( infoLog , infoLogLength );
                delete[] infoLog;

                return error;
            }

            GLuint getID() const
            {
                return m_id;
            }

            void clear()
            {
                if( m_id != 0 )
                {
                    glDeleteProgram( m_id );
                    m_id = 0;
                }
            }

            GLuint getUniformLocation( const std::string& uniformName )
            {
                if( m_uniforms.find(uniformName) != m_uniforms.end() )
                    return m_uniforms[uniformName];

                GLuint id = glGetUniformLocation( m_id , uniformName.c_str() );
                if( id > 0 )
                {
                    m_uniforms[uniformName] = id;
                }
            }

            ~Program()
            {
                clear();
            }


        private:
            GLuint m_id;

            std::map<std::string,GLuint> m_uniforms;

    };



    struct ShaderSnippet
    {
        ShaderType type;

        std::string defines;
        std::string global;
        std::string local;

    public:
        ShaderSnippet( ShaderType _type ,
                       std::string _defines ,
                       std::string _global ,
                       std::string _local ):
            type( _type ),
            defines( _defines ),
            global( _global ),
            local( _local )
      {}



        void loadFromStr( const std::string& str )
        {
            defines = getContent("defines" , str );
            global  = getContent("global"  , str );
            local   = getContent("local"   , str );
        }

/*
        static std::list<ShaderSnippet> loadFromFile( const std::string& fileName )
        {
            std::ifstream file( fileName.c_str() );
            if( !file.is_open() )
                return;

            std::string tmp = "";

            std::string line = "";

            while( file.good() )
            {
                std::getline( file , line );

                size_t tagStart = line.find( "<" );
                size_t tagEnd   = line.find( ">" );

                if( tagStart != line.npos && tagEnd != line.npos )
                {

                }

            }

            file.close();
        }*/

    private:


        static std::string getContent(const std::string& keyword , const std::string& str )
        {
            const std::string beginTag = "<"  + keyword + ">";
            const std::string endTag   = "</"  + keyword + ">";

            size_t start = str.find(beginTag);
            start += beginTag.size();
            size_t end   = str.find(endTag);

            if( start == str.npos || end == str.npos )
                return "";

            return str.substr( start , end - start );
        }
    };


    class ShaderGenerator
    {
    public:
        Shader generate( const std::list<ShaderSnippet>& shaderSnippteList )
        {
            std::string defines , global , local;
            ShaderType type = shaderSnippteList.front().type;

            for( const ShaderSnippet& s : shaderSnippteList )
            {
                 defines += s.defines;
                 global += s.global;
                 local += s.local;
            }

            Shader shader;
            shader.compileFromStr( type , defines + global + local );
            return shader;
        }

    private:

    };

    class ShaderManager
    {
        public:

    };
}

#endif //LIMP_SHADER_HPP
