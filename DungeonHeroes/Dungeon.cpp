#include "stdafx.h"
#include "Dungeon.h"
#include <iostream>
#include "Room.h"


Dungeon::Dungeon(int size) : m_Size{ size }, m_TextureTiles{ "Resources/tiles.png" }, m_FillerTile{1536.0f, 2816.0f, 256.0f, 256.0f},
m_CurrentRoom{0}
{
	GenerateRooms();
	GenerateHallways();
}
Dungeon::~Dungeon()
{
	for (Room*& r : m_Rooms)
	{
		delete r;
		r = nullptr;
	}
	for (Hallway*& h : m_HallWays)
	{
		delete h;
		h = nullptr;
	}
}
void Dungeon::GenerateRooms()
{
	bool failed = false;
	int counter{ 0 };

	while (counter != m_Size)
	{
//		Point2f m_TempCenter{ float(200 + (640 * counter)), float(rand() % 16) * 32 + 96};
		float m_TempWidth{ float(rand() % 6 + 13) };
		float m_TempHeight{ float(rand() % 6 + 11) };
		Point2f m_TempCenter{};
		if (counter == 0)
		{
			m_TempCenter = Point2f{ float(200 + (640 * counter)), 400.0f };
		}
		else
		{
			m_TempCenter = Point2f{ m_Rooms[counter-1]->GetRect().left + m_Rooms[counter-1]->GetRect().width + (m_TempWidth / 2.0f * Room::m_TileSize) + Room::m_TileSize * 3, 400.0f };
		}
		
		
		if (int(m_TempWidth) % 2 == 0)
		{
			++m_TempWidth;
		}
		if (int(m_TempHeight) % 2 == 0)
		{
			++m_TempHeight;
		}

		Rectf m_TempRect{ m_TempCenter.x - (m_TempWidth / 2), m_TempCenter.y - (m_TempHeight / 2), m_TempWidth, m_TempHeight };
		std::cout << "attempted\n";
		for (size_t i{ 0 }; i < m_Rooms.size(); ++i)
		{
			if (Intersects(Rectf{ m_TempCenter.x - ((m_TempWidth * Room::m_TileSize)  / 2), m_TempCenter.y - ((m_TempHeight * Room::m_TileSize) / 2), m_TempWidth * Room::m_TileSize, m_TempHeight * Room::m_TileSize }, m_Rooms[i]->GetRect()))
			{
				failed = true;
				break;
			}
		}
		if (!failed)
		{
			if (counter != 0 && counter != m_Size - 1)
			{
				m_Rooms.push_back(new Room{ m_TempCenter, m_TempWidth, m_TempHeight, RoomType::MIDDLE });
			}
			else if (counter == 0)
			{
				m_Rooms.push_back(new Room{ m_TempCenter, m_TempWidth, m_TempHeight, RoomType::BEGINNING });
				m_PosFirstRoom = m_TempCenter;
			}
			else if (counter == m_Size - 1)
			{
				m_Rooms.push_back(new Room{ m_TempCenter, m_TempWidth, m_TempHeight, RoomType::END });
			}

			++counter;
			std::cout << "added room!\n";
		}
	}
}
void Dungeon::GenerateHallways()
{
	for (size_t i{ 0 }; i < m_Rooms.size() - 1; ++i)
	{
		Doorway dExit = m_Rooms[i]->GetDoorExit();
		Doorway dEnter = m_Rooms[i + 1]->GetDoorEnter();
		m_HallWays.push_back(new Hallway{ dEnter, dExit });

		
	}
}

bool Dungeon::Intersects(const Rectf& r1, const Rectf& r2) const
{
	if (r1.left > r2.left + r2.width || r1.left + r1.width < r2.left || r1.bottom > r2.bottom + r2.height || r1.bottom + r1.height < r2.bottom)
		return false;
	else
		return true;
}
void Dungeon::Sort()
{
	/*for (size_t i{ 0 }; i < m_Rooms.size(); ++i)
	{	
		int minPos = i;
		Chamber r = m_Rooms[i];

		for (size_t j{ i + 1 }; j < m_Rooms.size(); ++j)
		{
			if (m_Rooms[j].boundry.left < r.boundry.left)
			{
				minPos = j;
				r = m_Rooms[j];
			}
		}

		m_Rooms[minPos] = m_Rooms[i];
		m_Rooms[i] = r;
	}*/
	
	std::cout << "sorted\n";
}
void Dungeon::Draw()
{
	DrawBackground();
	for (size_t i{ 0 }; i < m_HallWays.size(); ++i)
	{
		m_HallWays[i]->Draw();
	}
	for (size_t i{ 0 }; i < m_Rooms.size(); ++i)
	{
		m_Rooms[i]->Draw();
	}
	
}
void Dungeon::Update(Player& player,  BulletManager& bm, EnemyManager& em)
{
	DoCollisionRooms(player);
	DoCollisionHallways(player);
	CheckCurrentRoom(player);
	CollisionBullets(bm);
	CollisionBullets(em.GetBulletManager());
	CollisionEnemies(em);
	//std::cout << m_CurrentRoom << std::endl;
}

Point2f Dungeon::GetPosFirstRoom() const
{
	return m_PosFirstRoom;
}

void Dungeon::DoCollisionRooms(Player& player)
{
	Point2f r1{ player.GetPos().x + player.GetWidth() / 2, player.GetPos().y + player.GetHeight() };
	Point2f r2{ player.GetPos().x + player.GetWidth() / 2, player.GetPos().y };
	Point2f r3{ player.GetPos().x + player.GetWidth(), player.GetPos().y + player.GetHeight() / 2 };
	Point2f r4{ player.GetPos().x , player.GetPos().y + player.GetHeight() / 2 };
	Point2f centerPlayer{ player.GetPos().x + player.GetWidth() / 2, player.GetPos().y + player.GetHeight() / 2 };

	for (size_t i{ 0 }; i < m_Rooms.size(); ++i)
	{
		//common collision
		if (m_Rooms[i]->m_RoomType != RoomType::MIDDLE)
		{
			if (utils::Raycast(m_Rooms[i]->GetVertices(), r1, centerPlayer, m_HitInfo))
			{
				player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth() / 2, m_HitInfo.intersectPoint.y - player.GetHeight() });
			}
			else if (utils::Raycast(m_Rooms[i]->GetVertices(), r2, centerPlayer, m_HitInfo))
			{
				player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth() / 2, m_HitInfo.intersectPoint.y });
			}
		}
		switch (m_Rooms[i]->m_RoomType)
		{
		case RoomType::BEGINNING:
			CollisionBeginningRoom(m_Rooms[i], player, r3, centerPlayer, r4);
			break;
		case RoomType::MIDDLE:
			CollisionMiddleRoom(m_Rooms[i], player, r3, centerPlayer, r4, r1, r2);
			break;
		case RoomType::END:
			CollisionEndRoom(m_Rooms[i], player, r3, centerPlayer, r4);
			break;
		}
	}
}
void Dungeon::DoCollisionHallways(Player& player)
{
	Point2f centerPlayer{ player.GetPos().x + player.GetWidth() / 2, player.GetPos().y + player.GetHeight() / 2 };
	Point2f r1{ player.GetPos().x + player.GetWidth() / 2 , player.GetPos().y };
	Point2f r2{ player.GetPos().x + player.GetWidth() / 2 , player.GetPos().y + (player.GetHeight() )};
	Point2f r3{ player.GetPos().x, player.GetPos().y + player.GetHeight() / 2 };
	Point2f r4{ player.GetPos().x + player.GetWidth(), player.GetPos().y + player.GetHeight() / 2 };
	for (size_t i{ 0 }; i < m_HallWays.size(); ++i)
	{
		std::vector<Point2f> temp1;
		std::vector<Point2f> temp2;
		std::vector<Point2f> temp3;
		std::vector<Point2f> temp4;
		std::vector<Point2f> temp5;
		std::vector<Point2f> temp6;

		temp1.push_back(m_HallWays[i]->GetVertices()[0]);
		temp1.push_back(m_HallWays[i]->GetVertices()[1]);

		temp2.push_back(m_HallWays[i]->GetVertices()[1]);
		temp2.push_back(m_HallWays[i]->GetVertices()[2]);

		temp3.push_back(m_HallWays[i]->GetVertices()[2]);
		temp3.push_back(m_HallWays[i]->GetVertices()[3]);

		temp4.push_back(m_HallWays[i]->GetVertices()[4]);
		temp4.push_back(m_HallWays[i]->GetVertices()[5]);

		temp5.push_back(m_HallWays[i]->GetVertices()[5]);
		temp5.push_back(m_HallWays[i]->GetVertices()[6]);

		temp6.push_back(m_HallWays[i]->GetVertices()[6]);
		temp6.push_back(m_HallWays[i]->GetVertices()[7]);

		if (utils::Raycast(temp1, r2, centerPlayer, m_HitInfo) || utils::Raycast(temp3, r2, centerPlayer, m_HitInfo))
		{
			player.SetPos(Point2f{ player.GetPos().x, m_HitInfo.intersectPoint.y - player.GetHeight()});
		}
		else if (utils::Raycast(temp2, r3, centerPlayer, m_HitInfo) && m_HallWays[i]->m_HallwayDirection == HallwayDirection::DOWN)
		{
			player.SetPos(Point2f{ m_HitInfo.intersectPoint.x, player.GetPos().y });
		}
		else if (utils::Raycast(temp5, r4, centerPlayer, m_HitInfo) && m_HallWays[i]->m_HallwayDirection == HallwayDirection::DOWN)
		{
			player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth(), player.GetPos().y });
		}
		else if (utils::Raycast(temp2, r4, centerPlayer, m_HitInfo) && m_HallWays[i]->m_HallwayDirection == HallwayDirection::UP)
		{
			player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth(), player.GetPos().y });
		}
		else if (utils::Raycast(temp5, r3, centerPlayer, m_HitInfo) && m_HallWays[i]->m_HallwayDirection == HallwayDirection::UP)
		{
			player.SetPos(Point2f{ m_HitInfo.intersectPoint.x , player.GetPos().y });
		}
		else if (utils::Raycast(temp4, r1, centerPlayer, m_HitInfo) || utils::Raycast(temp6, r1, centerPlayer, m_HitInfo))
		{
			player.SetPos(Point2f{ player.GetPos().x, m_HitInfo.intersectPoint.y });
		}
	}


}
void Dungeon::CollisionBeginningRoom(const Room* room, Player& player, const Point2f& r1, const Point2f& center, const Point2f& r2)
{
	if (room->m_RoomType == RoomType::BEGINNING)
	{
		
		std::vector<Point2f> temp1;
		std::vector<Point2f> temp2;
		std::vector<Point2f> temp3;
		temp1.push_back(room->GetVertices()[0]);
		temp1.push_back(room->GetVertices()[1]);
		
		temp2.push_back(room->GetVertices()[4]);
		temp2.push_back(room->GetVertices()[5]);

		temp3.push_back(room->GetVertices()[2]);
		temp3.push_back(room->GetVertices()[3]);

		
		
		if (utils::Raycast(temp1, r1, center, m_HitInfo) || utils::Raycast(temp2, r1, center, m_HitInfo))
		{
			player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth(), m_HitInfo.intersectPoint.y - player.GetHeight() / 2 });
		}
		else if (utils::Raycast(temp3, r2, center, m_HitInfo))
		{
			player.SetPos(Point2f{ m_HitInfo.intersectPoint.x, m_HitInfo.intersectPoint.y - player.GetHeight() / 2 });
		}
	}
}
void Dungeon::CollisionMiddleRoom(const Room* room, Player& player, const Point2f& r1, const Point2f& center, const Point2f& r2, const Point2f& r3, const Point2f& r4)
{
	std::vector<Point2f> temp1;
	std::vector<Point2f> temp2;
	std::vector<Point2f> temp3;
	std::vector<Point2f> temp4;
	std::vector<Point2f> temp5;
	std::vector<Point2f> temp6;

	temp1.push_back(room->GetVertices()[0]);
	temp1.push_back(room->GetVertices()[1]);
	
	temp2.push_back(room->GetVertices()[4]);
	temp2.push_back(room->GetVertices()[5]);
	
	temp3.push_back(room->GetVertices()[2]);
	temp3.push_back(room->GetVertices()[3]);
	
	temp4.push_back(room->GetVertices()[6]);
	temp4.push_back(room->GetVertices()[7]);
	
	temp5.push_back(room->GetVertices()[1]);
	temp5.push_back(room->GetVertices()[2]);

	temp6.push_back(room->GetVertices()[5]);
	temp6.push_back(room->GetVertices()[6]);

	if (utils::Raycast(temp1, r2, center, m_HitInfo) || utils::Raycast(temp2, r2, center, m_HitInfo))
	{
		player.SetPos(Point2f{ m_HitInfo.intersectPoint.x, m_HitInfo.intersectPoint.y - player.GetHeight() / 2 });
	}
	else if (utils::Raycast(temp3, r1, center, m_HitInfo) || utils::Raycast(temp4, r1, center, m_HitInfo))
	{
		player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth(), m_HitInfo.intersectPoint.y - player.GetHeight() / 2 });
	}
	else if (utils::Raycast(temp5, r4, center, m_HitInfo))
	{
		player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth() / 2, m_HitInfo.intersectPoint.y });
	}
	else if (utils::Raycast(temp6, r3, center, m_HitInfo))
	{
		player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth() / 2, m_HitInfo.intersectPoint.y - player.GetHeight() });

	}
}
void Dungeon::CollisionEndRoom(const Room* room, Player& player, const Point2f& r1, const Point2f& center, const Point2f& r2)
{
	
	std::vector<Point2f> temp1;
	std::vector<Point2f> temp2;
	std::vector<Point2f> temp3;
	temp1.push_back(room->GetVertices()[0]);
	temp1.push_back(room->GetVertices()[1]);

	temp2.push_back(room->GetVertices()[4]);
	temp2.push_back(room->GetVertices()[5]);

	temp3.push_back(room->GetVertices()[2]);
	temp3.push_back(room->GetVertices()[3]);

	if (utils::Raycast(temp1, r2, center, m_HitInfo) || utils::Raycast(temp2, r2, center, m_HitInfo))
	{
		player.SetPos(Point2f{ m_HitInfo.intersectPoint.x, m_HitInfo.intersectPoint.y - player.GetHeight() / 2 });
	}
	else if (utils::Raycast(temp3, r1, center, m_HitInfo))
	{
		player.SetPos(Point2f{ m_HitInfo.intersectPoint.x - player.GetWidth(), m_HitInfo.intersectPoint.y - player.GetHeight() / 2 });
	}

}

void Dungeon::DrawBackground() const
{
	
			m_TextureTiles.Draw(Rectf{0.0f, -1000.0f, 2000.0f, 2000.0f}, m_FillerTile);
	
}

void Dungeon::CheckCurrentRoom(const Player& player)
{
	for (size_t i{ 0 }; i < m_Rooms.size(); ++i)
	{
		if (player.GetPos().x > m_Rooms[i]->GetRect().left && player.GetPos().x < m_Rooms[i]->GetRect().left + m_Rooms[i]->GetRect().width)
		{
			m_CurrentRoom = i;
		}
	}
}
Rectf Dungeon::GetCurrentRoomBounds() const
{
	return m_Rooms[m_CurrentRoom]->GetRect();
}

void Dungeon::CollisionBullets( BulletManager& bm)
{
	for (size_t i{ 0 }; i < m_Rooms.size(); ++i)
	{
		for (size_t j{0}; j < bm.GetBullets().size(); ++j)
		{
			if (m_Rooms[i]->m_RoomType != RoomType::MIDDLE)
			{
				if (utils::Raycast(m_Rooms[i]->GetVertices(), bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR3(), m_HitInfo))
				{
					cout << "hit\n";
					bm.RemoveBullet(j);
				}
				else if (utils::Raycast(m_Rooms[i]->GetVertices(), bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR2(), m_HitInfo))
				{
					cout << "hit\n";
					bm.RemoveBullet(j);
				}
			}
			else
			{
				std::vector<Point2f> temp1;
				std::vector<Point2f> temp2;
				std::vector<Point2f> temp3;
				std::vector<Point2f> temp4;
				std::vector<Point2f> temp5;
				std::vector<Point2f> temp6;

				temp1.push_back(m_Rooms[i]->GetVertices()[0]);
				temp1.push_back(m_Rooms[i]->GetVertices()[1]);

				temp2.push_back(m_Rooms[i]->GetVertices()[4]);
				temp2.push_back(m_Rooms[i]->GetVertices()[5]);

				temp3.push_back(m_Rooms[i]->GetVertices()[2]);
				temp3.push_back(m_Rooms[i]->GetVertices()[3]);

				temp4.push_back(m_Rooms[i]->GetVertices()[6]);
				temp4.push_back(m_Rooms[i]->GetVertices()[7]);

				temp5.push_back(m_Rooms[i]->GetVertices()[1]);
				temp5.push_back(m_Rooms[i]->GetVertices()[2]);

				temp6.push_back(m_Rooms[i]->GetVertices()[5]);
				temp6.push_back(m_Rooms[i]->GetVertices()[6]);

				if (utils::Raycast(temp5, bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR2(), m_HitInfo) || utils::Raycast(temp6, bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR2(), m_HitInfo))
				{
					bm.RemoveBullet(j);
				}
				else if (utils::Raycast(temp1, bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR3(), m_HitInfo) || utils::Raycast(temp2, bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR3(), m_HitInfo)
					|| utils::Raycast(temp3, bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR3(), m_HitInfo) || utils::Raycast(temp4, bm.GetBullets()[j]->GetR1(), bm.GetBullets()[j]->GetR3(), m_HitInfo))
				{
					bm.RemoveBullet(j);
				}
				
			}
			
		}
	}
}
void Dungeon::CollisionEnemies(EnemyManager& em)
{
	for (size_t i{ 0 }; i < m_Rooms.size(); ++i)
	{
		for (size_t j{ 0 }; j < em.GetEnemies().size(); ++j)
		{
			if (m_Rooms[i]->m_RoomType != RoomType::MIDDLE)
			{
				std::vector<Point2f> temp1;
				std::vector<Point2f> temp2;
				std::vector<Point2f> temp3;
				std::vector<Point2f> temp4;
				std::vector<Point2f> temp5;

				//right
				temp1.push_back(m_Rooms[i]->GetVertices()[0]);
				temp1.push_back(m_Rooms[i]->GetVertices()[1]);

				//right
				temp2.push_back(m_Rooms[i]->GetVertices()[4]);
				temp2.push_back(m_Rooms[i]->GetVertices()[5]);

				//left
				temp3.push_back(m_Rooms[i]->GetVertices()[2]);
				temp3.push_back(m_Rooms[i]->GetVertices()[3]);

				//top
				temp4.push_back(m_Rooms[i]->GetVertices()[1]);
				temp4.push_back(m_Rooms[i]->GetVertices()[2]);

				//bottom
				temp5.push_back(m_Rooms[i]->GetVertices()[3]);
				temp5.push_back(m_Rooms[i]->GetVertices()[4]);


				if (utils::Raycast(temp1, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR3(), m_HitInfo) ||
					utils::Raycast(temp2, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR3(), m_HitInfo))
				{
					em.GetEnemies()[j]->SetPos(Point2f{ m_HitInfo.intersectPoint.x - em.GetEnemies()[j]->GetWidth() / 2 , em.GetEnemies()[j]->GetPos().y });
				}
				else if (utils::Raycast(temp3, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR3(), m_HitInfo))
				{
					
					em.GetEnemies()[j]->SetPos(Point2f{ m_HitInfo.intersectPoint.x + em.GetEnemies()[j]->GetWidth() / 2 , em.GetEnemies()[j]->GetPos().y});
				}
				else if (utils::Raycast(temp4, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR2(), m_HitInfo))
				{
					em.GetEnemies()[j]->SetPos(Point2f{ em.GetEnemies()[j]->GetPos().x, m_HitInfo.intersectPoint.y - em.GetEnemies()[j]->GetHeight() / 2 });

				}
				else if (utils::Raycast(temp5, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR2(), m_HitInfo))
				{
					em.GetEnemies()[j]->SetPos(Point2f{ em.GetEnemies()[j]->GetPos().x, m_HitInfo.intersectPoint.y + em.GetEnemies()[j]->GetHeight() / 2 });
				}

				
			}
			else
			{
				std::vector<Point2f> temp1;
				std::vector<Point2f> temp2;
				std::vector<Point2f> temp3;
				std::vector<Point2f> temp4;
				std::vector<Point2f> temp5;
				std::vector<Point2f> temp6;

				temp1.push_back(m_Rooms[i]->GetVertices()[0]);
				temp1.push_back(m_Rooms[i]->GetVertices()[1]);

				temp2.push_back(m_Rooms[i]->GetVertices()[4]);
				temp2.push_back(m_Rooms[i]->GetVertices()[5]);

				temp3.push_back(m_Rooms[i]->GetVertices()[2]);
				temp3.push_back(m_Rooms[i]->GetVertices()[3]);

				temp4.push_back(m_Rooms[i]->GetVertices()[6]);
				temp4.push_back(m_Rooms[i]->GetVertices()[7]);

				temp5.push_back(m_Rooms[i]->GetVertices()[1]);
				temp5.push_back(m_Rooms[i]->GetVertices()[2]);

				temp6.push_back(m_Rooms[i]->GetVertices()[5]);
				temp6.push_back(m_Rooms[i]->GetVertices()[6]);
				//down
				if (utils::Raycast(temp5, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR2(), m_HitInfo))
				{
					em.GetEnemies()[j]->SetPos(Point2f{ em.GetEnemies()[j]->GetPos().x, m_HitInfo.intersectPoint.y + em.GetEnemies()[j]->GetHeight() / 2 });
				}
				//up
				else if (utils::Raycast(temp6, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR2(), m_HitInfo))
				{
					em.GetEnemies()[j]->SetPos(Point2f{ em.GetEnemies()[j]->GetPos().x, m_HitInfo.intersectPoint.y - em.GetEnemies()[j]->GetHeight() / 2 });
				}
				//right
				else if (utils::Raycast(temp1, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR3(), m_HitInfo) || utils::Raycast(temp2, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR3(), m_HitInfo))
				{
					em.GetEnemies()[j]->SetPos(Point2f{ m_HitInfo.intersectPoint.x + em.GetEnemies()[j]->GetWidth() / 2 , em.GetEnemies()[j]->GetPos().y });
				}
				//left
				else if(utils::Raycast(temp3, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR3(), m_HitInfo) || utils::Raycast(temp4, em.GetEnemies()[j]->GetR1(), em.GetEnemies()[j]->GetR3(), m_HitInfo))
				{
					em.GetEnemies()[j]->SetPos(Point2f{ m_HitInfo.intersectPoint.x - em.GetEnemies()[j]->GetWidth() / 2 , em.GetEnemies()[j]->GetPos().y });
				}
			}
		}
	}
}

int Dungeon::GetCurrentRoom() const
{
	return m_CurrentRoom;
}