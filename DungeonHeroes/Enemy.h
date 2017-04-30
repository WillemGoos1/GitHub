#pragma once
#include <iostream>
#include <memory>
#include "Texture.h"
#include "BulletManager.h"
class Enemy
{
public:
	Enemy(const Point2f& startPos, float velocity = 0.0f, int currentRoom = 0);
	virtual void Update(float elapsedSec, const Point2f& playerPos, BulletManager& bm);
	virtual void Draw() const;
	virtual Rectf GetCollisionRect() const;
	virtual Point2f GetR1() const;
	virtual Point2f GetR2() const;
	virtual Point2f GetR3() const;
	virtual float GetWidth() const;
	virtual float GetHeight() const;

	//non-virtual functions
	Point2f GetPos() const;
	void SetPos(const Point2f& newPos);
	void IsHit();
	bool IsAlive() const;
	int GetCurrentRoom() const;
private:
	const float m_Width = 64.0f;
	const float m_Height = 64.0f;
protected:
	Point2f m_Pos;
	
	float m_Velocity;
	int m_Health;
	bool m_IsAlive;
	int m_CurrentRoom;
};

