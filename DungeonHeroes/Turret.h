#pragma once
#include "Enemy.h"
#include "BulletManager.h"

class Turret : public Enemy
{
public:
	Turret(const Point2f& startPos, float velocity = 0.0f, int currentRoom = 0);
	virtual void Update(float elapsedSec, const Point2f& playerPos, BulletManager& bm) override ;
	virtual void Draw() const override;
	virtual Rectf GetCollisionRect() const override;
private:
	Rectf m_Rectf;

	const float m_TextureWidth = 96.0f;
	const float m_TextureHeight = 96.0f;
	const float m_Width = 64.0f;
	const float m_Height = 64.0f;

	float m_Angle;

	static unique_ptr<Texture> m_pTexture;
};

