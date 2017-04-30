#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include "BulletManager.h"
class Collision
{
public:
	Collision();
	void Update(float elapsedSec, Player& player, EnemyManager& enemyManager);
};

