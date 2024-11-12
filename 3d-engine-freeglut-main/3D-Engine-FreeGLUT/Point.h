#pragma once
#include <GL/freeglut.h>
#include "DrawableObject.h"

class Point : public virtual DrawableObject {
private:
	float* pointVert;
	float* pointColors;
	int howManyPoints;

public:
	Point();
	Point(float* pointVert, float* pointColors, int howManyPoints);

	void draw();

	float* getPointVert();
	void setPointVert(float* PointVert);

	float* getPointColors();
	void setPointColors(float* PointColors);

	int getHowManyPoints();
	void setHowManyPoints(int howManyPoints);
};