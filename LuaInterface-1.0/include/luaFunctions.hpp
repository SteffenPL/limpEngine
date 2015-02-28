#ifndef LUA_FUNCTIONS_HPP
#define LUA_FUNCTIONS_HPP

#include "lua.hpp"
#include "luaStack.hpp"
#include "luaTypes.hpp"

using LuaCFunction = int (*)(lua_State*);

#define LuaMethod(T , R_Args , Function )  		LuaFunctionHelper<T,R_Args>::LuaCFunction< &T::Function >
#define LuaStaticFunction( R_Args , Function ) 	LuaFunctionHelper<R_Args>::LuaCFunction< Function >

// Static Functions

template< typename T1 , typename T2 = void , typename T3 = void>
struct LuaFunctionHelper;

template< typename R , typename... Args >
struct LuaFunctionHelper< R (Args...) >
{
    using FunctionType = R(*)(Args...);

    template< FunctionType Function >
    static int LuaCFunction( lua_State* pLua )
    {
        LuaStack stack( pLua );
        R result = LuaFunctionHelper<R(Args...) , R()>::call( stack , Function );
        stack.push( result ) ;
        return 1;
    }
};

template< typename... Args >
struct LuaFunctionHelper< void(Args...) >
{
    using FunctionType = void(*)(Args...);

    template< FunctionType Function >
    static int LuaCFunction( lua_State* pLua )
    {
        LuaFunctionHelper<void(Args...) , void()>::call( LuaStack(pLua) , Function );
        return 0;
    }
};


// Member Functions

template< typename T , typename R , typename... Args >
struct LuaFunctionHelper< T , R(Args...) >
{
    using FunctionType = R(T::*)(Args...);

    template< FunctionType Function >
    static int LuaCFunction( lua_State* pLua )
    {
        LuaStack stack( pLua );
        T* self = stack.check<T*>( - (int)sizeof...(Args) - 1 );	// Der erste Parameter

        R result = LuaFunctionHelper<T , R(Args...) , R()>::call( stack , Function , self );
        stack.push( result );
        return 1;
    }
};

template< typename T , typename... Args >
struct LuaFunctionHelper< T , void(Args...) >
{
    using FunctionType = void(T::*)(Args...);

    template< FunctionType Function >
    static int LuaCFunction( lua_State* pLua )
    {
        LuaStack stack( pLua );
        T* self = stack.check<T*>( - (int)sizeof...(Args) - 1 );	// Der erste Parameter

        LuaFunctionHelper< T , void(Args...) , void()>::call( stack , Function , self );
        return 0;
    }
};


// Constructor


template< typename T1 , typename T2 = void>
struct LuaConstructor;

template< class T , typename... Args >
struct LuaConstructor< T(Args...) >
{
    static int LuaCFunction( lua_State* pLua )
    {
        LuaStack stack( pLua );

        T* newData;
        LuaConstructor<T(Args...),T()>::construct( stack , &newData );

        stack.push<T*>( newData );

        //cout << "Constructor: " << (**newData).get() << endl;

        return 1;
    }
};



template< class T >
struct LuaDestructor
{
    static int LuaCFunction( lua_State* pLua )
    {
        LuaStack stack( pLua );
        T* luaData = stack.check<T*>(-1);

        delete luaData;


        return 0;
    }
};



// LuaFunctionHelper::Call


template<typename R , typename... Args , typename... NewArgs >
struct LuaFunctionHelper<R(Args...),R(NewArgs...)>
{
    using FunctionType = R (*)(Args...);

    template< size_t i >
    using ArgType = typename std::tuple_element<i, std::tuple<Args...>>::type;

    static constexpr size_t Pos = sizeof...(NewArgs) + 1;

    static R call( LuaStack& stack  , FunctionType func , NewArgs&... args )
    {
        using NextType = ArgType< sizeof...(Args) - Pos >;
        NextType next = stack.check<NextType>( -(int)Pos );

        return LuaFunctionHelper<R(Args...) , R(NextType , NewArgs... )>::call( stack , func , next , args... );
    }
};

template<typename R , typename... Args>
struct LuaFunctionHelper<R(Args...),R(Args...)>
{
    using FunctionType = R (*)(Args...);

    static R call( LuaStack& stack  , FunctionType func , Args&... args )
    {
        return func(args...);
    }
};



// Memberfunction


template<class T , typename R , typename... Args , typename... NewArgs >
struct LuaFunctionHelper< T , R(Args...) , R(NewArgs...)>
{
    using FunctionType = R (T::*)(Args...);

    template< size_t i >
    using ArgType = typename std::tuple_element<i, std::tuple<Args...>>::type;

    static constexpr size_t Pos = sizeof...(NewArgs) + 1;

    static R call( LuaStack& stack  , FunctionType func , T* self , NewArgs&... args )
    {
        using NextType = ArgType< sizeof...(Args) - Pos >;
        NextType next = stack.check<NextType>( -(int)Pos );

        return LuaFunctionHelper< T , R(Args...) , R(NextType , NewArgs... )>::call( stack , func , self , next , args... );
    }
};

template<class T , typename R , typename... Args>
struct LuaFunctionHelper<T , R(Args...) , R(Args...)>
{
    using FunctionType = R (T::*)(Args...);

    static R call( LuaStack& stack  , FunctionType func , T* self , Args&... args )
    {
        return (self->*func)(args...);	// pSelf.func(args...)
    }
};


// LuaConstructor


template<class T ,typename... Args , typename... NewArgs >
struct LuaConstructor< T(Args...) , T(NewArgs...)>
{

    template< size_t i >
    using ArgType = typename std::tuple_element<i, std::tuple<Args...>>::type;

    static constexpr size_t Pos = sizeof...(NewArgs) + 1;

    static void construct( LuaStack& stack , T** self , NewArgs&... args )
    {
        using NextType = ArgType< sizeof...(Args) - Pos >;
        NextType next = stack.check<NextType>( -(int)Pos );

        LuaConstructor< T(Args...) , T(NextType , NewArgs... )>::construct( stack , self , next , args... );
    }
};

template<class T , typename... Args>
struct LuaConstructor< T(Args...) , T(Args...)>
{

    static void construct( LuaStack& stack , T** self , Args&... args )
    {
        *self = new T(args...);
    }
};

template<class T>
struct LuaSingeltonConstructor
{
    static int LuaCFunction( lua_State* pLua )
    {
        LuaStack stack( pLua );

        T& newData = T::getInstance();
        stack.push<T*>( &newData );

        return 1;
    }
};


#endif //LUA_FUNCTIONS_HPP
