#include "SpringForce.hpp"

SpringForce::SpringForce(Particle *p1, Particle *p2) : 
m_p1(p1), m_p2(p2), m_dist(length(p1->m_Position - p2->m_Position)){}

void SpringForce::draw()
{
	glBegin( GL_LINES );
	glColor3f(0.4, 0.2, 0.3);
	glVertex3f( m_p1->m_Position.x, m_p1->m_Position.y, m_p1->m_Position.z);
	glColor3f(0.4, 0.2, 0.3);
	glVertex3f( m_p2->m_Position.x, m_p2->m_Position.y, m_p2->m_Position.z);
	glEnd();
}

void SpringForce::apply_force()
{
	///TODO
	Vector3f cur_diff = m_p1->m_Position - m_p2->m_Position;
	double cur_dist = length(cur_diff);
	Vector3f cur_normalized = normalize(cur_diff);
	Vector3f f = -(m_ks * (cur_dist - m_dist) + m_kd * dot(m_p1->m_Velocity - m_p2->m_Velocity, cur_normalized)) * cur_normalized;
	m_p1->m_ForceAccumulator += f;
	m_p2->m_ForceAccumulator -= f;
}

