#include "stdafx.h"
#include "Enemy.h"



Enemy::Enemy(const Point2f& startPos, float velocity, int currentRoom):
	m_Pos{startPos},
	m_Velocity{velocity},
	m_IsAlive{true},
	m_CurrentRoom{currentRoom}
{
	
	std::cout << "enemy constructor\n";
}
void Enemy::Update(float elapsedSec, const Point2f& playerPos, BulletManager& bm)
{
	
}

void Enemy::Draw() const
{
	
}
Rectf Enemy::GetCollisionRect() const
{
	return Rectf{ m_Pos.x - m_Width / 2, m_Pos.y - m_Height / 2, m_Width, m_Height };
}
void Enemy::IsHit()
{
	if (m_Health > 1)
	{
		--m_Health;
	}
	else
	{
		m_IsAlive = false;
	}
}
bool Enemy::IsAlive() const
{
	return m_IsAlive;
}

Point2f Enemy::GetR1() const
{
	return Point2f{m_Pos.x, m_Pos.y};
}
Point2f Enemy::GetR2() const
{
	return Point2f{ m_Pos.x, m_Pos.y + m_Height};
}
Point2f Enemy::GetR3() const
{
	return Point2f{ m_Pos.x + m_Width, m_Pos.y };
}
void Enemy::SetPos(const Point2f& newPos)
{
	m_Pos = newPos;
}
float Enemy::GetWidth() const
{
	return m_Width;
}
float Enemy::GetHeight() const
{
	return m_Height;
}
Point2f Enemy::GetPos() const
{
	return m_Pos;
}

int Enemy::GetCurrentRoom() const
{
	return m_CurrentRoom;
}