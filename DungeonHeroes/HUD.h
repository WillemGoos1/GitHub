#pragma once
#include "Texture.h"
#include <memory>

using namespace std;

class HUD
{
public:
	HUD(const Point2f& startPos);
	void Update(const int playerHealth, const int totalHealth, const int currentBullets);
	void Draw() const;
private:
	Point2f m_Pos;
	unique_ptr<Texture> m_pHealthTexture;
	unique_ptr<Texture> m_pBulletHUDTexture;
	int m_TotalHealth;
	int m_CurrentHealth;
	int m_CurrentBullets;
	Rectf m_FullHeartRect;
	Rectf m_EmptyHearthRect;
	Rectf m_BulletRect;
	const float m_PaddingHearts = 48.0f;
	const float m_PaddingBulletsVert = 48.0f;
	const float m_PaddingBulletsHorz = 16.0f;

};

