#pragma once
#include "Bullet.h"
#include <memory>


class BulletManager
{
public:
	BulletManager();
	void PushBack(shared_ptr<Bullet> b);
	void Update(float elapsedSec);
	void Draw() const;
	std::vector<shared_ptr<Bullet>>& GetBullets();
	void RemoveBullet(int index);
private:
	std::vector<shared_ptr<Bullet>> m_Bullets;

};

