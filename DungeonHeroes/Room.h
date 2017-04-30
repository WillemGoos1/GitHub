#pragma once
#include <vector>
#include "Texture.h"


struct Doorway
{
	Point2f doorPoint1;
	Point2f doorPoint2;
};

enum RoomType
{
	BEGINNING = 0,
	MIDDLE = 1,
	END = 2
};
class Room
{
public:
	explicit Room(const Point2f& center, float width, float height, RoomType t);
	~Room();
	void Draw();
	void Update(float elapsedSec);
	void CalculateVertices();
	Rectf GetRect();
	static float m_HallWayWidth;
	static float m_TileSize;
	Doorway GetDoorEnter();
	Doorway GetDoorExit();
	std::vector<Point2f> GetVertices() const;
	RoomType m_RoomType;
private:
	Point2f m_Center;
	float m_Width;
	float m_Height;
	std::vector<Point2f> m_Vertices;
	Doorway m_DoorEnter;
	Doorway m_DoorExit;
	
	Rectf m_Rectangle;

	int m_LayoutType;

	//texture related variables
	static Texture* m_TextureTiles;
	static Texture* m_TextureObjects;
	static int m_InstanceCounter;
	//static Rectf m_OuterWallHorizontal;
	//static Rectf m_CornerWall;
	static float m_TextureTileWidth;
	//static Rectf m_OuterWallVertical;
	//static Rectf m_CornersUp;
	static Rectf m_FloorTile;
	//static Rectf m_IsoMetricWall;
	//static Rectf m_Pillar;
	//static Rectf m_Arch;
	//static Rectf m_BannerBlue;
	//static Rectf m_BannerRed;
	//static Rectf m_ArchHollow;

	static Rectf m_DungeonHorizontal1;
	static Rectf m_DungeonHorizontal2;
	static Rectf m_DungeonVertical1;
	static Rectf m_DungeonVertical2;
	static Rectf m_CornerRightUp;
	static Rectf m_CornerRightDown;
	static Rectf m_CornerLeftUp;
	static Rectf m_CornerLeftDown;
	static Rectf m_DoorWayRight;
	static Rectf m_DoorWayLeft;

	static Rectf m_HorizontalWall3;
	static Rectf m_HorizontalWall4;
	static Rectf m_VerticalWall3;
	static Rectf m_VerticalWall4;

	static Texture* m_Dungeon2;

	//draw help functions
	void DrawVertices();
	void DrawWalls();
	void DrawFloor();
	void DrawDecor(int type);
};