#ifndef LIMP_BUFFER_OBJECT_HPP
#define LIMP_BUFFER_OBJECT_HPP


#include "GL/glew.h"

namespace Limp
{
	

class BufferObject
{
public:
    BufferObject():
        m_id( 0 )
    {}

    template<typename T>
    void create(T* pData , int count , GLenum target = GL_STATIC_DRAW )
    {
        clear();

        // Generiere neuen Buffer
        glGenBuffers(1, &m_id);
		
        // Binde den Buffer, um ihn nutzbar zu machen
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
		
        // Die VertexDaten kopieren. ( count * sizeof(T) = die Länge des Arrays in Byte )
        glBufferData( GL_ARRAY_BUFFER, count * sizeof(T) , pData, target);
		
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    template<typename T>
    T*  mapBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        return (T*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    }

    void unmapBuffer()
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    template<typename T>
    void updateData( T* pData , int start , int end )
    {
        T* ptr = mapBuffer<T>();
        if( ptr )
        {
            for( int i = start ; i <= end ; ++i )
            {
                ptr[i] = pData[i];
            }
            unmapBuffer();
        }
    }

    GLuint  getID() const   {        return m_id;       }
    bool    isInit() const  {        return (m_id != 0);   }

    void clear()
    {
        if( m_id != 0 )
		{
            glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
    }

    ~BufferObject()
    {
        clear();
    }

private:
    GLuint m_id;
};

};

#endif //LIMP_BUFFER_OBJECT_HPP
