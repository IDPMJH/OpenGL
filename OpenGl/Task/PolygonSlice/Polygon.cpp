#include "Polygon.h"

Polygon::Polygon()
	:_type(mode_Default)
	,_move(move_Default)
	, _model	{ mat4(1.f) }
	, _rotate	{ mat4(1.f) }
	, _translate{ mat4(1.f) }
	, _scale	{ mat4(1.f) }
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

