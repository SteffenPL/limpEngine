#ifndef LIMP_TEXTURE_HPP
#define LIMP_TEXTURE_HPP

#include "GL/glew.h"

#include "SFML/Graphics/Image.hpp"
#include <vector>

namespace Limp
{
    enum class TextureType : GLenum
    {
        RGBA = GL_RGBA,
        RGB = GL_RGB,
        Red = GL_RED,
        Green = GL_GREEN,
        Blue = GL_BLUE,
        Alpha = GL_ALPHA,
        Depth = GL_DEPTH
    };

    class Texture2D
    {
    public:

            Texture2D():
                m_id(0),
                m_unit(0)
            {}

            void setUnit( int unit )
            {
                m_unit = unit;
            }

            int getUnit() const
            {
                return m_unit;
            }

            int bind()
            {
                glActiveTexture( GL_TEXTURE0 + m_unit );
                glBindTexture(GL_TEXTURE_2D, m_id );
                return true;
            }

            int bind( int unit  )
            {
                m_unit = unit;
                glActiveTexture( GL_TEXTURE0 + unit );
                glBindTexture(GL_TEXTURE_2D, m_id );
                return true;
            }

            int unbind();

            bool loadFromFile( const std::string& path )
            {
                std::vector< GLubyte > pixels;
                sf::Vector2u size;
                sf::Image image;
                image.loadFromFile( path );
                size = image.getSize();
                const GLubyte* pPixels = image.getPixelsPtr();

                if( m_id != 0 )
                    glDeleteTextures(1,&m_id);

                glGenTextures(1,&m_id);
                glBindTexture(GL_TEXTURE_2D , m_id );
                glTexImage2D(	GL_TEXTURE_2D, 0, 3, (GLsizei)size.x , (GLsizei)size.y ,
                                0, GL_RGBA , GL_UNSIGNED_BYTE, pPixels );

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

                return true;
            }

            void clear()
            {
                if( m_id != 0 )
                    glDeleteTextures(1, &m_id );
            }

            ~Texture2D()
            {
                clear();
            }

        private:
            GLuint m_id;
            int m_unit;
    };

}

#endif //LIMP_TEXTURE_HPP
