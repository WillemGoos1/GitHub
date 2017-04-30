#include "stdafx.h"
#include "Room.h"
#include "utils.h"


float Room::m_HallWayWidth = 32.0f;
float Room::m_TileSize = 32.0f;
int Room::m_InstanceCounter = 0;
float Room::m_TextureTileWidth = 256.0f;
Texture* Room::m_TextureTiles{ nullptr };
Texture* Room::m_TextureObjects{ nullptr };
Rectf Room::m_FloorTile{ 256.0f, 256.0f,m_TextureTileWidth ,m_TextureTileWidth };

Rectf Room::m_DungeonHorizontal1{48.0f, 48.0f, 48.0f, 48.0f};
Rectf Room::m_DungeonHorizontal2{ 384.0f, 480.0f, 48.0f, 48.0f };
Rectf Room::m_DungeonVertical1{0.0f, 432.0f, 48.0f, 48.0f};
Rectf Room::m_DungeonVertical2{ 432.0f, 96.0f, 48.0f, 48.0f};
Rectf Room::m_CornerRightUp{48.0f, 288.0f, 48.0f, 48.0f};
Rectf Room::m_CornerRightDown{48.0f,336.0f, 48.0f, 48.0f};
Rectf Room::m_CornerLeftUp{96.0f, 288.0f, 48.0f, 48.0f};
Rectf Room::m_CornerLeftDown{96.0f, 336.0f, 48.0f, 48.0f};
Rectf Room::m_DoorWayRight{0.0f, 96.0f, 48.0f, 32.0f};
Rectf Room::m_DoorWayLeft{432.0f, 416.0f, 48.0f,32.0f};

Rectf Room::m_HorizontalWall3{ 192.0f, 48.0f, 48.0f, 48.0f };
Rectf Room::m_HorizontalWall4{224.0f, 480.0f, 48.0f, 48.0f};
Rectf Room::m_VerticalWall3{0.0f, 288.0f, 48.0f, 48.0f};
Rectf Room::m_VerticalWall4{432.0f, 240.0f, 48.0f, 48.0f};



Texture* Room::m_Dungeon2{0};

Room::Room(const Point2f& center, float width, float height, RoomType t) :
	m_Center{ center },
	m_Width{ width },
	m_Height{ height },
	m_RoomType{t},
	m_Rectangle{ center.x - (width  * m_TileSize) / 2, center.y - (height * m_TileSize) / 2, width  * m_TileSize, height  * m_TileSize },
	m_LayoutType{rand() % 2}
	
{
	
	CalculateVertices();
	std::cout << m_Rectangle.left << " " << m_Rectangle.bottom << " " << m_Rectangle.width << " " << m_Rectangle.height << "\n";
	if (m_TextureTiles == nullptr)
	{
		m_TextureTiles = new Texture{ "Resources/tiles.png" };
	}
	if (m_TextureObjects == nullptr)
	{
		m_TextureObjects = new Texture{ "Resources/objects.png" };
	}
	if (m_Dungeon2 == nullptr)
	{
		m_Dungeon2 = new Texture{ "Resources/dungeonTiles.png" };
	}
	if (!m_TextureTiles->IsCreationOk())
	{
		std::cout << "error initiating tiles texture\n";
	}
	
	++m_InstanceCounter;
}

Room::~Room()
{
	--m_InstanceCounter;
	if (m_InstanceCounter == 0)
	{
		delete m_TextureTiles;
		m_TextureTiles = nullptr;
		delete m_TextureObjects;
		m_TextureObjects = nullptr;
		delete m_Dungeon2;
		m_Dungeon2 = nullptr;
	}
}

void Room::CalculateVertices()
{
	if (m_RoomType == RoomType::MIDDLE)
	{
		
		//lower room
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y - m_HallWayWidth / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y - (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y - (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y - m_HallWayWidth / 2 });

		//upper room
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y + m_HallWayWidth / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y + (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y + (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y + m_HallWayWidth / 2 });

		m_DoorEnter = Doorway{ m_Vertices[4], m_Vertices[0] };
		m_DoorExit = Doorway{ m_Vertices[7], m_Vertices[3] };
	}
	else if (m_RoomType == RoomType::BEGINNING)
	{
		
		//hallway start top
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y + m_HallWayWidth / 2 });
		//top right
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y + (m_Height * m_TileSize) / 2 });
		
		//top left
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y + (m_Height * m_TileSize) / 2 });
		//down left
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y - (m_Height * m_TileSize) / 2 });
		
		//down right
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y - (m_Height * m_TileSize) / 2 });
		//hallway start bottom
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y - m_HallWayWidth / 2 });
		
		m_DoorExit = Doorway{ m_Vertices[0], m_Vertices[5] };
	}
	else if (m_RoomType == RoomType::END)
	{
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y - m_HallWayWidth / 2 }); //
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y - (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y - (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x + (m_Width * m_TileSize) / 2, m_Center.y + (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y + (m_Height * m_TileSize) / 2 });
		m_Vertices.push_back(Point2f{ m_Center.x - (m_Width * m_TileSize) / 2, m_Center.y + m_HallWayWidth / 2 });
		m_DoorEnter = Doorway{ m_Vertices[5], m_Vertices[0] };
	}
	
}

void Room::Draw()
{	
	DrawWalls();
	//DrawVertices();
	DrawFloor();
//	DrawDecor(1);
}

Rectf Room::GetRect()
{
	return m_Rectangle;
}

Doorway Room::GetDoorEnter()
{
	return m_DoorEnter;
}
Doorway Room::GetDoorExit()
{
	return m_DoorExit;
}

void Room::DrawVertices()
{
	glColor3f(1, 1, 1);
	if (m_RoomType == RoomType::MIDDLE)
	{
		//draw vertices
		for (int i{ 0 }; i < 3; ++i)
		{
			utils::DrawLine(m_Vertices[i], m_Vertices[i + 1]);
			utils::DrawLine(m_Vertices[i + 4], m_Vertices[i + 5]);
		}
	}
	else if (m_RoomType == RoomType::BEGINNING)
	{
		for (size_t i{ 0 }; i < m_Vertices.size() - 1; ++i)
		{
			utils::DrawLine(m_Vertices[i], m_Vertices[i + 1]);
		}
	}
	else if (m_RoomType == RoomType::END)
	{
		for (size_t i{ 0 }; i < m_Vertices.size() - 1; ++i)
		{
			utils::DrawLine(m_Vertices[i], m_Vertices[i + 1]);
		}
	}
}

void Room::DrawWalls()
{
	Rectf HorizontalWall1;
	Rectf HorizontalWall2;
	Rectf VerticalWall1;
	Rectf VerticalWall2;
	Rectf CornerRightUp;
	Rectf CornerRightDown;
	Rectf CornerLeftUp;
	Rectf CornerLeftDown;

	switch (m_LayoutType)
	{
	case 0:
		HorizontalWall1 = m_DungeonHorizontal1;
		HorizontalWall2 = m_DungeonHorizontal2;
		VerticalWall1 = m_DungeonVertical1;
		VerticalWall2 = m_DungeonVertical2;
		CornerRightUp = m_CornerRightUp;
		CornerRightDown = m_CornerRightDown;
		CornerLeftUp = m_CornerLeftUp;
		CornerLeftDown = m_CornerLeftDown;
		break;
	case 1:
		HorizontalWall1 = m_HorizontalWall3;
		HorizontalWall2 = m_HorizontalWall4;
		VerticalWall1 = m_VerticalWall3;
		VerticalWall2 = m_VerticalWall4;
		CornerRightUp = m_CornerRightUp;
		CornerRightDown = m_CornerRightDown;
		CornerLeftUp = m_CornerLeftUp;
		CornerLeftDown = m_CornerLeftDown;
		break;
	}


	//drawing textures horizontal
	for (size_t i{ 0 }; i <(m_Width + 2); ++i)
	{
		if (i != 0 && i != m_Width + 1)
		{
			m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize + (i * m_TileSize), m_Center.y - ((m_Height * m_TileSize) / 2) - m_TileSize, m_TileSize, m_TileSize }, HorizontalWall2);
			m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize + (i * m_TileSize), m_Center.y + ((m_Height * m_TileSize) / 2) , m_TileSize, m_TileSize }, HorizontalWall1);
		}
		else if (i == 0)
		{
			m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * (m_TileSize)) / 2) - m_TileSize + (i * m_TileSize), m_Center.y - ((m_Height * m_TileSize) / 2) - m_TileSize , m_TileSize, m_TileSize }, CornerRightDown);
			m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize + (i * m_TileSize), m_Center.y + ((m_Height * m_TileSize) / 2) , m_TileSize, m_TileSize }, CornerRightUp);
		}
		else
		{
			m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize + (i * m_TileSize), m_Center.y - ((m_Height * m_TileSize) / 2) - m_TileSize, m_TileSize, m_TileSize }, CornerLeftDown);
			m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize + (i * m_TileSize), m_Center.y + ((m_Height * m_TileSize) / 2), m_TileSize, m_TileSize }, CornerLeftUp);
		}
	}

	//drawing texture verical

	for (int i{ 0 }; i < m_Height + 1; ++i)
	{
		if (i != 0)
		{
			switch (m_RoomType)
			{
			case RoomType::BEGINNING:
				m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize, m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, VerticalWall1);
				if(i != (((int)m_Height / 2) + 1))
					m_Dungeon2->Draw(Rectf{ m_Center.x + ((m_Width * m_TileSize) / 2) , m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, VerticalWall2);
				if(i == (((int)m_Height / 2) + 1))
					m_Dungeon2->Draw(Rectf{ m_Center.x + ((m_Width * m_TileSize) / 2) , m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, m_DoorWayLeft);
				break;
			case RoomType::MIDDLE:
				if (i == (((int)m_Height / 2) + 1))
				{
					m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize, m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, m_DoorWayRight);
					m_Dungeon2->Draw(Rectf{ m_Center.x + ((m_Width * m_TileSize) / 2) , m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, m_DoorWayLeft);
				}
				else if (i != (((int)m_Height / 2) + 1))
				{
					m_Dungeon2->Draw(Rectf{ m_Center.x + ((m_Width * m_TileSize) / 2) , m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, VerticalWall2);
					m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize, m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, VerticalWall1);
				}
				break;
			case RoomType::END:
				if (i != (((int)m_Height / 2) + 1))
					m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize, m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, VerticalWall1);
				
				else if(i == (((int)m_Height / 2) + 1))
					m_Dungeon2->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) - m_TileSize, m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, m_DoorWayRight);
				
				m_Dungeon2->Draw(Rectf{ m_Center.x + ((m_Width * m_TileSize) / 2) , m_Center.y - ((m_Height * m_TileSize) / 2) + (i * m_TileSize) - m_TileSize, m_TileSize, m_TileSize }, VerticalWall2);
				break;

			}
		}
	}
}
void Room::DrawFloor()
{
	for (int i{ 0 }; i < m_Width; ++i)
	{
		for (int j{ 0 }; j < m_Height; ++j)
		{
			
			m_TextureTiles->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) + (m_TileSize * i)
				, m_Center.y - ((m_Height * m_TileSize) / 2) + (m_TileSize * j), m_TileSize, m_TileSize }, m_FloorTile);
			
		}
	}
}

void Room::DrawDecor(int type)
{
	/*switch (type)
	{
	case 1:
		
		for (int i{ 0 }; i < m_Width; ++i)
		{
			if (i == 1 || i == m_Width - 2)
			{
				m_TextureObjects->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) + (i * m_TileSize), m_Center.y + (m_Height * m_TileSize) / 2 - m_TileSize, m_TileSize, m_TileSize }, m_BannerBlue);
			}
			else if (i == (int(m_Width )/ 2))
			{
				m_TextureObjects->Draw(Rectf{ m_Center.x - ((m_Width * m_TileSize) / 2) + (i * m_TileSize) ,  m_Center.y + (m_Height * m_TileSize) / 2 - m_TileSize , m_TileSize, m_TileSize }, m_ArchHollow);
			}
		}

		break;
	case 2:
		break;*/

	//}
}

std::vector<Point2f> Room::GetVertices() const
{
	return m_Vertices;
}