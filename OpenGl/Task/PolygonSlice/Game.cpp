#include <Windows.h>
#include <timeapi.h>
#include "Game.h"
#include "global.h"
#include <iostream>

using namespace std;



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

void Game::Run() {
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


	//	++_CallCount;

	//_dAcc += _dDT;	// DT ����

	//if (_dAcc >= 1.) // 1�ʿ� �ѹ�(m_dAcc�� ���� �� ���� 1�ʰ� �Ѿ��)
	//{
	//	_dAcc = 0.;
	//	_iFPS = _CallCount;
	//	_CallCount = 0;

	//	wchar_t szBuffer[255] = {};
	//	swprintf_s(szBuffer, L"FPS : %d, DT : %f", _iFPS, _dDT);
	//	cout << _iFPS << endl;
	//}
    
 

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
