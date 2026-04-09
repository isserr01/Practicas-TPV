#include "../ecs/System.h"
#include "../ecs/Entity.h"

struct Transform;

class GhostSystem : public ecs::System {
public:

	GhostSystem();
	virtual ~GhostSystem();
	void spawnGhost() override;
	void update() override;
	Vector2D randomVelocity();
private:
	std::unordered_map<Entity*, Vector2D> velocities;
	Transform* _pmTR;
	int timerGhost;
	bool immunity;
	int cantidadGhosts;
};

