#ifndef LIMP_COLOR_HPP
#define LIMP_COLOR_HPP

namespace Limp
{
	
	using uint8 = unsigned char;
	
	template< typename T >
	struct ColorTraits;

	template<>
	struct ColorTraits<float>
	{		
		static constexpr float	MaxValue = 1.f;
	};	
	
	template<>
	struct ColorTraits<uint8>
	{		
		static constexpr uint8	MaxValue = 255;
	};

	template<typename T , int Channels>
	struct Color;
	
	
	
	template<typename T>
	struct Color<T , 4>
	{
		T r;
		T g;
		T b;
		T a;
		
		static constexpr T MaxValue = ColorTraits<T>::MaxValue;
		
	public:
		Color():
			r( MaxValue ),
			g( MaxValue ),
			b( MaxValue ),
			a( MaxValue )
			{}
			
		template<typename U, int I >
		Color( const Color<U,I>& color ):
			r( (T)color.r ),
			g( (T)color.g ),
			b( (T)color.b ),
			a( (T)color.a )
			{}
			
		
		template<typename U>
		Color( const U _r ,const U _g , const U _b , const U _a )
		{
			setColor( _r , _g , _b , _a );
		}
		
		Color( const sf::Color& color )
		{
			setColor<uint8>( color.r , color.g , color.b , color.a );
		}
		
		template< typename U >
		void setColor( const U _r ,const U _g , const U _b , const U _a )
		{
			r = (T)(_r / ColorTraits<U>::MaxValue) * MaxValue ;
			g = (T)(_g / ColorTraits<U>::MaxValue) * MaxValue ;
			b = (T)(_b / ColorTraits<U>::MaxValue) * MaxValue ;
			a = (T)(_a / ColorTraits<U>::MaxValue) * MaxValue ;
		}
	};
	
	
	template<typename T>
	struct Color<T , 3>
	{
		T r;
		T g;
		T b;		
		static constexpr T a = ColorTraits<T>::MaxValue;
		
		static constexpr T MaxValue = ColorTraits<T>::MaxValue;
		
	public:
		Color(): 
			r( MaxValue ),
			g( MaxValue ),
			b( MaxValue )
			{}
			
		template<typename U , int I>
		Color( const Color<U,I>& color ):
			r( (T)color.r ),
			g( (T)color.g ),
			b( (T)color.b )
			{}
			
		template<typename U>
		Color( const U _r ,const U _g , const U _b , const U _a = 0 )
		{
			setColor( _r , _g , _b );
		}
			
		Color( const sf::Color& color )
		{
			setColor<uint8>( color.r , color.g , color.b );
		}
			
		template< typename U >
		void setColor( const U _r ,const U _g , const U _b , const U _a = 0 )
		{
			r = (T)(_r / ColorTraits<U>::MaxValue) * MaxValue ;
			g = (T)(_g / ColorTraits<U>::MaxValue) * MaxValue ;
			b = (T)(_b / ColorTraits<U>::MaxValue) * MaxValue ;
		}
	};
	
	using Color3f = Color<float,3>;
	using Color4f = Color<float,4>;
	using Color3ui = Color<uint8,3>;
	using Color4ui = Color<uint8,4>;
		
}

#endif //LIMP_COLOR_HPP
