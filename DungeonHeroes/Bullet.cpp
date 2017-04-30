#include "stdafx.h"
#include "Bullet.h"
#include "utils.h"

unique_ptr<Texture> Bullet::m_Texture = nullptr;
unique_ptr<Texture> Bullet::m_AlternateBullet = nullptr;

Bullet::Bullet(const Point2f& startPos, float angle, const Origin& o, float speed) :
	m_Angle{ angle },
	m_Pos{ startPos },
	m_Origin{ o },
	m_IsUsed{ true },
	m_Speed {speed}
{
	//std::cout << startPos.x << " " << startPos.y << "\n";
	if (m_Texture == nullptr)
	{
		m_Texture = make_unique<Texture>("Resources/bullet_level3.png");
	}
	if (m_AlternateBullet == nullptr)
	{
		m_AlternateBullet = make_unique<Texture>("Resources/bullet_level2.png");
	}

}
void Bullet::Update(float elapsedSec)
{
	m_Pos.x = m_Pos.x + cosf(m_Angle * float(M_PI) / 180.0f) * elapsedSec * m_Speed;
	m_Pos.y = m_Pos.y + sinf(m_Angle * float(M_PI) / 180.0f) * elapsedSec * m_Speed;
}
void Bullet::Draw() const
{
	Rectf sourceRect{ m_Pos.x, m_Pos.y , m_Width, m_Height };
	Rectf destinationRect{};
	if (m_IsUsed)
	{
		switch (m_Origin)
		{
		case PLAYER:
			destinationRect = Rectf{ 0.0f, 0.0f, m_Texture->GetWidth(), m_Texture->GetHeight()};
			m_AlternateBullet->Draw(sourceRect, destinationRect);
			break;
		case ENEMY:
			destinationRect = Rectf{ 0.0f, 0.0f, m_AlternateBullet->GetWidth(), m_AlternateBullet->GetHeight() };
			m_Texture->Draw(sourceRect, destinationRect);
			break;
		}
	}
}
Point2f Bullet::GetR1() const
{
	return Point2f{ m_Pos.x , m_Pos.y };
}
Point2f Bullet::GetR2() const
{
	return Point2f{ m_Pos.x, m_Pos.y + m_Height };
}
Point2f Bullet::GetR3() const
{
	return Point2f{ m_Pos.x + m_Width, m_Pos.y };
}
Origin Bullet::GetOrigin() const
{
	return m_Origin;
}
Rectf Bullet::GetCollisionRect() const
{
	return Rectf{ m_Pos.x + 2 * m_Width / 8.0f, m_Pos.y + 2 * m_Height / 8.0f, m_Width -  2 * m_Width / 8.0f, m_Height -2 * m_Height / 8.0f };
}