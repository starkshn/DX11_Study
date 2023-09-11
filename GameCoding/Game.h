#pragma once

#include "GameObject.h"
#define gra _graphcis
#define pipe _pipeline

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hWnd);
	void Update();
	void Render();

private:
	HWND					_hWnd;

private:
	shared_ptr<Graphics>	_graphcis;
	shared_ptr<Pipeline>	_pipeline;
	
private:
	shared_ptr<GameObject>	_gameObject;
};
