#include "stdafx.h"
#include "Collision.h"
#include <memory>
#include "utils.h"

Collision::Collision()
{

}
void Collision::Update(float elapsedSec, Player& player, EnemyManager& enemyManager )
{
	//collision between enemy bullets & player
	/*for (shared_ptr<Bullet>& e : bm.GetBullets())
	{
		if (e->GetOrigin() == Origin::ENEMY && utils::IsOverlapping(player.GetCollisionRect(), e->GetCollisionRect()))
		{
			

		}
	}*/

	for (size_t i{ 0 }; i < enemyManager.GetBulletManager().GetBullets().size(); ++i)
	{
		if (utils::IsOverlapping(player.GetCollisionRect(), enemyManager.GetBulletManager().GetBullets()[i]->GetCollisionRect())
			&& player.IsInvicible() == false)
		{
			std::cout << "player hit!\n";
			enemyManager.GetBulletManager().RemoveBullet(i);
			player.IsHit();
		}
	}

	for (size_t j{ 0 }; j < enemyManager.GetEnemies().size(); ++j)
	{
		for (size_t i{ 0 }; i < player.GetBulletManager().GetBullets().size(); ++i)
		{
			if (utils::IsOverlapping(player.GetBulletManager().GetBullets()[i]->GetCollisionRect(), enemyManager.GetEnemies()[j]->GetCollisionRect()))
			{
				player.GetBulletManager().RemoveBullet(i);
				enemyManager.GetEnemies()[j]->IsHit();
				cout << "enemy hit\n";
			}
		}
	}
	
}

