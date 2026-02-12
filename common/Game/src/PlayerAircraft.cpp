#include "../PlayerAircraft.h"
#include "sogl/rendering/glUtilities.h"
#include "sogl/rendering/renderable.hpp"
#include "sogl/rendering/gl/mesh/Mesh.h"
#include "sogl/rendering/Material.h"

PlayerAircraft::PlayerAircraft(const sogl::Mesh* mesh, sogl::Material* material) : Entity()
{
	this->meshRenderer = new sogl::renderable(*mesh, material);
}

void PlayerAircraft::Update(const float& deltaTime)
{
	meshRenderer->render();
}

PlayerAircraft::~PlayerAircraft()
{
	if (this->meshRenderer != nullptr)
	{
		delete meshRenderer;
		meshRenderer = nullptr;
	}
	if (this->Transform != nullptr)
	{
		delete Transform;
		Transform = nullptr;
	}
}