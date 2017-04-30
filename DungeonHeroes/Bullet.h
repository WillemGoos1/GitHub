#pragma once
#include <memory>
#include "Texture.h"

using namespace std;

enum Origin
{
	PLAYER = 0,
	ENEMY = 1
};

class Bullet
{
public:
	Bullet(const Point2f& startPos, float angle, const Origin& o, float speed=20.0f);
	void Update(float elapsedSec);
	void Draw() const;
	Point2f GetR1() const;
	Point2f GetR2() const;
	Point2f GetR3() const;
	Origin GetOrigin() const;
	Rectf GetCollisionRect() const;
private:
	float m_Angle;
	Point2f m_Pos;
	static unique_ptr<Texture> m_Texture;
	static unique_ptr<Texture> m_AlternateBullet;
	float m_Speed;
	const float m_Width = 16.0f;
	const float m_Height = 16.0f;
	Origin m_Origin;
	bool m_IsUsed;
};

