#include "stdafx.h"
#include "Camera.h"
#include <iostream>
Camera::Camera(float width, float height):
	m_Width{width},
	m_Height{height},
	m_Boundaries{0.0f,0.0f, width, height}
{}

void Camera::SetBoundaries(const Rectf& boundaries)
{
	m_Boundaries = boundaries;
	
}
Point2f Camera::GetPosition(const Rectf& toTrack)
{
	return Track(toTrack);
}
Point2f Camera::Track(const Rectf& toTrack)
{
	Point2f centerShape{ toTrack.left + (toTrack.width / 2), toTrack.bottom + (toTrack.height / 2) };
	Point2f returnValue{ centerShape.x - (m_Width / 2), centerShape.y - (m_Height / 2) };
	Clamp(returnValue);
	return returnValue;
}
void Camera::Clamp(Point2f& bottomLeft)
{
	if (bottomLeft.x < m_Boundaries.left - 32.0f )
	{
		bottomLeft.x = m_Boundaries.left - 32.0f;
	}
	if ((bottomLeft.x + m_Width) >= m_Boundaries.left + m_Boundaries.width + 32.0f)
	{
		bottomLeft.x = m_Boundaries.left + m_Boundaries.width - m_Width + 32.0f;
	}
	if (bottomLeft.y < m_Boundaries.bottom - 32.0f)
	{
		bottomLeft.y = m_Boundaries.bottom - 32.0f;
	}
	if (bottomLeft.y + m_Height >= m_Boundaries.bottom + m_Boundaries.height + 32.0f)
	{
		bottomLeft.y = m_Boundaries.bottom + m_Boundaries.height - m_Height + 32.0f;
	}
}


