#include "stdafx.h"
#include "HUD.h"
#include <iostream>


HUD::HUD(const Point2f& startPos):
	m_Pos{startPos},
	m_pHealthTexture{make_unique<Texture>("Resources/hearts.png")},
	m_pBulletHUDTexture{make_unique<Texture>("Resources/bullet_gui.png")},
	m_FullHeartRect{0.0f,0.0f, 224.0f, 232.0f},
	m_EmptyHearthRect{448.0f, 0.0f, 224.0f, 232.0f},
	m_BulletRect{ 0.0f,0.0f,m_pBulletHUDTexture->GetWidth(), m_pBulletHUDTexture->GetHeight() }
{
	if (!m_pHealthTexture->IsCreationOk())
	{
		cout << "initialization of health texture not ok\n";
	}
	if (!m_pBulletHUDTexture->IsCreationOk())
	{
		cout << "initialization of bullet hud texture not ok\n";
	}
}

void HUD::Update(const int playerHealth, const int totalHealth, const int currentBullets)
{
	m_TotalHealth = totalHealth;
	m_CurrentHealth = playerHealth;
	m_CurrentBullets = currentBullets;
}
void HUD::Draw() const
{
	for (int i{ 0 }; i < m_TotalHealth; ++i)
	{
		if (i < m_CurrentHealth)
		{
			m_pHealthTexture->Draw(Rectf{ m_Pos.x + (i * m_PaddingHearts), m_Pos.y, m_PaddingHearts,m_PaddingHearts }, m_FullHeartRect);
		}
		else
		{
			m_pHealthTexture->Draw(Rectf{ m_Pos.x + (i * m_PaddingHearts), m_Pos.y, m_PaddingHearts, m_PaddingHearts }, m_EmptyHearthRect);
		}
	}
	for (int i{ 0 }; i < m_CurrentBullets; ++i)
	{
		m_pBulletHUDTexture->Draw(Rectf{ m_Pos.x + (i * m_PaddingBulletsHorz), m_Pos.y - m_PaddingHearts, m_PaddingBulletsHorz,m_PaddingBulletsVert }, m_BulletRect);
	}
	
}


