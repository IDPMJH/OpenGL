#include "Polygon_12.h"

Polygon::Polygon()
	:_type(mode_Default)
	,_move(move_Default)
	,_clicked(false)
	,_can_click(true)
{
}

Polygon::Polygon(polygon_type pm)
	:_type(pm)
	, _move(move_Default)
	,_clicked(false)
	,_can_click(true)
{
}

Polygon::~Polygon()
{
}
