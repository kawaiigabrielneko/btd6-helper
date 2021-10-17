#ifndef ATTACK
#define ATTACK
#include <nlohmann/json.hpp>
#include "damage.h"
#include "upgrade.h"
using json = nlohmann::json;
class AttackBuilder;
class Attack {
	double cooldown;
	int pierce;
	Damage damage;
	int numProjectiles;
public:
	Attack();
	Attack(double, int, Damage, int);
	Attack(json);
	double getCooldown();
	int getPierce();
	Damage getDamage();
	int getBaseDamage();
	int getCeramicDamage();
	int getMoabDamage();
	int getNumProjectiles();
	double getDamagePerSecond();
	Attack improve(Upgrade);
	friend class AttackBuilder;
};
#endif // !ATTACK
