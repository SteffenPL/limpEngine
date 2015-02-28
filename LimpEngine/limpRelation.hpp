#ifndef LIMP_RELATION_HPP
#define LIMP_RELATION_HPP

#include <functional>
#include <map>
#include <cmath>

namespace Limp
{

/** @class AbstractRelation
 *  @brief Relation betwen two types.
 */
template< typename TIndex , typename TValue >
class AbstractRelation
{
public:
    virtual void update( const TIndex& index , TValue& value ) = 0;
};



/** @class DirectRelation
 *  @brief Relation betwen two numeric types
 */
template< typename TIndex , typename TValue >
class DirectRelation : public AbstractRelation<TIndex,TValue>
{
    typedef std::function<TValue(const TIndex&)> RelationFunction;

public:



    DirectRelation( RelationFunction relationFunction = AbstractRelation<TIndex,TValue>::linear ):
        m_relationFunction( relationFunction )
    {}

    void setRelation( RelationFunction relationFunction )
    {
        m_relationFunction = relationFunction;
    }

    virtual void update( const TIndex& index , TValue& value )
    {
        value = m_relationFunction( index );
    }

private:
    RelationFunction m_relationFunction;
};


/** @class KeyframeRelation
 *  @brief Keyframe-Relation betwen two numeric types
 */
template< typename TIndex , typename TValue >
class KeyframeRelation : public AbstractRelation<TIndex,TValue>
{
    typedef std::function<TValue(const TIndex&)> RelationFunction;

public:
    KeyframeRelation( RelationFunction relationFunction = AbstractRelation<TIndex,TValue>::linear ):
        m_relationFunction( relationFunction )
    {}

    void addKeyframe( const TIndex& index , const TValue& value )
    {
        m_keyframes[index] = value;
    }

    void setRelationFunction( RelationFunction relationFunction )
    {
        m_relationFunction = relationFunction;
    }

    virtual void update(const TIndex &index, TValue &value)
    {
        auto it = m_keyframes.begin();

        // find first greater index in the keyframesmap
        while( it != m_keyframes.end() && it->first < index)
            ++it;

        // if all indices are greater (or smaller),
        // take the first (last) keyframe
        if( it == m_keyframes.begin()
                ||  it == m_keyframes.end()  )
            value = it->second;
        else
        {
            auto higherKeyframe = it;
            auto lowerKeyframe = --it;

            const TIndex& higherIndex = higherKeyframe->first;
            const TIndex& lowerIndex = lowerKeyframe->first;

            const TIndex& higherValue = higherKeyframe->second;
            const TIndex& lowerValue = lowerKeyframe->second;

            // the relationFunction is defined betwen (0/0) and (1/1)
            // so the index must be porjected to this range
            // and the result must be scaled to the valuedifference
            value = lowerValue
                    + m_relationFunction( (index - lowerIndex) / (higherIndex - lowerIndex) )
                    * ( higherValue - lowerValue ) ;
        }


    }

private:
    std::map<TIndex,TValue> m_keyframes;
    RelationFunction        m_relationFunction;

};




}

#endif // LIMP_RELATION_HPP
