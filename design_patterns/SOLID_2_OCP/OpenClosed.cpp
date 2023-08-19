/*
This code is my implementation of Dmitri Nusteruk' example in the Design Patterns in Modern C++ Book

In the first commit I will implement the code that shows the need for the principle, and in the second I
will implement the code with the principle
*/
#include <string>
#include <vector>
#include <iostream>
#include <map>


enum class Color { Red, Green, Blue };

std::map<Color, std::string> colors{ {Color::Red, "Red"},
									 {Color::Green, "Green"},
									 {Color::Blue, "Blue"} };

enum class Size { Small, Medium, Large };

std::map<Size, std::string> sizes{ {Size::Small, "Small"},
									{Size::Medium, "Medium"},
									{Size::Large, "Large"} };

class Product {
public:
	std::string name;
	Color color;
	Size size;

	Product(std::string name, Color color, Size size);
};

Product::Product(std::string name, Color color, Size size)
	: name{name}, color{color}, size{size} {}

// We follow SRP further, and we divide filtering into a filter and a specification
// Specification:
template <typename T> class Specification {
public:
	virtual bool isSatisfied(T* item) = 0; // We enforce the use overwrites this method in every concrete implementation
};

// Filter:
template <typename T> class Filter {
public:
	virtual std::vector<T*> filter(std::vector<T*> items, Specification<T>& spec) = 0; // Again, we enforce
};

// Now we create a concrete filter using the abstract classes implemented
class BetterProductFilter : public Filter<Product> {
public:
	std::vector<Product*> filter(std::vector<Product*> items, Specification<Product>& spec) override {
		std::vector<Product*> filteredItems;

		for (auto& item : items) {
			if (spec.isSatisfied(item))
				filteredItems.push_back(item);
		}
		return filteredItems;
	}
};

class ColorSpecification : public Specification<Product> {
public:
	Color color; // Maybe this would be better using dependency injection? R/. It sort of already has the dependency injected, since we just declare the object here but initialize it based on what's passed in the constructor

	explicit ColorSpecification(Color color) /* Explicit, otherwise when using the filter, we could pass a color instead of specification, and the compiler would try to implicitly convert one type to the other, generating all sorts of problems*/
		: color{color} {}

	bool isSatisfied(Product* item) override {
		return item->color == color;
	}
};

// We extend specification for sizes
class SizeSpecification : public Specification<Product> {
public:
	Size size; // Maybe this would be better using dependency injection?

	explicit SizeSpecification(Size size) /* Explicit, otherwise when using the filter, we could pass a size instead of specification, and the compiler would try to implicitly convert one type to the other, generating all sorts of problems*/
		: size{ size } {}

	bool isSatisfied(Product* item) override {
		return item->size == size;
	}
};

int main() {
	Product p1Ptr { "Controller 1", Color::Red, Size::Small };
	Product p2Ptr { "Controller 2", Color::Red, Size::Medium };
	Product p3Ptr { "Controller 3", Color::Blue, Size::Small };

	std::vector<Product*> myPVector{&p1Ptr, &p2Ptr, &p3Ptr};
	
	BetterProductFilter bF;
	ColorSpecification specRedColor{ Color::Red };
	SizeSpecification specSmallSize{ Size::Small };

	std::vector<Product*> redItems = bF.filter(myPVector, specRedColor);
	std::vector<Product*> smallItems = bF.filter(myPVector, specSmallSize);

	for (auto& item : redItems) {
		std::cout << "Item: " << item->name << " color: " << colors[item->color] << " size: " << sizes[item->size] << std::endl;
	}

	for (auto& item : smallItems) {
		std::cout << "Item: " << item->name << " color: " << colors[item->color] << " size: " << sizes[item->size] << std::endl;
	}

	return 0;
}