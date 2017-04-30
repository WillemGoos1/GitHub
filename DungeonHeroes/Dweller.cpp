#include "stdafx.h"
#include "Dweller.h"
#include "utils.h"

unique_ptr<Texture> Dweller::m_pTexture = nullptr;

Dweller::Dweller(const Point2f& startPos, float velocity, int currentRoom) :
	Enemy(startPos, velocity, currentRoom),
	m_AnimFrameHorizontal{0},
	m_AnimFrameVertical{0},
	m_AnimTime{0.0f},
	m_State{State::IDLE},
	m_StateTimer{0},
	m_Direction{0},
	m_Angle{0}
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = make_unique<Texture>("Resources/DwellerWerewolf.png");
	}

	if (!m_pTexture->IsCreationOk())
	{
		cout << "error initializing dweller texture\n";
	}

	m_Health = 4;
	
}
void Dweller::Update(float elapsedSec, const Point2f& playerPos, BulletManager& bm)
{
	ControlStates(elapsedSec);
	CalculateAnimation(elapsedSec);
}
void Dweller::Draw() const
{
//	m_pTexture->Draw(Rectf{ m_Pos.x, m_Pos.y, m_Width, m_Height }, Rectf{ 0.0f,m_SpriteWidth, m_SpriteWidth, m_SpriteWidth});
	glPushMatrix();
	glTranslatef(m_Pos.x, m_Pos.y,0);
	glRotatef(m_Angle, 0, 0, 1);
	glTranslatef(-m_Width / 2, -m_Height / 2,0);
	switch (m_State)
	{
	case Dweller::IDLE:
		m_pTexture->Draw(Rectf{ 0.0f, 0.0f, m_Width, m_Height }, Rectf{0.0f,m_SpriteWidth, m_SpriteWidth, m_SpriteWidth });
		//cout << "IDLE\n";
		break;
	case Dweller::MOVING:
		m_pTexture->Draw(Rectf{ 0.0f, 0.0f, m_Width, m_Height }, Rectf{ m_Width * m_AnimFrameHorizontal,m_SpriteWidth * m_AnimFrameVertical + m_Width, m_SpriteWidth, m_SpriteWidth });
	//	cout << "MOVING\n";
		break;
	}
//	m_pTexture->Draw(Rectf{ m_Pos.x, m_Pos.y, m_Width, m_Height }, Rectf{ m_Width * m_AnimFrameHorizontal,m_SpriteWidth * m_AnimFrameVertical + m_Width, m_SpriteWidth, m_SpriteWidth });
	
	glPopMatrix();

	//utils::DrawRect(GetCollisionRect());

	/*utils::DrawLine(GetR1(), GetR2());
	utils::DrawLine(GetR1(), GetR3());*/

}
Rectf Dweller::GetCollisionRect() const
{
	return Rectf{( m_Pos.x + 2 * m_Width / 8) - m_Width / 2, (m_Pos.y +  2 * m_Height / 8) - m_Height / 2, m_Width - 4 * m_Width / 8, m_Height - 4 * m_Height / 8 };
}

void Dweller::CalculateAnimation(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	
	if (m_AnimTime >= (1.0f / 4))
	{
		m_AnimTime = 0;
		if (m_AnimFrameHorizontal == m_NrOfFramesHorizontal && m_AnimFrameVertical != m_NrOfFramesVertical)
		{
			m_AnimFrameHorizontal = 0;
			++m_AnimFrameVertical;
		}
		else if (m_AnimFrameHorizontal == m_NrOfFramesHorizontal && m_AnimFrameVertical == m_NrOfFramesVertical)
		{
			m_AnimFrameHorizontal = 0;
			m_AnimFrameVertical = 0;
		}
		++m_AnimFrameHorizontal;
	}

	//cout << "Horizontal: " << m_AnimFrameHorizontal << " Vertical: " << m_AnimFrameVertical << "\n";

}
void Dweller::ControlStates(float elapsedSec)
{
	
	if (m_State != State::MOVING)
	{
		
		int r = rand() % 50;
		if (r == 1)
		{
			m_StateTimer = float(rand() % 3);
			m_State = State::MOVING;
			m_Direction = rand() % 4;
		}
		
	}
	else if (m_State == State::MOVING)
	{
		m_StateTimer -= elapsedSec;
		//cout << m_StateTimer << "\n";

		if (m_StateTimer <= 0.0f)
		{
			m_State = State::IDLE;
		}

		switch (m_Direction)
		{
		case 0:
			//cout << "right\n";
			m_Pos.x += m_Velocity * elapsedSec;
			m_Angle = 90.0f;
			break;

		case 1:
			//cout << "up\n";
			m_Pos.y += m_Velocity * elapsedSec;
			m_Angle = 180.0f;
			
			break;

		case 2:
			m_Pos.x -= m_Velocity * elapsedSec;
			//cout << "left\n";
			m_Angle = 270.0f;
			break;

		case 3:
			//cout << "Down\n";
			m_Pos.y -= m_Velocity * elapsedSec;
			m_Angle = 0.0f;
			break;

		}
	}
	

}

Point2f Dweller::GetR1() const
{
	//down left
	return Point2f{ (m_Pos.x + 2 * m_Width / 8) - m_Width / 2, (m_Pos.y + 2 * m_Height / 8) - m_Height / 2 };
}
Point2f Dweller::GetR2() const
{
	//top left
	return Point2f{ (m_Pos.x + 2 * m_Width / 8) - m_Width / 2, (m_Pos.y + 2 * m_Height / 8) - m_Height / 2 + (4 * m_Height / 8)};
}
Point2f Dweller::GetR3() const
{
	//down right
	return Point2f{ (m_Pos.x + 2 * m_Width / 8) - m_Width / 2 + (4 * m_Width / 8), (m_Pos.y + 2 * m_Height / 8) - m_Height / 2 };
}
float Dweller::GetWidth() const
{
	return m_Width - 4 * m_Width / 8;
}
float Dweller::GetHeight() const
{
	return m_Height - 4 * m_Height / 8;
}

