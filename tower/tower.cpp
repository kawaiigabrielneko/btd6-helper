#include "tower.h"
#include <sstream>

Tower::Tower(std::string name, int cost, Attack attack) :
	name(name), cost(cost), attack(attack) {}

Tower::Tower(std::string name, json towerJson) :
		name(name), cost(towerJson.at("cost").get<int>()), attack(Attack(towerJson.at("attack"))) {
	json topUpgradesJson = towerJson.at("topUpgrades");
	for (auto topUpgrade = topUpgradesJson.begin(); topUpgrade != topUpgradesJson.end(); ++topUpgrade) {
		topUpgrades.push_back(Upgrade(*topUpgrade));
	}
	json midUpgradesJson = towerJson.at("midUpgrades");
	for (auto midUpgrade = midUpgradesJson.begin(); midUpgrade != midUpgradesJson.end(); ++midUpgrade) {
		midUpgrades.push_back(Upgrade(*midUpgrade));
	}
	json botUpgradesJson = towerJson.at("botUpgrades");
	for (auto botUpgrade = botUpgradesJson.begin(); botUpgrade != botUpgradesJson.end(); ++botUpgrade) {
		botUpgrades.push_back(Upgrade(*botUpgrade));
	}
}

double Tower::getDamagePerSecond() {
	return attack.getPierce() * attack.getDamage() * attack.getNumProjectiles() / attack.getCooldown();
}

double Tower::getSingleTargetDps() {
	return attack.getDamage() * attack.getNumProjectiles() / attack.getCooldown();
}

int Tower::getCost() {
	return cost;
}

int roundToNearestFive(int value) {
	return (value + 2) / 5 * 5;
}

int Tower::getCost(Difficulty difficulty) {
	int raw_cost = int(cost * DIFFICULTY_MULTIPLIERS[difficulty]);
	return roundToNearestFive(raw_cost);
}

std::string Tower::getName() {
	return name;
}

std::string Tower::getStats() {
	std::ostringstream statStream = std::ostringstream();
	statStream << "Cost: " << cost << std::endl;
	statStream << "Cooldown: " << attack.getCooldown() << std::endl;
	statStream << "Pierce: " << attack.getPierce() << std::endl;
	statStream << "Damage: " << attack.getDamage() << std::endl;
	statStream << "Projectiles: " << attack.getNumProjectiles() << std::endl;
	return statStream.str();
}
