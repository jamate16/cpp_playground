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

void doEnemyStuff(EnemyShip &ship) {
	ship.displayShip();
	ship.followHero();
	ship.shoot();
}

int main() {
	char enemyUserOption;

	std::cout << "Which enemy ship would you like to spawn?: (U / R)\n";
	std::cin >> enemyUserOption;

	// What if later on we decide to add lots of enemies, should we be changing the main function of out program? Hell nah!. Let's use a Factory Method design pattern.
	EnemyShip* enemyShip;
	switch (enemyUserOption) {
		case 'U':
			enemyShip = new UFOEnemyShip();
			break;
		case 'R':
			enemyShip = new RocketEnemyShip();
			break;
		default:
			enemyShip = new UFOEnemyShip();
	}

	doEnemyStuff(*enemyShip);

	delete enemyShip;
	return 0;
}