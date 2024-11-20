#include <Windows.h>
#include <timeapi.h>
#include "Game.h"
#include "Line.h"
#include "Rect.h"
#include "Triangle.h"


using namespace std;

template <typename T>
T Random_Number(int start, int end);

Game::Game() 
    :_CurCount{}
	, _Frequency{}
	, _PrevCount{}
	, _dDT(0.0)
	, _dAcc(0.0)
	, _CallCount(0)
	, _iFPS(0)
    // Constructor implementation
{
}

Game::~Game() {
    // Destructor implementation
}

HRESULT Game::Initialize()
{
	// Initialize method implementation
	// 프레임 제어
    // 현재 카운트
	QueryPerformanceCounter(&_CurCount);
    // 초당 카운트 횟수 (초당 카운트 차이 값)
	QueryPerformanceFrequency(&_Frequency);

    return S_OK;
}

void Game::Run() 
{
    // Run method implementation
}

float lastTime = (float)timeGetTime();
float timeDelta = 0.0f;

void Game::Update() {

	// Update method implementation
	// 시간 갱신
	// 현재 카운트 값
	QueryPerformanceCounter(&_CurCount);

	// 이전 프레임의 카운팅과 현재 프레임 카운팅 값의 차이를 구한다. 이를 초당 카운트 횟수로 나눈다 -> 프레임카운트당 시간이 나온다
	_dDT = (double)(_CurCount.QuadPart - _PrevCount.QuadPart) / (double)(_Frequency.QuadPart);

	// 이전 프레임카운트 값 현재 프레임카운트값으로 갱신 (다음 계산을 위해)
	_PrevCount = _CurCount;

	for (auto& polygon : _polygons)
	{
		polygon->Update();
	}

	return;
	//	++_CallCount;

	_dAcc += _dDT;	// DT 누적

	if (_dAcc >= 3.) // 1초에 한번(m_dAcc에 누적 된 값이 1초가 넘어가면)
	{
		float r = Random_Number<int>(0, 1);
		float r2 = Random_Number<int>(0, 1);
		if (r == 0)
		{
			// left side
			if (r2 == 0)
			{
				// generate Triangle
				Triangle* tp = new Triangle(-1.f, 0.5f, 0.2f, 0);
				tp->_dirx = dir_right;
				_polygons.push_back(tp);
			}
			else
			{
				// generate Rect
				Rect_p* rp = new Rect_p(-1.f, 0.5f, 0.2f);
				rp->_dirx = 1;
				_polygons.push_back(rp);

			}
		}
		else
		{
			// right side
			if (r2 == 0)
			{
				// generate Triangle
				Triangle* tp = new Triangle(1.f, 0.5f, 0.2f, 0);
				tp->_dirx = dir_left;
				_polygons.push_back(tp);


			}
			else
			{
				// generate Rect
				Rect_p* rp = new Rect_p(1.f, 0.5f, 0.2f);
				rp->_dirx = -1;
				_polygons.push_back(rp);

			}

		}
		_dAcc = 0;
		//	wchar_t szBuffer[255] = {};
		//	swprintf_s(szBuffer, L"FPS : %d, DT : %f", _iFPS, _dDT);
		//	cout << _iFPS << endl;
		//}
	}



}

HRESULT Game::Destroy()
{
    return E_NOTIMPL;
}


void Game::Render() {
    // Render method implementation
    // 프레임률 제어
 //   if (timecount == 0)
 //   {
	//	ctime = timeGetTime();
 //       timecount++;
 //   }

 //   // 현재 시간과 처음 받아온 시간 비교
 //   // 60프레임보다 크면 렌더, 타임을 다시 받아옴
	//if (ctime + (1000 / 90) <= timeGetTime())
	//{
	//	// 렌더링
 //      FrameCount++;
 //      glutPostRedisplay();
 //      timecount = 0;
	//}
	// 
	// 호출 횟수 증가

}

void Game::HandleInput() {
    // HandleInput method implementation
}

void Game::LoadContent() {
    // LoadContent method implementation
}

void Game::UnloadContent() {
    // UnloadContent method implementation
}

void Game::Exit() {
    // Exit method implementation
}
