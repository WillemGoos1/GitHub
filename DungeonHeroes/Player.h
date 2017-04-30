#pragma once
#include "Vector2f.h"
#include "Texture.h"
#include <memory>
#include <vector>
#include "BulletManager.h"

enum State
{
	IDLE = 0,
	MOVING = 1,
	SHOOTING = 2,
	RELOADING = 3
};


using namespace std;
class Player
{
public:
	Player(const Point2f& startPos, float windowHeight);
	~Player();
	void Update(float elapsedSec, const Point2f& mousePos, const Point2f& cameraPos);
	void Draw() const;
	void SetPos(const Point2f& pos);
	Point2f GetPos() const;
	float GetHeight() const;
	float GetWidth() const;
	Rectf GetCollisionRect() const;
	void IsHit();
	bool IsInvicible() const;
	Rectf GetShape() const;
	BulletManager& GetBulletManager() ;
	int GetCurrentHealth() const;
	int GetTotalHealth() const;
	int GetCurrentBullets() const;


private:
	void StateHandling(float elapsedSec);
	void MovementHandling(float elapsedSec);
	void Controls(float elapsedSec,  BulletManager& bulletManager);
	void CalculateAnimation(float elapsedSec);

	State m_State;
	Point2f m_Pos;
	Rectf m_Bounds;

	const float m_HorSpeed = 100.0f;
	const float m_VertSpeed = 100.0f;
	/*const float m_Width = 40.0f;
	const float m_Height = 32.0f;*/

	const float m_Width = 32.0f;
	const float m_Height = 24.0f;


	Vector2f m_Velocity;
	Texture m_Texture;
	Rectf m_PlayerSprite;
	float m_Angle;
	float m_WindowHeight;
	

	float m_Timer;
	unique_ptr<Texture> m_SpritesMoving;
	unique_ptr<Texture> m_SpritesIdle;
	unique_ptr<Texture> m_SpritesReloading;
	const float m_ClipHeight = 206.0f;
	const float m_ClipWidth = 313.0f;
	const int m_NrOfFrames = 19;
	const int m_NrOfFramesPerSec = 40;
	float m_AnimTime;
	int m_AnimFrame;
	
	float m_CoolDownShooting;
	
	bool m_IsInvicible;
	float m_InvincibleTimer;
	
	

	//health
	int m_CurrentHealth;
	int m_TotalHealth;

	//bullets
	int m_CurrentBullets;
	const int m_TotalBullets = 10;
	BulletManager m_BulletManager;
};

