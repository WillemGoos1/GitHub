#include "stdafx.h"
#include "Hallway.h"
#include "utils.h"
#include "Texture.h"
float Hallway::m_TextureTileWidth{ 256.0f };
int Hallway::m_InstanceCounter{};
Texture* Hallway::m_pTextureTiles{ nullptr };
Rectf Hallway::m_IsoMetricWall{ 1792.0f, 2816.0f, m_TextureTileWidth , m_TextureTileWidth };
Rectf Hallway::m_HorizontalWallDown{0.0f, 2048.0f, m_TextureTileWidth ,m_TextureTileWidth };
Rectf Hallway::m_HorizontalWallUp{ 512.0f, 2048.0f, m_TextureTileWidth ,m_TextureTileWidth };
Rectf Hallway::m_CornerUp{ 1024.0f, 2816.0f, m_TextureTileWidth ,m_TextureTileWidth };
Rectf Hallway::m_CornerDown{ 0.0f, 2816.0f,  m_TextureTileWidth , m_TextureTileWidth };
Rectf Hallway::m_WallRight{ 768.0f, 2048.0f, m_TextureTileWidth , m_TextureTileWidth };
Rectf Hallway::m_WallLeft{ 256.0f, 2048.0f, m_TextureTileWidth , m_TextureTileWidth };
Rectf Hallway::m_CornerDownRight{512.0f, 2816.0f, m_TextureTileWidth ,m_TextureTileWidth };
Rectf Hallway::m_CornerTopRight{ 1536.0f, 2560.0f, m_TextureTileWidth ,m_TextureTileWidth };

Hallway::Hallway( Doorway& d1,  Doorway& d2) 
{
	++m_InstanceCounter;
	m_DEnter.doorPoint1 = d1.doorPoint1;
	m_DEnter.doorPoint2 = d1.doorPoint2;
	m_DExit.doorPoint1 = d2.doorPoint1;
	m_DExit.doorPoint2 = d2.doorPoint2;

	GenerateVertices();
	if (m_pTextureTiles == nullptr)
	{
		m_pTextureTiles = new Texture{ "Resources/tiles.png" };
	}
	if (!m_pTextureTiles->IsCreationOk())
	{
		std::cout << "failed to initialize tiles in hallway\n";
	}
}
Hallway::~Hallway()
{
	--m_InstanceCounter;
	if (m_InstanceCounter == 0)
	{
		delete m_pTextureTiles;
		m_pTextureTiles = nullptr;
	}
}

void Hallway::GenerateVertices()
{
	float HalfTileWidth = Room::m_TileSize / 2;
	float HalfDeltaRooms{ (m_DEnter.doorPoint1.x - m_DExit.doorPoint1.x) / 2.0f };
	if (m_DEnter.doorPoint1.y > m_DExit.doorPoint1.y)
	{
		//upper hall
		m_Vertices.push_back(m_DExit.doorPoint1);
		m_Vertices.push_back(Point2f{ m_DExit.doorPoint1.x + HalfDeltaRooms - HalfTileWidth, m_DExit.doorPoint1.y });
		m_Vertices.push_back(Point2f{ m_DEnter.doorPoint1.x - HalfDeltaRooms - HalfTileWidth, m_DEnter.doorPoint1.y });
		m_Vertices.push_back(m_DEnter.doorPoint1);

		//lower hall
		m_Vertices.push_back(m_DExit.doorPoint2);
		m_Vertices.push_back(Point2f{ m_DExit.doorPoint2.x + HalfDeltaRooms + HalfTileWidth, m_DExit.doorPoint2.y });
		m_Vertices.push_back(Point2f{ m_DEnter.doorPoint2.x - HalfDeltaRooms + HalfTileWidth, m_DEnter.doorPoint2.y });
		m_Vertices.push_back(m_DEnter.doorPoint2);

		m_HallwayDirection = HallwayDirection::DOWN;
	}
	else if (m_DEnter.doorPoint1.y < m_DExit.doorPoint1.y)
	{
		//upper hall
		m_Vertices.push_back(m_DExit.doorPoint1);
		m_Vertices.push_back(Point2f{ m_DExit.doorPoint1.x + HalfDeltaRooms + HalfTileWidth, m_DExit.doorPoint1.y });
		m_Vertices.push_back(Point2f{ m_DEnter.doorPoint1.x - HalfDeltaRooms + HalfTileWidth, m_DEnter.doorPoint1.y });
		m_Vertices.push_back(m_DEnter.doorPoint1);

		//lower hall
		m_Vertices.push_back(m_DExit.doorPoint2);
		m_Vertices.push_back(Point2f{ m_DExit.doorPoint2.x + HalfDeltaRooms - HalfTileWidth, m_DExit.doorPoint2.y });
		m_Vertices.push_back(Point2f{ m_DEnter.doorPoint2.x - HalfDeltaRooms - HalfTileWidth, m_DEnter.doorPoint2.y });
		m_Vertices.push_back(m_DEnter.doorPoint2);

		m_HallwayDirection = HallwayDirection::UP;
		
	}
	else if (m_DEnter.doorPoint1.y == m_DExit.doorPoint1.y)
	{
		m_Vertices.push_back(m_DExit.doorPoint1);
		m_Vertices.push_back(Point2f{ m_DExit.doorPoint1.x + HalfDeltaRooms - HalfTileWidth, m_DExit.doorPoint1.y });
		m_Vertices.push_back(Point2f{ m_DEnter.doorPoint1.x - HalfDeltaRooms + HalfTileWidth, m_DEnter.doorPoint1.y });
		m_Vertices.push_back(m_DEnter.doorPoint1);

		m_Vertices.push_back(m_DExit.doorPoint2);
		m_Vertices.push_back(Point2f{ m_DExit.doorPoint2.x + HalfDeltaRooms - HalfTileWidth , m_DExit.doorPoint2.y });
		m_Vertices.push_back(Point2f{ m_DEnter.doorPoint2.x - HalfDeltaRooms + HalfTileWidth, m_DEnter.doorPoint2.y });
		m_Vertices.push_back(m_DEnter.doorPoint2);
		
		m_HallwayDirection = HallwayDirection::STRAIGHT;
	}

	

}
void Hallway::Draw() const
{
	/*for (int i{ 0 }; i < 3; ++i)
	{
		utils::DrawLine(m_Vertices[i], m_Vertices[i + 1]);
		utils::DrawLine(m_Vertices[i + 4], m_Vertices[i + 5]);
	}*/
	float TileWidth{ Room::m_TileSize };

	int lengthUpper1{ int(m_Vertices[1].x - m_Vertices[0].x) / int(TileWidth) };
	int lenghtLower1{ int(m_Vertices[5].x - m_Vertices[4].x) / int(TileWidth) };
	int lengthVertical1{ int(m_Vertices[2].y - m_Vertices[1].y) / int(TileWidth) };
	int lengthVertical2{ int(m_Vertices[6].y - m_Vertices[5].y) / int(TileWidth) };
	int lengthUpper2{ int(m_Vertices[3].x - m_Vertices[2].x) / int(TileWidth) };
	int lengthLower2{ int(m_Vertices[7].x - m_Vertices[6].x) / int(TileWidth) };

	//std::cout << lengthVertical2 << std::endl;
	

	for (int i{ 0 }; i < lengthUpper1 - 1; ++i)
	{
		m_pTextureTiles->Draw(Rectf{ m_Vertices[0].x + (TileWidth * i), m_Vertices[0].y, TileWidth, TileWidth }, m_IsoMetricWall);
		m_pTextureTiles->Draw(Rectf{ m_Vertices[0].x + (TileWidth * i), m_Vertices[0].y + TileWidth, TileWidth, TileWidth }, m_HorizontalWallUp);
	}
	for (int i{ 1 }; i < lenghtLower1 - 1; ++i)
	{
		m_pTextureTiles->Draw(Rectf{ m_Vertices[4].x + (TileWidth * i), m_Vertices[4].y - TileWidth, TileWidth, TileWidth }, m_HorizontalWallDown);
	}

	switch (m_HallwayDirection)
	{
	case 1:
		m_pTextureTiles->Draw(Rectf{ m_Vertices[1].x - TileWidth, m_Vertices[1].y + TileWidth, TileWidth, TileWidth }, m_CornerUp);
		m_pTextureTiles->Draw(Rectf{ m_Vertices[1].x - TileWidth, m_Vertices[1].y, TileWidth, TileWidth }, m_IsoMetricWall);
		m_pTextureTiles->Draw(Rectf{ m_Vertices[5].x - TileWidth, m_Vertices[5].y - TileWidth, TileWidth, TileWidth }, m_HorizontalWallDown);
		m_pTextureTiles->Draw(Rectf{ m_Vertices[5].x , m_Vertices[5].y - TileWidth, TileWidth, TileWidth }, m_CornerDown);
		for (int i{ 2 }; i < lengthVertical1 + 1; ++i)
		{
			m_pTextureTiles->Draw(Rectf{ m_Vertices[1].x - TileWidth, m_Vertices[1].y + (i * TileWidth), TileWidth, TileWidth }, m_WallLeft);
		}
		for (int i{ 0 }; i < lengthVertical2 - 1; ++i)
		{
			m_pTextureTiles->Draw(Rectf{ m_Vertices[5].x , m_Vertices[5].y + (i * TileWidth), TileWidth, TileWidth }, m_WallRight);
		}

		m_pTextureTiles->Draw(Rectf{ m_Vertices[6].x , m_Vertices[6].y - TileWidth, TileWidth, TileWidth }, m_CornerDownRight);
		m_pTextureTiles->Draw(Rectf{ m_Vertices[2].x - TileWidth, m_Vertices[2].y + TileWidth , TileWidth, TileWidth }, m_CornerTopRight);
		for (int i{ 0 }; i < lengthUpper2; ++i)
		{
			m_pTextureTiles->Draw(Rectf{ m_Vertices[2].x + (TileWidth * i), m_Vertices[2].y, TileWidth, TileWidth }, m_IsoMetricWall);
			m_pTextureTiles->Draw(Rectf{ m_Vertices[2].x + (TileWidth * i), m_Vertices[2].y + TileWidth, TileWidth, TileWidth }, m_HorizontalWallUp);

		}
		for (int i{ 0 }; i < lengthLower2; ++i)
		{
			m_pTextureTiles->Draw(Rectf{ m_Vertices[6].x + TileWidth + (TileWidth * i), m_Vertices[6].y - TileWidth, TileWidth, TileWidth }, m_HorizontalWallDown);
		}
		break;
	}

}
std::vector<Point2f> Hallway::GetVertices() const
{
	return m_Vertices;
}