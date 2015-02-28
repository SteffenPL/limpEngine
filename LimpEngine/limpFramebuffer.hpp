#ifndef LIMP_FRAMEBUFFER_HPP
#define LIMP_FRAMEBUFFER_HPP

#include "GL/glew.h"

namespace Limp
{
	class Framebuffer
	{
	public:
		
		Framebuffer( int width , int height )
		{
			glGenFramebuffers( 1 , &m_fboID );
		}
		
		
		void createRenderTargets( int count )
		{
			GLuint* pIDs;
			glGenRenderbuffers( count , pIDs );
			m_renderTargets = std::vector<Gluint>(pIDs , count);
		}

		void bindFramebuffer()
		{
			glBindFramebuffer( GL_FRAMEBUFFER , m_fboID );
		}
		
		void createTextures( int count )
		{
			GLuint* textures;
			glGenTextures( count , textures );
			
			for( int i = 0 ; i < count ; ++i )
			{
				GLuint tex = textures[i];
				glBindTexture( GL_TEXTURE_2D , tex);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  512, 512, 0, GL_RGBA8, GL_UNSIGNED_BYTE, nil);
			}
		}
		
		void createFramebufferTargets( int count )
		{
			
		}
		
		void unbindFramebuffer()
		{
			glBindFramebuffer( GL_FRAMEBUFFER , 0 );
		}

		void enableTarget( int index )
		{
		}
		
		void disableTargets();
		
		void clear()
		{
			glDeleteFramebuffers( 1 , &m_fboID );
			
			for( int i = 0 ; i < m_renderTargetCount ; ++i )
				glDeleteRenderbuffers( m_render )
		}
		
		~Framebuffer();
		
	private:
	
		GLuint 	m_fboID;
		int 	m_renderTargetCount;
		GLuint* m_renderTargets;
		
		int m_width;
		int m_height;
	};
}

#endif // LIMP_FRAMEBUFFER_HPP
