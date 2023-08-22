/*
Basic vector class implementation to better grasp the concepts of copy constructor/assignment and move constructor/assignment.
*/

/*
Vector should have the ability to:
	- Create it only based on its max capacity
	- Create it from an existing vector
	- Create it from a temporary object
	- Assing it to an existing one
	- Assign it to a temporary one
*/
#include <algorithm>
#include <iostream>
#include <utility>


class Vector {
private:
	int size;
	int* data;

public:
	Vector(int size)
		: size{ size }, data{ new int[size] } {}

	Vector(const Vector& other)
		: size{ other.size }, data{ new int[other.size] } {
		std::cout << "Vector(const Vector& other)" << std::endl;
		std::copy(other.data, other.data + other.size, data);
	}

	Vector(Vector&& other) noexcept
		: size{ other.size }, data{ other.data } {
		std::cout << "Vector(Vector&& other)" << std::endl;

		other.size = 0;
		other.data = nullptr;
	}

	Vector& operator=(Vector& other) {
		std::cout << "Vector& operator=(Vector& other)" << std::endl;
		if (this != &other) {
			delete[] data; // We have to make sure we don't leak memory

			size = other.size;
			data = new int[size];

			std::copy(other.data, other.data + other.size, data);
		}

		return *this;
	}
	
	Vector& operator=(Vector&& other) noexcept {
		std::cout << "Vector& operator=(Vector&& other)" << std::endl;
		if (this != &other) {
			delete[] data;

			size = other.size;
			data = other.data; // No allocation needed, we are "stealing" the pointer

			other.size = 0;
			other.data = nullptr;
		}

		return *this;
	}
};


int main() {
	// Copy constructor
	Vector myV{ 1 };
	Vector myV2(myV);
	
	// Move constructor
	Vector myV3(Vector{ 2 }); // Can't see the move contructor being called because of an optimization called copy elision.
	Vector myV3_1(std::move(Vector{ 2 })); // We force the compiler to move the object instead do doing copy elision.

	// Copy assignment
	Vector myV4{ 3 };
	Vector myV5{ 4 };
	myV4 = myV5;

	// Move assignment
	Vector myV6{ 5 };
	myV6 = Vector{ 6 };

	return 0;
}