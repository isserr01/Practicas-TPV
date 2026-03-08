#include "Gun.h"


	Gun::Gun() :
		_tr(nullptr),
		_lastBullet(0),
		_lastShotTime(0) 
	{
		for (auto& b : _bullets) {
			b.used = false;
		}
	}

	Gun::~Gun() = default;

	void Gun::initComponent() {
		_tr = _ent->getComponent<Transform>();
		assert(_tr != nullptr);
	}


	void Gun::update() {
		auto& vt = sdlutils().virtualTimer();

		//mover balas
		for (auto& b : _bullets) {
			if (b.used) {
				b.pos = b.pos + b.vel;
				std::cout << b.pos.getX() << " " << b.pos.getY() << std::endl;
			}
		}
		

		//disparar
		if (ih().isKeyDown(SDL_SCANCODE_S)) {
			if (vt.currTime() > _lastShotTime + 250) {
				Vector2D& p = _tr->getPos();
				Vector2D& v = _tr->getVel();
				float r = _tr->getRot();

				int bw = 5;
				int bh = 20;

				Vector2D c = p + Vector2D(
					_tr->getWidth() / 2.0f,
					_tr->getHeight() / 2.0f
				);

				Vector2D bp = c - Vector2D(0, 30).rotate(r);
				Vector2D bv = Vector2D(0, -1).rotate(r) * (v.magnitude() + 5.0f);
				float br = Vector2D(0, -1).angle(bv);
				shoot(bp, bv, bw, bh, br);

				_lastShotTime = vt.currTime();

				//sdlutils().soundEffects().at("fire")->play();

			}
		}
	}
	//renderizado de las balas en su posicion
	void Gun::render() {
		auto tex = &sdlutils().images().at("fire");

		for (auto& b : _bullets) {

			if (b.used) {

				SDL_FRect dst = {
					(int)b.pos.getX(),
					(int)b.pos.getY(),
					b.width,
					b.height
				};

				tex->render(dst, b.rot);
			}
		}
	}

	void Gun::reset() {
		for (auto& b : _bullets) {
			b.used = false;
		}
	}

	void Gun::shoot(Vector2D p, Vector2D v, int width, int height, float r) {
		//busqueda circular de las balas para inicializar
		for (int i = 0; i < _max_bullets; i++) {
			int idx = (_lastBullet + i) % _max_bullets;

			if (!_bullets[idx].used) {
				Bullet& b = _bullets[idx];

				_lastBullet = (idx + 1) % _max_bullets;
				b.used = true;
				b.pos = p;
				b.vel = v;
				b.width = width;
				b.height = height;
				b.rot = r;
				return;
			}
		}
	}
