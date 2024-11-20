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
	LARGE_INTEGER	_CurCount;		// ���� ī��Ʈ
	LARGE_INTEGER	_PrevCount;		// ���� ī��Ʈ
	LARGE_INTEGER	_Frequency;		// �ʴ� ī��Ʈ Ƚ��
	double			_dDT;			// (FPS�� ����=), 1 �����Ӵ� �ð�	(Delta Time), ������ ������ �ð���
	double			_dAcc;			// 1�� üũ�� ���� �����ð�
	UINT			_CallCount;	// �Լ� ȣ�� Ƚ�� 
	UINT			_iFPS;			// �ʴ� �Լ� ȣ�� Ƚ��
	int				_Score;

};

