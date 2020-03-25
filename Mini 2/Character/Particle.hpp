#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_

#define GRAV 9.8f

#include <Vector/Vector.hpp>
#include <Graphics/Graphics.hpp>

class Particle{

public:
	Particle(const Vector3f & ConstructPos);
	virtual ~Particle(void);

	void reset();
	void draw();

	void clearForce();

	Vector3f m_ConstructPos;
	Vector3f m_Position;
	Vector3f m_PrevPosition;
	Vector3f m_Velocity;
	Vector3f m_ForceAccumulator;
	float m_Mass;
};


#endif
