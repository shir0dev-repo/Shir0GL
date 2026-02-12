#include "../Entity.h"
#include "sogl/rendering/glUtilities.h"

Entity::Entity()
{
	this->Transform = new sogl::transform();
}

Entity::~Entity()
{
	if (this->Transform != nullptr)
		delete this->Transform;
}

void Entity::Update(const float& deltaTime) {}