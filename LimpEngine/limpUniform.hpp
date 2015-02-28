#ifndef LIMP_UNIFORM_HPP
#define LIMP_UNIFORM_HPP

#include "GL/glew.h"

#include "limpShader.hpp"

#include "limpMath.hpp"
#include "limpColor.hpp"
#include "limpTexture.hpp"

#include <string>

namespace Limp
{
    template<typename T>
    class Uniform
    {
    public:
        Uniform<T>():
            m_id(0),
            m_pProgram(NULL),
            m_size(0),
            m_name()
            {}


        Uniform<T>( Program& p , const std::string& name , int size = 1 ):
            Uniform<T>()
        {
            init(p,name,size);
        }


        void init( Program& p , const std::string& name , int size = 1 )
        {
            if( m_pProgram == &p && name == m_name && size == m_size )
                return;

            m_id = glGetUniformLocation( p.getID() , name.c_str());
            m_pProgram = &p;
            m_size = size;
            m_name = name;
        }

        bool isInit() const
        {
            return m_id != 0;
        }

        void setSize( int size )
        {
            m_size = size;
        }

        int getSize() const
        {
            return m_size;
        }

        Uniform<T>& operator=( const T* pValue )
        {
            set(pValue);
            return *this;
        }

        Uniform<T>& operator=( const T& pValue )
        {
            if( m_size == 1 )
                set(&pValue);
            return *this;
        }

        void set( const T* pValue );

        void set( Program& p , const T* pValue )
        {
            if( &p != m_pProgram )
            {
                m_id = p.getUniformLocation( m_name );
                m_pProgram = &p;
            }
            set( pValue );
        }

        private:
            GLuint 		m_id;
            Program* 	m_pProgram;
            int 		m_size;
            std::string m_name;

    };


    template< typename ... Args>
    class UniformStruct;

    template< typename T, typename ... Args >
    class UniformStruct<T,Args...>
    {
        static const unsigned short int argCount = sizeof...(Args);

    public:

        void init( Program& p , const std::vector<std::string>& memberNames
                   , const std::string& prefix = "" , int offset = 0 )
        {
            if( offset >= memberNames.size() )
                return;

            m_uniformT.init(    p , memberNames , prefix , offset );
            m_uniformArgs.init( p , memberNames , prefix , ++offset );
        }

        void set( const T* pValue , const Args*... args )
        {
            m_uniformT.set( pValue );
            m_uniformArgs.set( args... );
        }

        void set( const T& pValue , const Args&... args )
        {
            m_uniformT.set( pValue );
            m_uniformArgs.set( args... );
        }

    private:
        UniformStruct<T>       m_uniformT;
        UniformStruct<Args...> m_uniformArgs;
    };

    template< typename T >
    class UniformStruct<T>
    {
    public:
        inline void init( Program& p , const std::vector<std::string>& memberNames
                          , const std::string& prefix = "" , int offset = 0 )
        {
            if( offset >= memberNames.size() )
                return;

            m_uniform.init( p , prefix + memberNames[offset] );
        }

        inline void set( const T* pValue  )
        {
            m_uniform.set( pValue );
        }

        inline void set( const T& pValue  )
        {
            m_uniform = pValue ;
        }
    private:
        Uniform<T> m_uniform;
    };
/*
    template<>
    class UniformStruct<PointLight>
    {
        void init( Program& p , const std::string& varName )
        {
            m_uniformStruct.init( p ,  )
        }

    private:
        //UniformStruct<float,float,float,float> m_uniformStruct;
    };
*/
    template<>
    void Uniform<float>::set(const float* pValue)
    {
        glUniform1fv(m_id,m_size,pValue);
    }

    template<>
    void Uniform<int>::set( const int* pValue)
    {
        glUniform1iv(m_id,m_size,pValue);
    }

    template<>
    void Uniform<GLuint>::set( const GLuint* pValue)
    {
        glUniform1uiv(m_id,m_size,pValue);
    }

    template<>
    void Uniform<Vector2D>::set( const Vector2D* pValue)
    {
        glUniform2fv(m_id,m_size,(float*)pValue);
    }

    template<>
    void Uniform<Vector3D>::set( const Vector3D* pValue)
    {
        glUniform3fv(m_id,m_size,(float*)pValue);
    }

    template<>
    void Uniform<Vector4D>::set( const Vector4D* pValue)
    {
        glUniform4fv(m_id,m_size,(float*)pValue);
    }

    template<>
    void Uniform<Color3f>::set( const Color3f* pValue)
    {
        glUniform3fv(m_id,m_size,(float*)pValue);
    }

    template<>
    void Uniform<Color4f>::set( const Color4f* pValue)
    {
        glUniform4fv(m_id,m_size,(float*)pValue);
    }

    template<>
    void Uniform<Matrix4x4>::set( const Matrix4x4* pValue)
    {
        glUniformMatrix4fv(m_id,m_size,true,pValue->getPointer());
    }

    template<>
    void Uniform<Texture2D>::set( const Texture2D* pValue )
    {
        int textureUnits[m_size];
        for( int i = 0 ; i < m_size ; ++i )
            textureUnits[i] = pValue[i].getUnit();
        glUniform1iv( m_id, m_size , textureUnits );
    }



}

#endif //LIMP_UNIFORM_HPP
