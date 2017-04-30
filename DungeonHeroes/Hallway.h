#pragma once
#include <vector>
#include "Room.h"
class Texture;

enum HallwayDirection
{
	UP = 0,
	DOWN = 1,
	STRAIGHT = 2
};
class Hallway
{
public:
	Hallway( Doorway& d1,  Doorway& d2);
	~Hallway();
	void Draw() const;
	std::vector<Point2f> GetVertices() const;
	HallwayDirection m_HallwayDirection;
private:
	void GenerateVertices();
	
	std::vector<Point2f> m_Vertices;
	Doorway m_DEnter;
	Doorway m_DExit;
	

	static float m_TextureTileWidth;
	static int m_InstanceCounter;
	static Texture* m_pTextureTiles;
	static Rectf m_IsoMetricWall;
	static Rectf m_HorizontalWallDown;
	static Rectf m_HorizontalWallUp;
	static Rectf m_CornerUp;
	static Rectf m_CornerDown;
	static Rectf m_WallRight;
	static Rectf m_WallLeft;
	static Rectf m_CornerDownRight;
	static Rectf m_CornerTopRight;
	

};