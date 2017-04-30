#pragma once
#include "Dungeon.h"
#include "Room.h"
#include "Player.h"
#include "Camera.h"
#include "BulletManager.h"
#include "Turret.h"
#include "EnemyManager.h"
#include <memory>
#include <vector>
#include "Collision.h"
#include "HUD.h"
#include "Dweller.h"
class Game
{
public:
	explicit Game( const Window& window );
	Game( const Game& other ) = delete;
	Game& operator=( const Game& other ) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( );

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	Window m_Window;
	Dungeon m_Dungeon;
	Player m_Player;
	Room m_Room;
	Camera m_Camera;
	Point2f m_MousePos;
	BulletManager m_BulletManager;
	
	HUD m_HUD;

	//testing
	EnemyManager m_EnemyManager;
	Collision m_Collision;


	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( );
};