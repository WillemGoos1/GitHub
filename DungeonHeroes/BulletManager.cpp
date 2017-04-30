#include "stdafx.h"
#include "BulletManager.h"
#include <iostream>

BulletManager::BulletManager()
{
}
void BulletManager::PushBack(shared_ptr<Bullet> b)
{
	m_Bullets.push_back(shared_ptr<Bullet>(b));
}
void BulletManager::Update(float elapsedSec)
{
	for (shared_ptr<Bullet>& b : m_Bullets)
	{
		b->Update(elapsedSec);
	}
}

void BulletManager::Draw() const
{
	for (const shared_ptr<Bullet>& b : m_Bullets)
	{
		b->Draw();
	}
}
std::vector<shared_ptr<Bullet>>& BulletManager::GetBullets()
{
	return m_Bullets;
}
void BulletManager::RemoveBullet(int index)
{
	//m_Bullets.erase(m_Bullets.begin(), m_Bullets.begin() + index);
	shared_ptr<Bullet>b(m_Bullets[m_Bullets.size() - 1]);
	m_Bullets[m_Bullets.size()-1] = m_Bullets[index];
	m_Bullets[index] = b;
	m_Bullets.pop_back();
	
	//m_Bullets.erase(m_Bullets.begin() + index);
}