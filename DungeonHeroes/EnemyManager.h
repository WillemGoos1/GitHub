#pragma once
#include "Enemy.h"
#include <memory>
#include "BulletManager.h"
#include "Turret.h"
#include "Dweller.h"

using namespace std;

class EnemyManager
{
public:
	EnemyManager();
	void PushBack(const Enemy& enemy);
	void PushBack(const Turret& turret);
	void PushBack(const Dweller& dweller);
	void Update(float elapsedSec, const Point2f& playerPos, int currentPlayerRoom);
	void Draw() const;
	std::vector<shared_ptr<Enemy>>& GetEnemies();
	BulletManager& GetBulletManager();
	void Remove(int index);
private:
	std::vector<shared_ptr<Enemy>> m_Enemies;
	BulletManager m_BulletManager;
};

