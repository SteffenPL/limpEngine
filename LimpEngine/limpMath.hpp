#ifndef LIMP_MATH_HPP
#define LIMP_MATH_HPP

#include "SFML/Graphics.hpp"
#include "btBulletDynamicsCommon.h"

#include "luaInterface.hpp"

#include <sstream>
#include <cmath>

namespace Limp
{

    constexpr float pi = M_PI;

    btVector3
    struct Vector2D : public sf::Vector2f
    {
        public:
            Vector2D():
                sf::Vector2f()
            {}

            Vector2D(float x_ , float y_ ):
                sf::Vector2f(x_,y_)
            {}


            Vector2D( const sf::Vector2f& v):
                sf::Vector2f(v)
            {}

            Vector2D( const Vector2D& v):
                sf::Vector2f(v.x,v.y)
            {}


            Vector2D normalize()
            {
                float lenght = x * x + y * y;
                lenght = sqrtf(lenght );
                x /= lenght;
                y /= lenght;

                return *this;
            }

            std::string toString() const
            {
                std::stringstream ss;
                ss << "(" << x << ","
                   << y << ")" << std::endl;
                return ss.str();
            }
    };

    struct Vector3D : public sf::Vector3f
    {
        public:
            Vector3D():
                sf::Vector3f()
            {}

            Vector3D(float x_ , float y_ , float z_ ):
                sf::Vector3f(x_,y_,z_)
            {}


            Vector3D( const sf::Vector3f& v):
                sf::Vector3f(v)
            {}

            Vector3D( const Vector3D& v):
                sf::Vector3f(v.x,v.y,v.z)
            {}

            Vector3D& normalize()
            {
                float lenght = x * x + y * y + z * z;
                lenght = sqrtf(lenght );
                x /= lenght;
                y /= lenght;
                z /= lenght;

                return *this;
            }

            std::string toString() const
            {
                std::stringstream ss;
                ss << "(" << x << ","
                   << y << ","
                   << z << ")" << std::endl;
                return ss.str();
            }
    };

    Vector3D cross( const Vector3D& a , const Vector3D& b )
    {
        return Vector3D( a.y * b.z - a.z * b.y , a.z * b.x - a.x * b.z , a.x * b.y - a.y * b.x );
    }

    float   operator* ( const Vector3D& a , const Vector3D& b )
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    struct Vector4D : public Vector3D
    {
        float w;

        public:
            Vector4D():
                Vector3D()
            {}

            Vector4D(float x_ , float y_ , float z_ , float w_ = 1.f):
                Vector3D(x_,y_,z_),
                w(w_)
            {}


            Vector4D( const Vector3D& v):
                Vector3D(v),
                w(1.0)
            {}

            Vector4D( const Vector4D& v):
                Vector3D(v.x,v.y,v.z),
                w(v.w)
            {}

            Vector4D& normalize()
            {
                float lenght = x * x + y * y + z * z + w * w;
                lenght = sqrtf(lenght );
                x /= lenght;
                y /= lenght;
                z /= lenght;
                w /= lenght;

                return *this;
            }


            std::string toString() const
            {
                std::stringstream ss;
                ss << "(" << x << ","
                   << y << ","
                   << z << ","
                   << w << ")" << std::endl;
                return ss.str();
            }
    };


    struct Matrix4x4;
    inline Matrix4x4 operator *( const Matrix4x4& a ,const Matrix4x4& b);
    inline Vector4D operator *( const Matrix4x4& a ,const Vector4D& b);


    template<int N>
    void calc_transpos( float a[N][N] , float transpose[N][N])
    {
        float tmp1 , tmp2;

        for( int i = 0 ; i < N ; ++i )
        {
            for( int j = i + 1 ; j < N ; ++j )
            {
                tmp1 = a[j][i];
                tmp2 = a[i][j];

                transpose[i][j] = tmp1;
                transpose[j][i] = tmp2;
            }
        }
    }

    template<int N>
    void calc_minor( const float a[N][N] , int i  , int j , float minor[N-1][N-1] )
    {

        int transpos_i = 0;
        int transpos_j = 0;

        for( int m = 0 ; m < N ; ++m )
        {
            if( m == i )
                transpos_i = -1;
            else
                for( int k = 0 ; k < N ; ++k )
                {
                    if( k == j )
                        transpos_j = -1;
                    else
                        minor[ m + transpos_i ][ k + transpos_j ] = a[m][k];
                }
         }
    }

    template<int N>
    float calc_determinate( const float a[N][N] )
    {
        if( N < 1 )
            return 0;

        float det = 0.f;

        float minor[N-1][N-1];

        for( int j = 0 ; j < N ; ++j )
        {
            calc_minor( a , 0 , j , minor );

            float s;
            if( j % 2 == 0 ) s =  1.f;
            else             s = -1.f;
            det += s * a[0][j] * calc_determinate< N-1 >( minor );
        }
        return det;
    }

    template<>
    float calc_determinate<1>( const float a[1][1] )
    {
        return a[0][0];
    }

    template<>
    float calc_determinate<2>( const float a[2][2] )
    {
        return a[0][0] * a[1][1] - a[1][0] * a[0][1];
    }


    template< int N>
    void calc_product( const float a[N][N] , const float b[N][N] , float c[N][N])
    {
        for( int i = 0 ; i < N ; ++i )
            for( int j = 0 ; j < N ; ++j )
            {
                c[i][j] = 0.f;
                for( int m = 0 ; m < N ; ++m )
                    c[i][j] += a[i][m] * b[m][j];
            }
    }



    // Eine Matrix Klasse.
    // Zugriff über die 16 Variablen:
    //
    // m11 m12 m13 m14
    // m21 m22 m24 m24
    // m31 m32 m33 m34
    // m41 m42 m43 m44
    //
    struct Matrix4x4
    {
        union
        {
            float n[16];
            float a[4][4];
            struct
            {
                float   m11, m12, m13, m14,
                        m21, m22, m23, m24,
                        m31, m32, m33, m34,
                        m41, m42, m43, m44;
            };
        };

    public:
        Matrix4x4():
            Matrix4x4(	1.f,0.f,0.f,0.f,
                        0.f,1.f,0.f,0.f,
                        0.f,0.f,1.f,0.f,
                        0.f,0.f,0.f,1.f)  //Id.
        {}

        Matrix4x4(  float _m11, float _m12, float _m13, float _m14,
                    float _m21, float _m22, float _m23, float _m24,
                    float _m31, float _m32, float _m33, float _m34,
                    float _m41, float _m42, float _m43, float _m44) :
                m11(_m11),m12(_m12),m13(_m13),m14(_m14),
                m21(_m21),m22(_m22),m23(_m23),m24(_m24),
                m31(_m31),m32(_m32),m33(_m33),m34(_m34),
                m41(_m41),m42(_m42),m43(_m43),m44(_m44)
        {}

        Matrix4x4( const btTransform& trans )
        {
            trans.getOpenGLMatrix(n);
            transpose();
        }


        const float* getPointer() const
        {
            return (float*)n;
        }

        std::string toString() const
        {
            std::stringstream ss;
            ss << " ( " << m11 << ", " << m12 << ", " << m13 << ", " << m14 << ") \n";
            ss << "(( " << m21 << ", " << m22 << ", " << m23 << ", " << m24 << ")) \n";
            ss << "(( " << m31 << ", " << m32 << ", " << m33 << ", " << m34 << ")) \n";
            ss << " ( " << m41 << ", " << m42 << ", " << m43 << ", " << m44 << ")\n";
            return ss.str();
        }

        operator btTransform() const
        {
            return btTransform( btMatrix3x3(m11,m21,m31,m12,m22,m32,m13,m23,m33) , btVector3(m14,m24,m34) );
        }


        Matrix4x4& loadIdentity()
        {
            m11 = 1.f;        m12 = 0.f;        m13 = 0.f;        m14 = 0.f;
            m21 = 0.f;        m22 = 1.f;        m23 = 0.f;        m24 = 0.f;
            m31 = 0.f;        m32 = 0.f;        m33 = 1.f;        m34 = 0.f;
            m41 = 0.f;        m42 = 0.f;        m43 = 0.f;        m44 = 1.f;
            return *this;
        }

        Matrix4x4& invert()
        {
           m11 = m23*m34*m42 - m24*m33*m42 + m24*m32*m43 - m22*m34*m43 - m23*m32*m44 + m22*m33*m44;
           m12 = m14*m33*m42 - m13*m34*m42 - m14*m32*m43 + m12*m34*m43 + m13*m32*m44 - m12*m33*m44;
           m13 = m13*m24*m42 - m14*m23*m42 + m14*m22*m43 - m12*m24*m43 - m13*m22*m44 + m12*m23*m44;
           m14 = m14*m23*m32 - m13*m24*m32 - m14*m22*m33 + m12*m24*m33 + m13*m22*m34 - m12*m23*m34;
           m21 = m24*m33*m41 - m23*m34*m41 - m24*m31*m43 + m21*m34*m43 + m23*m31*m44 - m21*m33*m44;
           m22 = m13*m34*m41 - m14*m33*m41 + m14*m31*m43 - m11*m34*m43 - m13*m31*m44 + m11*m33*m44;
           m23 = m14*m23*m41 - m13*m24*m41 - m14*m21*m43 + m11*m24*m43 + m13*m21*m44 - m11*m23*m44;
           m24 = m13*m24*m31 - m14*m23*m31 + m14*m21*m33 - m11*m24*m33 - m13*m21*m34 + m11*m23*m34;
           m31 = m22*m34*m41 - m24*m32*m41 + m24*m31*m42 - m21*m34*m42 - m22*m31*m44 + m21*m32*m44;
           m32 = m14*m32*m41 - m12*m34*m41 - m14*m31*m42 + m11*m34*m42 + m12*m31*m44 - m11*m32*m44;
           m33 = m12*m24*m41 - m14*m22*m41 + m14*m21*m42 - m11*m24*m42 - m12*m21*m44 + m11*m22*m44;
           m34 = m14*m22*m31 - m12*m24*m31 - m14*m21*m32 + m11*m24*m32 + m12*m21*m34 - m11*m22*m34;
           m41 = m23*m32*m41 - m22*m33*m41 - m23*m31*m42 + m21*m33*m42 + m22*m31*m43 - m21*m32*m43;
           m42 = m12*m33*m41 - m13*m32*m41 + m13*m31*m42 - m11*m33*m42 - m12*m31*m43 + m11*m32*m43;
           m43 = m13*m22*m41 - m12*m23*m41 - m13*m21*m42 + m11*m23*m42 + m12*m21*m43 - m11*m22*m43;
           m44 = m12*m23*m31 - m13*m22*m31 + m13*m21*m32 - m11*m23*m32 - m12*m21*m33 + m11*m22*m33;
           scale( 1.f/determinant() );
           return *this;
        }

        Matrix4x4 getInverse()
        {
            Matrix4x4 m = *this;
            return m.invert();
        }


        float determinant()
        {
           return
           m14*m23*m32*m41 - m13*m24*m32*m41 - m14*m22*m33*m41 + m12*m24*m33*m41+
           m13*m22*m34*m41 - m12*m23*m34*m41 - m14*m23*m31*m42 + m13*m24*m31*m42+
           m14*m21*m33*m42 - m11*m24*m33*m42 - m13*m21*m34*m42 + m11*m23*m34*m42+
           m14*m22*m31*m43 - m12*m24*m31*m43 - m14*m21*m32*m43 + m11*m24*m32*m43+
           m12*m21*m34*m43 - m11*m22*m34*m43 - m13*m22*m31*m44 + m12*m23*m31*m44+
           m13*m21*m32*m44 - m11*m23*m32*m44 - m12*m21*m33*m44 + m11*m22*m33*m44;
        }

        Matrix4x4& transpose()
        {
            *this = Matrix4x4(  m11 , m21 , m31 , m41 ,
                                m12 , m22 , m32 , m42 ,
                                m13 , m23 , m33 , m43 ,
                                m14 , m24 , m34 , m44 );
            return *this;
        }

        Matrix4x4& translate( const Vector3D& v)
        {
            Matrix4x4 tmp(	1,0,0,v.x,
                            0,1,0,v.y,
                            0,0,1,v.z,
                            0,0,0,1 );
            *this = tmp * (*this);

            return *this;
        }

        Matrix4x4& rotateX ( float alpha )
        {
            Matrix4x4 tmp;

            const float sin_a = sin(alpha);
            const float cos_a = cos(alpha);

            tmp.m22 = cos_a  ;
            tmp.m23 = -sin_a;
            tmp.m32 = sin_a;
            tmp.m33 = cos_a;

            // Die erste Transformation steht rechts
            *this = tmp * (*this);
                    return *this;
        }

        Matrix4x4& rotateY( float alpha )
        {
            Matrix4x4 tmp;

            const float sin_a = sin(alpha);
            const float cos_a = cos(alpha);

            tmp.m11 = cos_a  ;
            tmp.m13 = sin_a;
            tmp.m31 = -sin_a ;
            tmp.m33 = cos_a;

            // Die erste Transformation steht rechts
            *this = tmp * (*this);
            return *this;
        }

        Matrix4x4& rotateZ ( float alpha )
        {
            Matrix4x4 tmp;

            const float sin_a = sin(alpha);
            const float cos_a = cos(alpha);

            tmp.m11 = cos_a  ;
            tmp.m12 = -sin_a;
            tmp.m21 = sin_a ;
            tmp.m22 = cos_a;

            // Die erste Transformation steht rechts
            *this = tmp * (*this);
            return *this;
        }

        Matrix4x4& rotate( const float angle , Vector3D v )
        {
            const float& x = v.x ;
            const float& y = v.y ;
            const float& z = v.z;
            const float& cos_a = cos( angle );
            const float& sin_a = sin( angle );

            Matrix4x4 r( x*x*(1-cos_a) + cos_a , x*y*(1-cos_a) - z* sin_a , x*z*(1-cos_a) + y * sin_a , 0.f,
                         x*y*(1-cos_a) + z * sin_a , y*y*(1-cos_a) + cos_a , y*z*(1-cos_a) - x * sin_a , 0.f,
                         x*z*(1-cos_a) - y * sin_a , y*z*(1-cos_a) + x* sin_a , z*z*(1-cos_a) + cos_a , 0.f,
                         0.f , 0.f , 0.f , 1.f );

            *this = r * (*this);

            return *this;
        }

        Matrix4x4& lookAt( const Vector3D& p , const Vector3D& f , Vector3D up )
        {

            Vector3D s = cross( f , up );
            s.normalize();
            up = cross( s , f );
            up.normalize();

            loadIdentity();

            Matrix4x4 r( 	s.x  , s.y  , s.z  , 0.f,
                            up.x , up.y , up.z , 0.f,
                            -f.x , -f.y , -f.z , 0.f,
                            0.f  , 0.f  , 0.f  , 1.f );

            translate( -p );

            // translate first, then rotate
            *this = r * (*this) ;
            return *this;
        }

        Matrix4x4& rotate( float a , float x , float y , float z )
        {
            rotate( a , Vector3D( x,y,z) );
            return *this;
        }


        Matrix4x4& scale ( float f )
        {
            Matrix4x4 tmp;
            tmp.m11 = f;
            tmp.m22 = f;
            tmp.m33 = f;

            // Die erste Transformation steht rechts
            *this = tmp * (*this);
            return *this;
        }


        Matrix4x4& scale ( float x, float y , float z )
        {
            Matrix4x4 tmp;
            tmp.m11 = x;
            tmp.m22 = y;
            tmp.m33 = z;

            // Die erste Transformation steht rechts
            *this = tmp * (*this);
            return *this;
        }

        Matrix4x4& scale ( const Vector3D& v )
        {
            return scale(v.x,v.y,v.z);
        }

        Matrix4x4& scaleX ( float f )
        {
            Matrix4x4 tmp;
            tmp.m11 = f;

            *this = tmp * (*this);
            return *this;
        }

        Matrix4x4& scaleY ( float f )
        {
            Matrix4x4 tmp;
            tmp.m22 = f;
            *this = tmp * (*this);
            return *this;
        }

        Matrix4x4& scaleZ ( float f )
        {
            Matrix4x4 tmp;
            tmp.m33 = f;
            *this = tmp * (*this);
            return *this;
        }

        Matrix4x4& loadPerspectivProjection_fov( float fov , float aspect , float n , float f )
        {
            const float t =  fabsf( tan(fov * 0.5f) * n );
            const float r = t * aspect;

            loadPerspectivProjection(  r  , - r  , t  , - t  , n , f);
            return *this;
        }

        Matrix4x4& loadOrthogonalProjection_fov( float fov , float aspect , float n , float f )
        {
            const float t =  fabsf( tan(fov * 0.5f) * n );
            const float r = t * aspect;

            loadOrthogonalProjection( r  , - r  , t  , - t  , n , f);
            return *this;
        }

        // Die Matrix aus der Facharbeit...
        Matrix4x4& loadPerspectivProjection( float r , float l, float t  , float b , float n , float f )
        {
            m11 = 2*n / (r - l);    m12 = 0.f;              m13 = (r+l)/(r-l);    m14 = 0.f;
            m21 = 0.f;              m22 = 2*n / (t - b);    m23 = (t+b)/(t-b);    m24 = 0.f;
            m31 = 0.f;              m32 = 0.f;              m33 = (n+f)/(n-f);    m34 = 2*n*f/(n-f);
            m41 = 0.f;              m42 = 0.f;              m43 = -1;             m44 = 0.f;
            return *this;
        }

        // Die Matrix aus der Facharbeit...
        Matrix4x4& loadOrthogonalProjection( float r , float l, float t  , float b , float n , float f )
        {
            m11 = 2 / (r - l);      m12 = 0.f;              m13 = 0.f;          m14 = -(r+l)/(r-l);
            m21 = 0.f;              m22 = 2 / (t - b);      m23 = 0.f;          m24 = -(t+b)/(t-b);
            m31 = 0.f;              m32 = 0.f;              m33 = 2/(n-f);      m34 = (n+f)/(n-f);
            m41 = 0.f;              m42 = 0.f;              m43 = 0;            m44 = 1.f;
            return *this;
        }

        static constexpr const char* Name = "LimpMatrix";

        static void registerClass( LuaState& lua )
        {
            static LuaLib functions[] =
            {
                { "new" , LuaConstructor<Matrix4x4()>::LuaCFunction },
                libEnd
            };

            static LuaLib methods[] =
            {
                { "loadIdentity", LuaMethod( Matrix4x4 , Matrix4x4&()  , loadIdentity ) },
                //{ "toString",     LuaMethod( Matrix4x4 , std::string() , toString) },
                { "__gc" , LuaDestructor< Matrix4x4 >::LuaCFunction },
                libEnd
            };


            LuaClass< Matrix4x4 > luaClass( lua );
            luaClass.registerFunctions(functions);
            luaClass.registerMethods(methods);
        }


    };


    Matrix4x4 operator *( const Matrix4x4& a ,const Matrix4x4& b)
    {
        return Matrix4x4
               (
                   a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41 ,
                   a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42 ,
                   a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43 ,
                   a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44 ,

                   a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41 ,
                   a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42 ,
                   a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43 ,
                   a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44 ,

                   a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41 ,
                   a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42 ,
                   a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43 ,
                   a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44 ,

                   a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41 ,
                   a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42 ,
                   a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43 ,
                   a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44
               );
    }

    Vector4D operator *( const Matrix4x4& a ,const Vector4D& b)
    {
        return Vector4D(
                            a.m11 * b.x + a.m12*b.y + a.m13*b.z + a.m14*b.w ,
                            a.m21 * b.x + a.m22*b.y + a.m23*b.z + a.m24*b.w ,
                            a.m31 * b.x + a.m32*b.y + a.m33*b.z + a.m34*b.w ,
                            a.m41 * b.x + a.m42*b.y + a.m43*b.z + a.m44*b.w
                        );
    }




}
#endif //LIMP_MATH_HPP
