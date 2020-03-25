#ifndef _SPRINGFROCE_HPP_
#define _SPRINGFROCE_HPP_

#include "Particle.hpp"
#include "Force.hpp"

class SpringForce : public Force
{

public:
	SpringForce(Particle *p1, Particle *p2);

	virtual void draw();
	virtual void apply_force();

private:
	Particle *const m_p1; //particle1
	Particle *const m_p2; //particle2
	double const m_dist;  //rest length
	double const m_ks = 3.f, m_kd = .7f; //spring strength constants
};
#endif
