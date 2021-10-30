#include "tower.h"
#include <sstream>

Tower::Tower(std::string name, int cost, Attack attack) :
	name(name), cost(cost), attacks(attack) {}

Tower::Tower(std::string name, json towerJson) :
		name(name), cost(towerJson.at("cost").get<int>()), attacks(Attack(towerJson.at("attack"))) {
	json topUpgradesJson = towerJson.value("topUpgrades", json::array());
	for (auto topUpgrade = topUpgradesJson.begin(); topUpgrade != topUpgradesJson.end(); ++topUpgrade) {
		topUpgrades.push_back(Upgrade(*topUpgrade));
	}
	json midUpgradesJson = towerJson.value("midUpgrades", json::array());
	for (auto midUpgrade = midUpgradesJson.begin(); midUpgrade != midUpgradesJson.end(); ++midUpgrade) {
		midUpgrades.push_back(Upgrade(*midUpgrade));
	}
	json botUpgradesJson = towerJson.value("botUpgrades", json::array());
	for (auto botUpgrade = botUpgradesJson.begin(); botUpgrade != botUpgradesJson.end(); ++botUpgrade) {
		botUpgrades.push_back(Upgrade(*botUpgrade));
	}
}

Tower::Tower(const Tower& tower) {
	name = tower.name;
	cost = tower.cost;
	attacks = tower.attacks;
	topUpgrades = tower.topUpgrades;
	midUpgrades = tower.midUpgrades;
	botUpgrades = tower.botUpgrades;
}

double Tower::getDamagePerSecond() {
	return attacks.getTotalDps();
}

double Tower::getDamagePerSecond(UpgradePattern upgradePattern) {
	if (upgradePattern.top > topUpgrades.size()
		|| upgradePattern.mid > midUpgrades.size()
		|| upgradePattern.bot > botUpgrades.size()) {
		throw std::invalid_argument(name + "does not have that many upgrades");
	}
	AttackList upgradedAttacks = attacks;
	for (int i = 0; i < upgradePattern.top; i++) {
		std::vector<Buff*> topBuffs = topUpgrades[i].getBuffs();
		for (std::vector<Buff*>::iterator topBuff = topBuffs.begin(); topBuff != topBuffs.end(); ++topBuff) {
			upgradedAttacks = (*topBuff)->buff(upgradedAttacks);
		}
	}
	for (int i = 0; i < upgradePattern.mid; i++) {
		std::vector<Buff*> midBuffs = midUpgrades[i].getBuffs();
		for (std::vector<Buff*>::iterator midBuff = midBuffs.begin(); midBuff != midBuffs.end(); ++midBuff) {
			upgradedAttacks = (*midBuff)->buff(upgradedAttacks);
		}
	}
	for (int i = 0; i < upgradePattern.bot; i++) {
		std::vector<Buff*> botBuffs = botUpgrades[i].getBuffs();
		for (std::vector<Buff*>::iterator botBuff = botBuffs.begin(); botBuff != botBuffs.end(); ++botBuff) {
			upgradedAttacks = (*botBuff)->buff(upgradedAttacks);
		}
	}
	return upgradedAttacks.getTotalDps();
}

double Tower::getSingleTargetDps() {
	return 0;
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

bool Tower::matches(std::string str) {
	std::istringstream iss(name);
	std::string token;
	while (iss >> token) {
		if (str.length() == token.length()) {
			int len = str.length();
			bool matches = true;
			for (int i = 0; i < len; i++) {
				if (tolower(str[i]) != tolower(token[i])) {
					matches = false;
					break;
				}
			}
			if (matches) {
				return true;
			}
		}
	}
	return false;
}

std::string Tower::getUpgradePathStats(char initial, std::vector<Upgrade>& upgrades) {
	std::ostringstream statStream{};
	for (int i = 0; i < upgrades.size(); i++) {
		statStream << initial << i + 1 << ":" << std::endl;
		statStream << upgrades[i].getName() << " " << upgrades[i].getCost() << std::endl;
		for (std::vector<Buff*>::iterator buff = upgrades[i].getBuffs().begin(); 
				buff != upgrades[i].getBuffs().end(); ++buff) {
			statStream << **buff << std::endl;
		}
	}
	return statStream.str();
}

std::string Tower::getTopUpgradeStats() {
	return getUpgradePathStats('T', topUpgrades);
}

std::string Tower::getMidUpgradeStats() {
	return getUpgradePathStats('M', midUpgrades);
}

std::string Tower::getBotUpgradeStats() {
	return getUpgradePathStats('B', botUpgrades);
}

std::string Tower::getStats() {
	std::ostringstream statStream = std::ostringstream();
	statStream << "Cost: " << cost << std::endl;
	attacks.streamStats(statStream);
	statStream << getTopUpgradeStats();
	statStream << getMidUpgradeStats();
	statStream << getBotUpgradeStats();
	return statStream.str();
}

std::string Tower::getStats(UpgradePattern upgradePattern) {
	if (upgradePattern.top > topUpgrades.size()
		|| upgradePattern.mid > midUpgrades.size() 
		|| upgradePattern.bot > botUpgrades.size()) {
		return name + " does not have that many upgrades\n";
	}
	std::ostringstream statStream = std::ostringstream();
	AttackList upgradedAttacks = attacks;
	int totalCost = cost;
	for (int i = 0; i < upgradePattern.top; i++) {
		std::vector<Buff*> topBuffs = topUpgrades[i].getBuffs();
		for (std::vector<Buff*>::iterator topBuff = topBuffs.begin(); topBuff != topBuffs.end(); ++topBuff) {
			upgradedAttacks = (*topBuff)->buff(upgradedAttacks);
		}
	}
	for (int i = 0; i < upgradePattern.mid; i++) {
		std::vector<Buff*> midBuffs = midUpgrades[i].getBuffs();
		for (std::vector<Buff*>::iterator midBuff = midBuffs.begin(); midBuff != midBuffs.end(); ++midBuff) {
			upgradedAttacks = (*midBuff)->buff(upgradedAttacks);
		}
	}
	for (int i = 0; i < upgradePattern.bot; i++) {
		std::vector<Buff*> botBuffs = botUpgrades[i].getBuffs();
		for (std::vector<Buff*>::iterator botBuff = botBuffs.begin(); botBuff != botBuffs.end(); ++botBuff) {
			upgradedAttacks = (*botBuff)->buff(upgradedAttacks);
		}
	}
	statStream << "Cost: " << totalCost << std::endl;
	upgradedAttacks.streamStats(statStream);
	return statStream.str();
}