#pragma once

class Camera
{
public:
	Camera(float width, float height);
	void SetBoundaries(const Rectf& boundaries);
	Point2f GetPosition(const Rectf& toTrack);
	
	
private:
	float m_Width;
	float m_Height;
	Point2f Track(const Rectf& toTrack);
	void Clamp(Point2f& bottomLeftPos);
	Rectf m_Boundaries;
};