#ifndef LIMP_ANIMATION_HPP
#define LIMP_ANIMATION_HPP

#include <list>
#include <functional>

#include <iostream>

#include "limpRelation.hpp"

namespace Limp
{

template< typename TIndex = float , typename TValue = float >
class Animation
{
public:
    Animation():
        m_pRelation( new DirectRelation<TIndex,TValue>() ),
        m_pValue( NULL ),
        m_pIndex( NULL ),
        m_speed( 0.f )
    {}


    void start()
    {

    }

    void restart()
    {

    }

    void pause()
    {

    }

    void update( float dTime )
    {
        if( m_pIndex != NULL && m_pValue != NULL )
            m_pRelation->update( *m_pIndex , *m_pValue );
    }

private:
    AbstractRelation<TValue,TIndex>* m_pRelation;

    TValue*  m_pValue;
    TIndex*  m_pIndex;

    float m_speed;
};


/*
using std::list;
using std::cout;
using std::endl;
using std::function;

template<typename TObject , typename TIndex >
class Animation
{
public:

    class RelationBase
    {
    public:
        virtual void update(TObject& object , const TIndex& index) = 0;
    };

    template< typename TValue , typename TIndexParam >
    class DirectRelation : public RelationBase
    {
        typedef function<TValue(TIndexParam&)> AnimationFunction;

    public:
        DirectRelation( TValue TObject::*value ,
                    TIndexParam  TIndex::*index,
                    AnimationFunction func):
            m_value(value),
            m_index(index),
            m_function(func)
        {}


        virtual void update(TObject& object , const TIndex& index)
        {
            object.*m_value = m_function(index.*m_index);
        }

    private:
        TValue      TObject::*m_value;
        TIndexParam  TIndex::*m_index;
        AnimationFunction     m_function;
    };

    template< typename TValue>
    class ComplexRelation : public RelationBase
    {
        typedef function<TValue(const TIndex&)> AnimationFunction;

    public:
        ComplexRelation( TValue TObject::*value ,
                    AnimationFunction func):
            m_value(value),
            m_function(func)
        {}


        virtual void update(TObject& object , const TIndex& index)
        {
            object.*m_value = 10;//m_function(index);
        }

    private:
        TValue      TObject::*m_value;
        AnimationFunction     m_function;
    };

public:

    template< typename TValue , typename TIndexParam >
    void connect( TValue TObject::*value , TIndexParam TIndex::*param , TValue(*func)(TIndexParam))
    {
        connect(value,param,function<TValue(TIndexParam)>(func));
    }

    template< typename TValue , typename TIndexParam >
    void connect( TValue TObject::*value , TIndexParam TIndex::*param , function<TValue(TIndexParam)> func)
    {
        RelationBase* p = new DirectRelation<TValue,TIndexParam>(value,param,func);
        m_animationFunctions.push_back( p );
    }

    template< typename TValue >
    void connect( TValue TObject::*value , TValue(*func)(const TIndex&))
    {
        connect(value,function<TValue(const TIndex&)>(func));
    }

    template< typename TValue >
    void connect( TValue TObject::*value , function<TValue(const TIndex&)> func)
    {
        RelationBase* p = new ComplexRelation<TValue>(value,func);
        m_animationFunctions.push_back( p );
    }

    void update(TObject& object , const TIndex& index)
    {
        for( RelationBase* p : m_animationFunctions )
            p->update(object,index);
    }

    ~Animation()
    {
        for( RelationBase* p : m_animationFunctions )
        {
            if( p != NULL )
                delete p;
        }
    }


private:
    list<RelationBase*>       m_animationFunctions;

};
*/
}

#endif // LIMP_ANIMATION_HPP
