#include "Entity.h"

Entity::Entity()  { root = new SoSeparator; root->ref(); }
Entity::~Entity() { root->unref(); }
