#pragma once
#include <GL/freeglut.h>
#include "DrawableObject.h"

enum LineType{NORMAL, CLOSED};

class Line : public virtual DrawableObject {
private:
	float* lineVert;
	float* lineColors;
	int howManyLines;
	int type;

public:
	Line();
	Line(int type, float* lineVert, float* lineColors, int howManyLines);

	void draw();

	float* getLineVert();
	void setLineVert(float* LineVert);

	float* getLineColors();
	void setLineColors(float* LineColors);

	int getHowManyLines();
	void setHowManyLines(int howManyLines);
};