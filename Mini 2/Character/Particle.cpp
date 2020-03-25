#include "Particle.hpp"

Particle::Particle(const Vector3f & ConstructPos) 
{
	m_ConstructPos = make_vector< float >(ConstructPos.x, ConstructPos.y, ConstructPos.z);
	m_PrevPosition = m_Position = make_vector< float >(0.0f, 0.0f, 0.0f);
	m_Velocity = make_vector< float >(0.0f, 0.0f, 0.0f);
	m_ForceAccumulator = make_vector< float >(0.0f, 0.0f, 0.0f);
	m_Mass = 1.f;
}

Particle::~Particle(void)
{
}

void Particle::reset()
{
	m_PrevPosition = m_Position = m_ConstructPos;
	m_Velocity = make_vector< float >(0.0f, 0.0f, 0.0f);
}

void Particle::draw()
{
	const double h = 0.3;
	glColor3f(.9f, 0.f, 0.2f);
	glBegin(GL_QUADS);
	glVertex3f(m_Position.x-h/2.0, m_Position.y-h/2.0, m_Position.z);
	glVertex3f(m_Position.x+h/2.0, m_Position.y-h/2.0, m_Position.z);
	glVertex3f(m_Position.x+h/2.0, m_Position.y+h/2.0, m_Position.z);
	glVertex3f(m_Position.x-h/2.0, m_Position.y+h/2.0, m_Position.z);
	glEnd();
}

void Particle::clearForce()
{
	///clears the force to just gravity
	m_ForceAccumulator = make_vector< float >(0.0f, -GRAV * m_Mass, 0.0f);
}

