#pragma once
#include <vector>
#include "Room.h"
#include "Hallway.h"
#include "Player.h"
#include "utils.h"
#include "Texture.h"
#include "BulletManager.h"
#include "EnemyManager.h"
class Dungeon
{
public:
	explicit Dungeon(int size);
	~Dungeon();
	void GenerateRooms();
	void Draw();
	void Update(Player& player,  BulletManager& bm, EnemyManager& em);
	Point2f GetPosFirstRoom() const;
	Rectf GetCurrentRoomBounds() const;
	int GetCurrentRoom() const;

private:
	void Sort();
	void GenerateHallways();
	bool Intersects(const Rectf& r1, const Rectf& r2) const;
	void DoCollisionRooms(Player& player);
	void DoCollisionHallways(Player& player);
	void CollisionBeginningRoom(const Room* room, Player& player, const Point2f& r1, const Point2f& center, const Point2f& r2);
	void CollisionMiddleRoom(const Room* room, Player& player, const Point2f& r1, const Point2f& center, const Point2f& r2, const Point2f& r3, const Point2f& r4);
	void CollisionEndRoom(const Room* room, Player& player, const Point2f& r1, const Point2f& center, const Point2f& r2);
	void DrawBackground() const;
	void CheckCurrentRoom(const Player& player);
	void CollisionBullets( BulletManager& bm);
	void CollisionEnemies(EnemyManager& em);


	int m_Size;
	std::vector<Room*> m_Rooms;
	std::vector<Hallway*> m_HallWays;
	Point2f m_PosFirstRoom;
	utils::HitInfo m_HitInfo;
	Texture m_TextureTiles;
	Rectf m_FillerTile;
	int m_CurrentRoom;
};