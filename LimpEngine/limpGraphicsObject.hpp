#ifndef LIMP_GRAPHICS_OBJECT_HPP
#define LIMP_GRAPHICS_OBJECT_HPP

#include "limpOpenGL.hpp"
#include "limpMath.hpp"
#include "limpMesh.hpp"
#include "limpObject.hpp"
#include "limpOpenGLRenderer.hpp"

#include <memory>
#include <functional>


namespace Limp
{

class GraphicsObject : public AbstractObject
{
public:

    GraphicsObject():
        m_matTrasform()
    {}

    virtual void move( const Vector3D& v )
    {
        m_matTrasform.translate(v);
    }

    virtual void scale( float f )
    {
        m_matTrasform.scale(f);
    }

    virtual void rotate( float angle , const Vector3D& v )
    {
        m_matTrasform.rotate(angle,v);
    }

    virtual void simulate()
    {

    }

    void setMesh( shared_ptr<BaseMesh> pMesh )
    {
        m_mesh = pMesh;
    }

    virtual void render(OpenGLRenderer& renderer,Camera& cam)
    {
        renderer.uniModelView = cam.getMatrix() * m_matTrasform;
        m_mesh->draw();

    }

    virtual ~GraphicsObject(){}

    Matrix4x4& getTransform(){ return m_matTrasform; }
    shared_ptr<BaseMesh> getMesh(){ return m_mesh; }


private:
    shared_ptr<BaseMesh> 		m_mesh;
    Matrix4x4                   m_matTrasform;
};



class ParticleObject : public GraphicsObject
{
public:
    ParticleObject(){}


    void setParticleCount( int n )
    {
        m_positions.resize(n,Vector3D());
    }


    void setTime( float t )
    {
        m_time = t;
    }

    virtual void simulate()
    {
        //int i = 0;
        int sx = (int)sqrt((double)m_positions.size());
        float s = 20;
        float dx = s / sx;

        static std::random_device rd;
        static std::default_random_engine e2(rd());

        static std::uniform_real_distribution<> dist(0.f , 1.f) ;

        for( int ix = 0 ; ix < sx ; ++ix )
            for( int iy = 0 ; iy < sx ; ++iy )
            {
                float x = -s/2. + ix*dx + dist(e2) ;
                float y = -s/2. + iy*dx + dist(e2);

                Vector3D p = Vector3D(  x , sin(m_time)*cos(x/(y*y+1)) + cos(m_time)*x*(y*y+1) , y);
                m_positions[ix + sx*iy] = p;
            }

    }

    virtual void render(OpenGLRenderer& renderer,Camera& cam)
    {
        Matrix4x4 modelCam = cam.getMatrix() * getTransform();
        Matrix4x4 particleMat;
        for( Vector3D& pos : m_positions )
        {
            particleMat.translate(pos);
            renderer.uniModelView = modelCam * particleMat;
            getMesh()->draw();
            particleMat.translate(-pos);
        }


    }

    virtual ~ParticleObject(){}

private:
    std::vector<Vector3D> m_positions;
    float m_time;


};

}

#endif //LIMP_GRAPHICS_OBJECT_HPP
