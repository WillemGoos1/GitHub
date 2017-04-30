#include "stdafx.h"
#include "Turret.h"
#include "utils.h"

unique_ptr<Texture> Turret::m_pTexture = nullptr;

Turret::Turret(const Point2f& startPos, float velocity, int currentRoom):
	Enemy( startPos, velocity, currentRoom),
	m_Angle{}
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = make_unique<Texture>("Resources/turretsSprites.png");
	}

	if (!m_pTexture->IsCreationOk())
	{
		cout << "error initializing turret texture\n";
	}
	int r1{ rand() % 8 };
	int r2{ rand() % 2 + 1 };
	m_Rectf = Rectf{ r1 * m_TextureWidth, r2 * m_TextureHeight, m_TextureWidth, m_TextureHeight };

	m_Health = 5;

}
void Turret::Update(float elapsedSec, const Point2f& playerPos, BulletManager& bm)
{
	m_Angle =  utils::CalcRotation(m_Pos, playerPos) - 90.0f;

	int r = rand() % 50;

	//shooting at player
	if (r == 25 ||  r == 35)
	{
		bm.PushBack(make_shared<Bullet>(m_Pos, m_Angle + 90.0f, Origin::ENEMY, 100.0f));
	}
	//8 way shooting
	/*
	else if (r == 1)
	{
		for (int i{ 0 }; i < 8; ++i)
		{
			bm.PushBack(make_shared<Bullet>(m_Pos, 0.0f + (45.0f*i)));
		}
	}
	*/
}
void Turret::Draw() const
{
	glPushMatrix();

	glTranslatef(m_Pos.x, m_Pos.y, 0);
	glRotatef(m_Angle, 0, 0, 1);
	glTranslatef(-m_Width / 2, -m_Height / 2, 0);
	m_pTexture->Draw(Rectf{ 0.0f, 0.0f, m_Width, m_Height }, m_Rectf);
	glPopMatrix();
	//m_Texture->Draw(Rectf{m_Pos.x, m_Pos.y, m_Width, m_Height}, m_Rectf);
	
}
Rectf Turret::GetCollisionRect() const
{
	//cout << "turret rect opgevraagd!\n";
	return Rectf{ m_Pos.x - m_Width / 2 +   2 * m_Width / 8, m_Pos.y - m_Height / 2 + 2 * m_Height / 8, m_Width -  m_Width / 2, m_Height -  m_Height / 2 };
}
