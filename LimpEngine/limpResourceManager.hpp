#ifndef LIMP_RESOURCE_MANAGER_HPP
#define LIMP_RESOURCE_MANAGER_HPP

#include <map>
#include <memory>
#include <stack>

namespace Limp
{

using std::map;
using std::shared_ptr;
using std::stack;

template < typename KeyType , typename ValueType >
class ResourceManager
{

public:
    using MapType = map<KeyType,shared_ptr<ValueType>>;
    using StackType = stack<shared_ptr<ValueType>>;
    using PValue = shared_ptr<ValueType>;

    ResourceManager():
        m_resrc( new Resources )
    {}

    ResourceManager( ResourceManager& rm ):
        m_resrc( rm.m_resrc )
    {}

    bool exists(const KeyType& key)
    {
        return (m_resrc->m_mp.find(key) != m_resrc->m_mp.end());
    }

    shared_ptr<ValueType> get(const KeyType& key)
    {
        return m_resrc->m_mp[key];
    }

    ValueType& getRef( const KeyType& key )
    {
        return *(get(key));
    }


    shared_ptr<ValueType> set( const KeyType& key , shared_ptr<ValueType> pValue)
    {
        m_resrc->m_mp[key] = pValue;
        return pValue;
    }

    shared_ptr<ValueType> set( const KeyType& key , ValueType* pValue)
    {
        shared_ptr<ValueType>& pointer = m_resrc->m_mp[key] ;
        pointer = std::shared_ptr<ValueType>(pValue);
        return pointer;
    }

    template<typename T>
    shared_ptr<T> setNew( const KeyType& key )
    {
        return std::dynamic_pointer_cast<T>(set(key,new T));
    }

    shared_ptr<ValueType>& operator[](const KeyType& key)
    {
        return m_resrc->m_mp[key];
    }

    const ValueType* operator[](const KeyType& key) const
    {
        return m_resrc->m_mp[key];
    }

    void erase(const KeyType& key)
    {
        delete get(key);
        m_resrc->m_mp->erase(key);
    }

    shared_ptr<ValueType> push(shared_ptr<ValueType> pValue)
    {
        m_resrc->m_stack.push( pValue );
        return pValue;
    }

    shared_ptr<ValueType> push(ValueType* pValue)
    {
        shared_ptr<ValueType> pointer(pValue);
        m_resrc->m_stack.push(pointer);
        return pointer;
    }

    template<typename T>
    shared_ptr<T> pushNew( const KeyType& key)
    {
        return std::dynamic_pointer_cast<T>(push(key, new T));
    }

    shared_ptr<ValueType> pop()
    {
        return m_resrc->m_stack.pop();
    }

    ~ResourceManager()
    {
    }



private:

    struct Resources
    {
        MapType   m_mp;
        StackType m_stack;
    };

    shared_ptr<Resources> m_resrc;
};

}

#endif //LIMP_RESOURCE_MANAGER_HPP
