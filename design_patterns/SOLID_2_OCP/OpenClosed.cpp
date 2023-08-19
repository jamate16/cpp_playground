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

// We follow the single responsability principle, keeping in mind that product has no responsability of filtering
class ProductFilter {
public:
	typedef std::vector<Product*> Items; // Create an alias to std::vector<Product*> to avoid writing that 238942394 times

	Items byColor(Items, Color color);
	Items bySize(Items, Size size);
};

ProductFilter::Items ProductFilter::byColor(Items items, Color color) {
	Items result;
	for (auto& item : items)
		if (item->color == color)
			result.push_back(item);

	return result;
}

// This is a copy of the code above. We are modifying the base class and in case we added another property, we would have to extend this filter class in the same way as with this bySize()
// it would become in the same code duplicated multiple times, so here we need the OC principle. Open for extension, closed for modification.
ProductFilter::Items ProductFilter::bySize(Items items, Size size) {
	Items result;
	for (auto& item : items)
		if (item->size == size)
			result.push_back(item);

	return result;
}

int main() {
	Product p1Ptr { "Controller 1", Color::Red, Size::Small };
	Product p2Ptr { "Controller 2", Color::Red, Size::Medium };
	Product p3Ptr { "Controller 3", Color::Blue, Size::Small };

	ProductFilter::Items myPVector{&p1Ptr, &p2Ptr, &p3Ptr};
	
	ProductFilter myFilter;
	ProductFilter::Items redItems = myFilter.byColor(myPVector, Color::Red);
	ProductFilter::Items smallItems = myFilter.bySize(myPVector, Size::Small);

	for (auto& item : redItems) {
		std::cout << "Item: " << item->name << " color: " << colors[item->color] << " size: " << sizes[item->size] << std::endl;
	}

	for (auto& item : smallItems) {
		std::cout << "Item: " << item->name << " color: " << colors[item->color] << " size: " << sizes[item->size] << std::endl;
	}

	return 0;
}