#include "Polygon_18.h"

Polygon::Polygon()
	:_type(mode_Default)
	,_move(move_Default)
{
}

Polygon::Polygon(polygon_type pm)
	:_type(pm)
	, _move(move_Default)
{
}

Polygon::~Polygon()
{
}

