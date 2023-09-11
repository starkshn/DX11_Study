#include "pch.h"
#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init(HWND hWnd)
{
	_hWnd			= hWnd;
	_graphcis		= make_shared<Graphics>(hWnd);
	_pipeline		= make_shared<Pipeline>(_graphcis->GetDeviceContext());

	// GO
	_gameObject = make_shared<GameObject>(_graphcis->GetDevice(), _graphcis->GetDeviceContext(), true);
}

void Game::Update()
{
	_gameObject->Update();
}

void Game::Render()
{
	_graphcis->RenderBegin();

	{
		_gameObject->Render(pipe);
	}

	_graphcis->RenderEnd();
}
