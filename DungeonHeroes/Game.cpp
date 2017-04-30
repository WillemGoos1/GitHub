#pragma once
#include "stdafx.h"
#include "Game.h"
#include "utils.h"


Game::Game(const Window& window)
	:m_Window{ window },
	m_Room{ Point2f{200.0f,200.0f},6,6, RoomType::MIDDLE },
	m_Dungeon{ 5 },
	m_Player{ m_Dungeon.GetPosFirstRoom(), m_Window.height },
	m_Camera{ 320.0f, 240.0f },
	m_BulletManager{},
	m_EnemyManager{},
	m_Collision{},
	m_HUD{ Point2f{10.0f, m_Window.height - 50.0f} }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	int currentRoom = 0;
	m_EnemyManager.PushBack(Dweller(Point2f{ m_Dungeon.GetPosFirstRoom().x + 600.0f, m_Dungeon.GetPosFirstRoom().y + 100.0f}, 50.0f,currentRoom+1));
	m_EnemyManager.PushBack(Dweller(Point2f{ m_Dungeon.GetPosFirstRoom().x + 120.0f, m_Dungeon.GetPosFirstRoom().y + 100.0f }, 50.0f, currentRoom));
	m_EnemyManager.PushBack(Dweller(Point2f{ m_Dungeon.GetPosFirstRoom().x + 220.0f, m_Dungeon.GetPosFirstRoom().y + 100.0f }, 50.0f, currentRoom));
	m_EnemyManager.PushBack(Dweller(Point2f{ m_Dungeon.GetPosFirstRoom().x - 120.0f, m_Dungeon.GetPosFirstRoom().y + 100.0f }, 50.0f, currentRoom));

	m_EnemyManager.PushBack(Turret(Point2f{ m_Dungeon.GetPosFirstRoom().x + 128.0f, m_Dungeon.GetPosFirstRoom().y },0.0f, currentRoom));
	m_EnemyManager.PushBack(Turret(Point2f{ m_Dungeon.GetPosFirstRoom().x - 128.0f, m_Dungeon.GetPosFirstRoom().y },0.0f, currentRoom));
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	m_Camera.SetBoundaries(m_Dungeon.GetCurrentRoomBounds());
	m_Player.Update(elapsedSec ,m_MousePos ,Point2f{ m_Camera.GetPosition(m_Player.GetShape()).x,m_Camera.GetPosition(m_Player.GetShape()).y });
	m_Dungeon.Update(m_Player, m_Player.GetBulletManager(), m_EnemyManager);
	
	m_EnemyManager.Update(elapsedSec, m_Player.GetPos(), m_Dungeon.GetCurrentRoom());
	m_Collision.Update(elapsedSec, m_Player, m_EnemyManager);
	m_HUD.Update(m_Player.GetCurrentHealth(), m_Player.GetTotalHealth(), m_Player.GetCurrentBullets());
}

void Game::Draw( )
{
	ClearBackground( );
	glColor3f(1, 1, 1);
	glPushMatrix();
	glScalef(3, 3, 0);
	glTranslatef(-m_Camera.GetPosition(m_Player.GetShape()).x, -m_Camera.GetPosition(m_Player.GetShape()).y, 0);
	m_Dungeon.Draw();
	m_Player.Draw();
	m_BulletManager.Draw();
	m_EnemyManager.Draw();
	
	glPopMatrix();
	
	m_HUD.Draw();
	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	m_MousePos = Point2f{float( e.x), float(m_Window.height - e.y) };
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
