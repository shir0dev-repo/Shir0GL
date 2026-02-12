#pragma once
#include "Entity.h"
#include "sogl/rendering/gl/mesh/Mesh.h"
#include "sogl/rendering/renderable.hpp"
#include "sogl/rendering/Material.h"

struct PlayerAircraft : public Entity
{
	PlayerAircraft(const sogl::Mesh*, sogl::Material*);
	~PlayerAircraft();

	virtual void Update(const float& deltaTime) override;

private:
	sogl::renderable* meshRenderer;
};