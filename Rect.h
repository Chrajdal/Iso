#pragma once

#include "Vec2.h"

template < typename T >
class Rect
{
public:
	inline	Rect() {}
	inline	Rect( T top,T bottom,T left,T right )
		:
	top( top ),
	bottom( bottom ),
	left( left ),
	right( right )
	{}
	inline	Rect( const Rect& rect )
		:
	top( rect.top ),
	bottom( rect.bottom ),
	left( rect.left ),
	right( rect.right )
	{}
	inline	void Translate( _Vec2< T > d )
	{
		Translate( d.x,d.y );
	}
	inline	void Translate( T dx,T dy )
	{
		top += dy;
		bottom += dy;
		left += dx;
		right += dx;
	}
	template < typename T2 >
	inline	operator Rect< T2 >() const
	{
		return { (T2)top,(T2)bottom,(T2)left,(T2)right };
	}
	inline	void ClipTo( const Rect& rect )
	{
		top = max( top,rect.top );
		bottom = min( bottom,rect.bottom );
		left = max( left,rect.left );
		right = min( right,rect.right );
	}

public:
	T top;
	T bottom;
	T left;
	T right;
};

typedef Rect< int > RectI;
typedef Rect< float > RectF;