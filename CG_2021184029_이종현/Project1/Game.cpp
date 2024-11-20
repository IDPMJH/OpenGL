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
	// ������ ����
    // ���� ī��Ʈ
	QueryPerformanceCounter(&_CurCount);
    // �ʴ� ī��Ʈ Ƚ�� (�ʴ� ī��Ʈ ���� ��)
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
	// �ð� ����
	// ���� ī��Ʈ ��
	QueryPerformanceCounter(&_CurCount);

	// ���� �������� ī���ð� ���� ������ ī���� ���� ���̸� ���Ѵ�. �̸� �ʴ� ī��Ʈ Ƚ���� ������ -> ������ī��Ʈ�� �ð��� ���´�
	_dDT = (double)(_CurCount.QuadPart - _PrevCount.QuadPart) / (double)(_Frequency.QuadPart);

	// ���� ������ī��Ʈ �� ���� ������ī��Ʈ������ ���� (���� ����� ����)
	_PrevCount = _CurCount;

	for (auto& polygon : _polygons)
	{
		polygon->Update();
	}

	return;
	//	++_CallCount;

	_dAcc += _dDT;	// DT ����

	if (_dAcc >= 3.) // 1�ʿ� �ѹ�(m_dAcc�� ���� �� ���� 1�ʰ� �Ѿ��)
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
    // �����ӷ� ����
 //   if (timecount == 0)
 //   {
	//	ctime = timeGetTime();
 //       timecount++;
 //   }

 //   // ���� �ð��� ó�� �޾ƿ� �ð� ��
 //   // 60�����Ӻ��� ũ�� ����, Ÿ���� �ٽ� �޾ƿ�
	//if (ctime + (1000 / 90) <= timeGetTime())
	//{
	//	// ������
 //      FrameCount++;
 //      glutPostRedisplay();
 //      timecount = 0;
	//}
	// 
	// ȣ�� Ƚ�� ����

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
