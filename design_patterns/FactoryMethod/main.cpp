/*
This code is my C++ implementation based off of Derek Banas' Java code.

First as in the video, I'm going to write how the program would be without the pattern
and then I will implement the pattern.
*/
#include <string>
#include <iostream>


class EnemyShip {
protected:
	std::string name;
	float amountDamage;

public:

	void followHero() {
		std::cout << name << " is following the hero." << std::endl;
	}

	void displayShip() {
		std::cout << name << " is on screen." << std::endl;
	}

	void shoot() {
		std::cout << name << " attacks hero and deals " << amountDamage << "." << std::endl;
	}
};


class UFOEnemyShip : public EnemyShip {
public:
	UFOEnemyShip() {
		name = "UFO";
		amountDamage = 15;
	}
};

class RocketEnemyShip : public EnemyShip {
public:
	RocketEnemyShip() {
		name = "Rocket";
		amountDamage = 30;
	}
};

class EnemyShipFactory {
public:
	EnemyShip* makeEnemyShip(char typeShip) {
		EnemyShip* enemyShip; // I have a question here, does this force object slicing?
		switch (typeShip) {
			case 'U':
				enemyShip = new UFOEnemyShip();
				break;
			case 'R':
				enemyShip = new RocketEnemyShip();
				break;
			default:
				enemyShip = new UFOEnemyShip();
		}
		return enemyShip;
	}
};

void doEnemyStuff(EnemyShip &ship) {
	ship.displayShip();
	ship.followHero();
	ship.shoot();
}

int main() {
	char enemyUserOption;

	std::cout << "Which enemy ship would you like to spawn?: (U / R)\n";
	std::cin >> enemyUserOption;

	EnemyShipFactory enemyShipFactory;
	EnemyShip *enemyShip = enemyShipFactory.makeEnemyShip(enemyUserOption);

	doEnemyStuff(*enemyShip);

	delete enemyShip;
	return 0;
}