#pragma once
#include "Enemy.h"



class Dweller : public Enemy
{
public:
	Dweller(const Point2f& startPos, float velocity = 0, int currentRoom = 0);
	virtual void Update(float elapsedSec, const Point2f& playerPos, BulletManager& bm) override;
	virtual void Draw() const override;
	virtual Rectf GetCollisionRect() const override;
	virtual Point2f GetR1() const override;
	virtual Point2f GetR2() const override;
	virtual Point2f GetR3() const override;
	virtual float GetWidth() const override;
	virtual float GetHeight() const override;

private:
	//dimensions
	const float m_Width = 48.0f;
	const float m_Height = 48.0f;
	const float m_SpriteWidth = 48.0f;

	//texture
	static unique_ptr<Texture> m_pTexture;
	
	//animation
	void CalculateAnimation(float elapsedSec);
	
	const int m_NrOfFramesHorizontal = 3;
	const int m_NrOfFramesVertical = 1;

	float m_AnimTime;
	float m_AnimFrameHorizontal;
	float m_AnimFrameVertical;

	//state
	enum State
	{
		IDLE =0,
		MOVING = 1

	};

	State m_State;
	void ControlStates(float elapsedSec);

	int m_Direction;
	float m_StateTimer;

	float m_Angle;
};