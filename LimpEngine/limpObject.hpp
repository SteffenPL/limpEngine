#ifndef LIMP_OBJECT_HPP
#define LIMP_OBJECT_HPP

#include "limpMath.hpp"
#include "limpWorld.hpp"
#include "limpCamera.hpp"
#include "limpOpenGLRenderer.hpp"

namespace Limp
{
    class AbstractObject
    {
        public:

            virtual void move( const Vector3D& v ) = 0;
            virtual void scale( float f ) = 0;
            virtual void rotate( float angle , const Vector3D& v ) = 0;

            virtual void simulate() = 0;


        private:
    };

}

#endif //LIMP_OBJECT_HPP
