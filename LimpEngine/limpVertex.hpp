#ifndef LIMP_VERTEX_HPP
#define LIMP_VERTEX_HPP

#include <tuple>

#include "GL/glew.h"
#include "limpMath.hpp"
#include "limpTypes.hpp"

namespace Limp
{
	
struct AbstractVertex
{
	public:
		static void  setPointers();
		static void  enable();
		static void  disable();
		
		template< typename T >
		void setPosition( const T& v )
		{}
		
		template< typename T >
		void setNormal( const T& v )
		{}
		
		template< typename T >
		void setColor( const T& col )
		{}
		
		template< typename T >
		void setTexCoord( const T& v , int index = 0 )
		{}
		
		template< typename T >
		void setMaterial( const T& v )
		{}
		
	protected:
		/*
		static void setPositionPointer( size_t size , size_t& offset );		
		static void setNormalPointer( size_t size , size_t& offset );
		static void setColor3fPointer( size_t size , size_t& offset );
		static void setColor4fPointer( size_t size , size_t& offset );
		static void setColor3uiPointer( size_t size , size_t& offset );
		static void setColor4uiPointer( size_t size , size_t& offset );
		static void setTexCoord2fPointer( size_t size , size_t& offset );		
		static void setTexCoord3fPointer( size_t size , size_t& offset );
			*/
			
        template< typename T , size_t  N >
        static void setAttributePointer( int index , size_t size , size_t* offset )
		{
            glVertexAttribPointer( (GLuint)index , N , LimpTypeTraits<T>::GLName , false , size , (GLvoid*)*offset );
			*offset += sizeof(T) * N;
		}
			
		static constexpr GLenum Position = GL_VERTEX_ARRAY;
		static constexpr GLenum Normal   = GL_NORMAL_ARRAY;
		static constexpr GLenum Color    = GL_COLOR_ARRAY;
		static constexpr GLenum TexCoord = GL_TEXTURE_COORD_ARRAY;
};

/*
static void AbstractVertex::setPositionPointer( size_t size , size_t& offset )
{
	glVertexPointer( 3 , GL_FLOAT , size , (GLvoid*)offset );	
	offset += 3 * sizeof( float );
}

static void AbstractVertex::setNormalPointer( size_t size , size_t& offset )
{
	glNormalPointer( GL_FLOAT , size , (GLvoid*)offset );
	offset += 3 * sizeof( float );
}


static void AbstractVertex::setColor3fPointer( size_t size , size_t& offset )
{
	glColorPointer( 3 , GL_FLOAT , size , (GLvoid*)offset );
	offset += 3 * sizeof(float);
}

static void AbstractVertex::setColor4fPointer( size_t size , size_t& offset )
{
	glColorPointer( 4 , GL_FLOAT , size , (GLvoid*)offset );
	offset += 4 * sizeof(float);
}

static void AbstractVertex::setColor3uiPointer( size_t size , size_t& offset )
{
	glColorPointer( 3 , GL_UNSIGNED_BYTE , size , (GLvoid*)offset );
	offset += 3 * sizeof(char);
}

static void AbstractVertex::setColor4uiPointer( size_t size , size_t& offset )
{
	glColorPointer( 4 , GL_UNSIGNED_BYTE , size , (GLvoid*)offset );
	offset += 4 * sizeof(char);
}

static void AbstractVertex::setTexCoord2fPointer( size_t size , size_t& offset )
{
	glTexCoordPointer( 2 , GL_FLOAT , size , (GLvoid*)offset );
	offset += 2 * sizeof(float);
}

static void AbstractVertex::setTexCoord3fPointer( size_t size , size_t& offset )
{
	glTexCoordPointer( 3 , GL_FLOAT , size , (GLvoid*)offset );
	offset += 3 * sizeof(float);
}
*/


}

#endif //LIMP_VERTEX_HPP
