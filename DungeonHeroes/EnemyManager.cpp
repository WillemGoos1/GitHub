#include "stdafx.h"
#include "EnemyManager.h"
#include "utils.h"

EnemyManager::EnemyManager()
{
}
void EnemyManager::PushBack(const Enemy& enemy)
{
	m_Enemies.push_back(make_unique<Enemy>(enemy));
	std::cout << "enemy pushed back\n";
	std::cout << m_Enemies.size() << std::endl;
}
void EnemyManager::PushBack(const Turret& turret)
{
	m_Enemies.push_back(make_unique<Turret>(turret));
}
void EnemyManager::PushBack(const Dweller& dweller)
{
	m_Enemies.push_back(make_unique<Dweller>(dweller));
}
void EnemyManager::Update(float elapsedSec, const Point2f& playerPos, int currentPlayerRoom)
{
	
	for (size_t i{ 0 }; i < m_Enemies.size(); ++i)
	{
		
		if (m_Enemies[i]->IsAlive() && currentPlayerRoom == m_Enemies[i]->GetCurrentRoom())
		{
			m_Enemies[i]->Update(elapsedSec, playerPos, m_BulletManager);
		}
		else if(!m_Enemies[i]->IsAlive())
		{
			Remove(i);
		}
		
	}
	m_BulletManager.Update(elapsedSec);
}
void EnemyManager::Draw() const
{
	for (const shared_ptr<Enemy>& e : m_Enemies)
	{	
		e->Draw();
	}
	m_BulletManager.Draw();
}
std::vector<shared_ptr<Enemy>>& EnemyManager::GetEnemies() 
{
	return m_Enemies;
}
BulletManager& EnemyManager::GetBulletManager() 
{
	return m_BulletManager;
}
void EnemyManager::Remove(int index)
{
	m_Enemies.erase(m_Enemies.begin() + index);
}