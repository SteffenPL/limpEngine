#ifndef LIMP_SCENE_HPP
#define LIMP_SCENE_HPP

#include "limpOpenGL.hpp"

#include <vector>

namespace Limp
{
	using std::vector;
	class Scene
	{
	public:
		void draw()
		{
			for( BaseMesh* pMesh : m_meshes )
			{
				pMesh->draw();
			}
		}
			
	private:
		vector<BaseMesh*> m_meshes;
		
	};
}

#endif LIMP_SCENE_HPP
