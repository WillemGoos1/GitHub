#include "stdafx.h"
#include "Player.h"
#include "utils.h"


Player::Player(const Point2f& startPos, float windowHeight) :
	m_Pos{ startPos },
	m_Velocity{ 0.0f,0.0f },
	m_Texture{ "Resources/player.png" },
	m_PlayerSprite{ 0.0f, 24.0f, 32.0f, 24.0f },
	m_Angle{ 0.0f },
	m_WindowHeight{ windowHeight },
	m_Timer{ 0 },
	m_AnimTime{ 0 },
	m_AnimFrame{ 0 },
	m_State{ State::IDLE },
	m_SpritesIdle{ make_unique<Texture>("Resources/RifleIdle.png") },
	m_SpritesMoving{ make_unique<Texture>("Resources/RifleMove.png") },
	m_SpritesReloading{ make_unique<Texture>("Resources/RifleReloading.png") },
	m_IsInvicible{ false },
	m_InvincibleTimer{0},
	m_CurrentHealth{5},
	m_TotalHealth{5},
	m_CurrentBullets{10}
{
		m_Bounds = Rectf{ startPos.x, startPos.y, m_Width, m_Height };
		
}


Player::~Player()
{
}


void Player::Draw() const
{
	
	glPushMatrix();
	glTranslatef(m_Bounds.left + m_Bounds.width / 2, m_Bounds.bottom + m_Bounds.height / 2, 0);
	glRotatef(m_Angle,0, 0, 1);
	glTranslatef(-m_Width / 2, -m_Height / 2, 0);
	float width = 40.0f;
	float height = 32.0f;
	switch (m_State)
	{
	case IDLE:
		m_SpritesIdle->Draw(Rectf{ 0.0f,0.0f,width, height}, Rectf{ 0.0f,( m_ClipHeight+1) * (m_AnimFrame + 1), m_ClipWidth, m_ClipHeight+1 });
		break;
	case MOVING:
		m_SpritesMoving->Draw(Rectf{ 0.0f,0.0f,width,height}, Rectf{ 0.0f, m_ClipHeight * (m_AnimFrame + 1), m_ClipWidth, m_ClipHeight });
		break;
	case RELOADING:
		m_SpritesReloading->Draw(Rectf{ 0.0f, 0.0f, width, height}, Rectf{ 0.0f, 217.0f * (m_AnimFrame + 1), 322.0f, 217.0f });
		break;
	}
	
	glPopMatrix();
	//utils::DrawRect(Rectf{ m_Bounds.left + m_Width / 8, m_Bounds.bottom + m_Height / 8, m_Bounds.width - m_Width / 8, m_Bounds.height - m_Height / 8 });

	m_BulletManager.Draw();
	
}

void Player::Update(float elapsedSec, const Point2f& mousePos,  const Point2f& cameraPos)
{
	
	m_Angle = utils::CalcRotation(Point2f{ (m_Bounds.left - cameraPos.x +( m_Width / 2)) * 3,( m_Bounds.bottom - cameraPos.y + m_Height / 2) * 3 }, mousePos);
	
	//debug
	/*std::cout << "Angle: " << m_Angle << "\n";
	std::cout << "MousePos x : " << mousePos.x << " ";
	std::cout << "MousePos y : " << mousePos.y << " \n";
	std::cout << "PlayerPos x: " << ((m_Bounds.left - cameraPos.x) + (m_Width / 2)) * 3 << " ";
	std::cout << "PlayerPos y: " << (m_Bounds.bottom - cameraPos.y + m_Height / 2) * 3 << " ";*/


	 Controls(elapsedSec, m_BulletManager);
	 CalculateAnimation(elapsedSec);
	 MovementHandling(elapsedSec);
	 StateHandling(elapsedSec);

	 m_BulletManager.Update(elapsedSec);
	

}
void Player::SetPos(const Point2f& pos)
{
	m_Bounds.left = pos.x;
	m_Bounds.bottom = pos.y;
}
Point2f Player::GetPos() const
{
	return Point2f{ m_Bounds.left, m_Bounds.bottom };
}

float Player::GetHeight() const 
{
	return m_Bounds.height;
}
float Player::GetWidth() const
{
	return m_Bounds.width;
}
void Player::Controls(float elapsedSec,  BulletManager& bulletManager)
{
	int x{}, y{};
	const Uint8* pKeyPressed = SDL_GetKeyboardState(nullptr);
	Uint8 mousePos = SDL_GetMouseState(&x, &y);
	m_CoolDownShooting -= elapsedSec;

	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		if (m_CoolDownShooting <= 0 && m_State != State::RELOADING && m_CurrentBullets != 0)
		{
			//bulletManager.PushBack(make_shared<Bullet>(Point2f{ m_Bounds.left, m_Bounds.bottom }, m_Angle, Origin::PLAYER));
			m_BulletManager.PushBack(make_shared<Bullet>(Point2f{ m_Bounds.left, m_Bounds.bottom }, m_Angle, Origin::PLAYER,200.0f));
			--m_CurrentBullets;

			//testing correct bullet firing position
			/*Point2f pos{ m_Bounds.left + m_Bounds.width  + (m_Width / 2 * cosf(m_Angle * float(M_PI) / 180.0f )), m_Bounds.bottom + m_Bounds.height / 2 + (m_Height / 2 * sinf(m_Angle * float(M_PI) / 180.0f)) };
			m_BulletManager.PushBack(make_shared<Bullet>(pos, m_Angle, Origin::PLAYER));*/
			m_CoolDownShooting = 0.35f;
		}
		
		//std::cout << m_Bounds.left << " " << m_Bounds.bottom << "\n";
	}
	

	if (pKeyPressed[SDL_SCANCODE_W])
	{
		//up
		//m_Bounds.bottom += m_Velocity.y * elapsedSec;
		m_Velocity.y = m_VertSpeed;
	}
	if (pKeyPressed[SDL_SCANCODE_S])
	{
		//down
		m_Velocity.y = -m_VertSpeed;
		//m_Bounds.bottom -= m_Velocity.y * elapsedSec;
	}
	if (pKeyPressed[SDL_SCANCODE_A])
	{
		//left
		m_Velocity.x = -m_HorSpeed;
		//m_Bounds.left -= m_Velocity.x * elapsedSec;
	}
	if (pKeyPressed[SDL_SCANCODE_D])
	{
		//right
		m_Velocity.x = m_HorSpeed;
		//m_Bounds.left += m_Velocity.x * elapsedSec;
	}
	if (pKeyPressed[SDL_SCANCODE_R])
	{
		if (m_CurrentBullets != m_TotalBullets)
		{
			m_State = State::RELOADING;
			m_Timer = 1.0f;
			m_AnimFrame = 0;
		}
	}
	
}
Rectf Player::GetShape() const
{
	return m_Bounds;
}
void Player::CalculateAnimation(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_State != State::RELOADING)
	{
		if (m_AnimTime > (1.0f / m_NrOfFramesPerSec))
		{
			m_AnimTime = 0.0f;
			if (m_AnimFrame == m_NrOfFrames)
			{
				m_AnimFrame = 0;
			}

			++m_AnimFrame;
		}
	}
	else
	{
		if (m_AnimTime > (2.0f / m_NrOfFramesPerSec))
		{
			m_AnimTime = 0.0f;
			if (m_AnimFrame == m_NrOfFrames)
			{
				m_AnimFrame = 0;
			}

			++m_AnimFrame;
		}
	}

}
void Player::StateHandling(float elapsedSec)
{
	
	if (m_Velocity.x != 0.0f && m_State != State::RELOADING || m_Velocity.y != 0.0f && m_State != State::RELOADING)
	{
		m_State = State::MOVING;
	}
	else if(m_Velocity.x == 0 && m_Velocity.y == 0 && m_State != State::RELOADING)
	{
		m_State = State::IDLE;
	}
	
	if (m_State == State::RELOADING)
	{

		m_Timer -= elapsedSec;
		//std::cout << m_Timer << std::endl;
		if (m_Timer <= 0)
		{
			m_CurrentBullets = m_TotalBullets;
			m_State = State::IDLE;
		}
		
	}

	if (m_IsInvicible)
	{
		m_InvincibleTimer -= elapsedSec;
		if (m_InvincibleTimer <= 0)
		{
			m_IsInvicible = false;
		}
	}

}
void Player::MovementHandling(float elapsedSec)
{
	m_Bounds.left = m_Bounds.left + m_Velocity.x * elapsedSec;
	m_Bounds.bottom = m_Bounds.bottom + m_Velocity.y * elapsedSec;


	if (m_Velocity.x > 0)
	{
		m_Velocity.x -= 4.0f;
	}

	else if (m_Velocity.x < 0)
	{
		m_Velocity.x += 4.0f;
	}
	if (m_Velocity.y > 0)
	{
		m_Velocity.y -= 4.0f;
	}
	else if (m_Velocity.y < 0)
	{
		m_Velocity.y += 4.0f;
	}
}

Rectf Player::GetCollisionRect() const
{
	return Rectf{ m_Bounds.left + m_Width / 8, m_Bounds.bottom + m_Height / 8, m_Bounds.width - m_Width / 8, m_Bounds.height - m_Height / 8 };
}

void Player::IsHit()
{
	m_IsInvicible = true;
	m_InvincibleTimer = 1;
	--m_CurrentHealth;
}
bool Player::IsInvicible() const
{
	return m_IsInvicible;
}
BulletManager& Player::GetBulletManager() 
{
	return m_BulletManager;
}
int Player::GetCurrentHealth() const
{
	return m_CurrentHealth;
}
int Player::GetTotalHealth() const
{
	return m_TotalHealth;
}
int Player::GetCurrentBullets() const
{
	return m_CurrentBullets;
}