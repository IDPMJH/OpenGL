

void Mouse_Convert_Win_To_OpenGL(float& _xpos, float& _ypos, float _width, float _height)
{
	_xpos = (double)_xpos / (double)(_width / 2) - 1;
	_ypos = (double)_ypos / (double)(_height / 2) - 1;
	_ypos = -_ypos;
}