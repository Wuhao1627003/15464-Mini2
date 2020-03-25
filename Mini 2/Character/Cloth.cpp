#include "Cloth.hpp"
#include "SpringForce.hpp"
#include <cstdlib>
//parallelize using openmp
#include <omp.h>

#define DAMP .9f
//side length of grid
#define NUM 100

#define PSIZE (NUM * NUM)
#define FSIZE (6 * NUM * NUM - 10 * NUM + 2)

//array insertions faster than vector
static Particle* pVector[PSIZE];
static Force* fVector[FSIZE];

using namespace std;

Cloth::Cloth() {

	Vector3f center = make_vector(5.0f, 5.0f, 5.0f);
	float dist = 10.f / NUM;
	Vector3f xoffset = make_vector(dist, .0f, .0f);
	Vector3f yoffset = make_vector(.0f, .0f, dist);

	long counter = 0;

	//Init particles
#pragma omp for
	for (int x = 0; x < NUM; x++) {
		for (int y = 0; y < NUM; y++) {
			pVector[counter++] = new Particle(center + (x - NUM / 2) * xoffset + (y - NUM / 2) * yoffset);
		}
	}

	//lambda lookup
	auto vec = [](int x, int y) { return pVector[x * NUM + y]; };

	counter = 0;
	//Init spring forces
#pragma omp for
	for (int x = 0; x < NUM; x++) {
		for (int y = 0; y < NUM; y++) {
			if (y != NUM - 1)
			{
				//stretch
				fVector[counter++] = new SpringForce(vec(x, y), vec(x, y + 1));
				fVector[counter++] = new SpringForce(vec(y, x), vec(y + 1, x));
				if (x != NUM - 1)
				{
					//shear
					fVector[counter++] = new SpringForce(vec(x, y), vec(x + 1, y + 1));
					fVector[counter++] = new SpringForce(vec(x + 1, y), vec(x, y + 1));
				}
				if (y != NUM - 2)
				{
					//bending
					fVector[counter++] = new SpringForce(vec(x, y), vec(x, y + 2));
					fVector[counter++] = new SpringForce(vec(y, x), vec(y + 2, x));
				}
			}
		}
	}
	assert(counter == FSIZE);
}

Cloth::~Cloth() {
}

void Cloth::reset() {
	for (Particle* p : pVector) {
		p->reset();
	}
}

void Cloth::draw() {
	for (Particle* p : pVector) {
		p->draw();
	}

	for (Force* f : fVector) {
		f->draw();
	}
}

void Cloth::simulation_step() {

	///TODO:
	///first, you need to clear force accumulators for all the particles
#pragma omp for
	for (long i = 0; i < PSIZE; i ++ ) {
		pVector[i]->clearForce();
	}

	///second, apply spring forces to them
#pragma omp for
	for (long i = 0; i < FSIZE; i++) {
		fVector[i]->apply_force();
	}

	///if you want to implement hard constraints, the third step is to calculate constraint forces
	///for the basic cloth simulation, you can skip this.

	///Then, we can move forward
	///Change this to others if you want to implement RK2 or RK4 or other integration method
	//forward_euler_step();
	backward_euler_step();
	//verlet_step();

	///Finally, if you want to implement collisions, you could solve them here
	///for the basic cloth simulation, you can skip this.
	//naive handler for colliding into y=0 plane
#pragma omp for
	for (int x = 0; x < NUM; x++) {
		for (int y = 1; y < NUM - 1; y++) {
			Particle* p = pVector[x * NUM + y];
			if (p->m_Position.c[1] < 0) {
				p->m_Velocity = -DAMP * p->m_ForceAccumulator * dt / p->m_Mass;
				p->m_Position += dt * p->m_Velocity;
			}
		}
	}
	
}

void Cloth::forward_euler_step() {
#pragma omp for
	for (int x = 1; x < NUM - 1; x++) {
		for (int y = 1; y < NUM - 1; y++) {
			Particle* p = pVector[x * NUM + y];
			p->m_Position += dt * p->m_Velocity;
			p->m_Velocity = DAMP * (p->m_Velocity) + dt * p->m_ForceAccumulator / p->m_Mass;
		}
	}
}

void Cloth::verlet_step() {
#pragma omp for
	for (int x = 1; x < NUM - 1; x++) {
		for (int y = 1; y < NUM - 1; y++) {
			Particle* p = pVector[x * NUM + y];
			Vector3f prev_pos = p->m_PrevPosition;
			p->m_PrevPosition = p->m_Position;
			p->m_Position = 2 * p->m_Position - prev_pos + dt * dt * p->m_ForceAccumulator / p->m_Mass;
		}
	}
}

void Cloth::backward_euler_step() {
#pragma omp for
	for (int x = 1; x < NUM - 1; x++) {
		for (int y = 1; y < NUM - 1; y++) {
			Particle* p = pVector[x * NUM + y];
			p->m_Velocity = DAMP * (p->m_Velocity) + dt * p->m_ForceAccumulator / p->m_Mass;
			p->m_Position += dt * p->m_Velocity;
		}
	}
}
