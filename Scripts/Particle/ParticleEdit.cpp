#include "ParticleEdit.h"
#include "../Camera/Camera.h"
#include "../imgui/ImguiControl.h"

void ParticleEachDeta::Update()
{
	auto particle = m_particles.begin();
	auto particleSprite = m_particleSprite.particles.begin();
	for (int i = 0; i < m_particles.size(); i++)
	{
		particleSprite->position = (*particle)->m_position;
		particleSprite->scale = (*particle)->scale;
		(*particle)->Update();
		m_particleSprite.UpdateParticleEdit(Cameras::camera.eye, Cameras::camera.target, Cameras::camera.up);
		if (particle != m_particles.end())
		{
			particle++;
			particleSprite++;
		}
	}
}
