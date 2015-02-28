#ifndef LIMP_CAMERA_HPP
#define LIMP_CAMERA_HPP

#include "limpMath.hpp"

#include "luaInterface.hpp"

namespace Limp
{

    class Camera
    {
    public:
            Camera():
                m_pos(0.f,0.f,0.f),
                m_up(0.f,1.f,0.f),
                m_forward(0.f,0.f,-1.f),
                m_mat()
                {}


            void setPosition(  const Vector3D& position )
            {
                m_pos = position;
            }

            void move( const Vector3D& v )
            {
                Vector3D f = m_forward;
                m_pos += f * v.z;
                if( v.x != 0 )
                {
                    Vector3D side = cross( m_up , m_forward );
                    side.normalize();
                    m_pos += side * v.x;
                }
                m_pos += m_up * v.y;


            }

            void rotate( float angle , float x , float y , float z  )
            {
                Matrix4x4 tmp;

                tmp.rotate(angle , Vector3D(x,y,z));
                Vector3D newForward = tmp * m_forward;
                if( fabs(m_up * newForward) >= 0.99f )
                    return;

                newForward.normalize();
                m_forward = newForward;
            }

            void rotate( float angle , const Vector3D& v)
            {
                rotate( angle , v.x , v.y , v.z );
            }

            void yaw(float angle )
            {
                rotate( angle , m_up );
            }

            void pitch(float angle )
            {
                Vector3D side = cross( m_forward , m_up );
                side.normalize();
                rotate( angle , side );
            }

            void roll(float angle )
            {
                rotate( angle , m_forward );
            }

            void lookAt( const Vector3D& point )
            {
                m_forward = point - m_pos;
                m_forward.normalize();
            }

            void update()
            {
                m_mat.lookAt( m_pos , m_forward , m_up );
            }

            const Matrix4x4& getMatrix() const
            {
                return m_mat;
            }


            static constexpr const char* Name = "LimpCamera";

            static void registerClass( LuaState& lua )
            {
                static LuaLib functions[] =
                {
                    { "new" , LuaConstructor<Camera()>::LuaCFunction },
                    libEnd
                };

                static LuaLib methods[] =
                {
                    { "__gc" , LuaDestructor< Camera >::LuaCFunction },
                    libEnd
                };


                LuaClass< Camera > luaClass( lua );
                luaClass.registerFunctions(functions);
                luaClass.registerMethods(methods);
            }

        private:

            Vector3D m_pos;
            const Vector3D m_up;
            Vector3D m_forward;

            Matrix4x4 m_mat;
    };

}

#endif
