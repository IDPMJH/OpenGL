#pragma once
#include <vector>

using std::vector;
class Polygon;
class Line;

class Game
{
public:
	Game();
	~Game();
	HRESULT Initialize();
	void Run();
	void Update();
	HRESULT Destroy();
	void Render();
	void HandleInput();
	void LoadContent();
	void UnloadContent();
	void Exit();

public:
	vector<class::Polygon*> _polygons;
	Line* _line;
	LARGE_INTEGER	_CurCount;		// 현재 카운트
	LARGE_INTEGER	_PrevCount;		// 이전 카운트
	LARGE_INTEGER	_Frequency;		// 초당 카운트 횟수
	double			_dDT;			// (FPS의 역수=), 1 프레임당 시간	(Delta Time), 프레임 사이의 시간값
	double			_dAcc;			// 1초 체크를 위한 누적시간
	UINT			_CallCount;	// 함수 호출 횟수 
	UINT			_iFPS;			// 초당 함수 호출 횟수
	int				_Score;

};

