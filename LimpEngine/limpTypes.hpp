#ifndef LIMP_TYPES_HPP
#define LIMP_TYPES_HPP

#include "GL/glew.h"

template< typename T>
struct LimpTypeTraits;

template<>
struct LimpTypeTraits<bool>
{
	static constexpr GLenum GLName = GL_BOOL;
};

template<>
struct LimpTypeTraits<short>
{
	static constexpr GLenum GLName = GL_SHORT;
};

template<>
struct LimpTypeTraits<unsigned short>
{
	static constexpr GLenum GLName = GL_UNSIGNED_SHORT;
};
template<>
struct LimpTypeTraits<int>
{
	static constexpr GLenum GLName = GL_INT;
	
};

template<>
struct LimpTypeTraits<unsigned int>
{
	static constexpr GLenum GLName = GL_UNSIGNED_INT;
};

template<>
struct LimpTypeTraits<float>
{
	static constexpr GLenum GLName = GL_FLOAT;
};

template<>
struct LimpTypeTraits<double>
{
	static constexpr GLenum GLName = GL_DOUBLE;
};

template<>
struct LimpTypeTraits<char>
{
	static constexpr GLenum GLName = GL_BYTE;
};

template<>
struct LimpTypeTraits<unsigned char>
{
	static constexpr GLenum GLName = GL_UNSIGNED_BYTE;
};

#endif //LIMP_TYPES_HPP
