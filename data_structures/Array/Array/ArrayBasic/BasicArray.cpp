#include <iostream>

class Array {
private:
	int* arr_ptr;
	int size;

public:
	Array(int size)
		: arr_ptr{ new int[size] }, size{ size } {}

	~Array() {
		delete[] arr_ptr;
	}

	void insert(int idx, int value) {
		if (!arr_ptr) return;
		arr_ptr[idx] = value;
	}

	void remove(int idx) {
		if (!arr_ptr) return;
		arr_ptr[idx] = -1; // Default value
	}
	
	int getValue(int idx) const {
		if (idx > size)
			return -1;
		return arr_ptr[idx];
	}

	int getSize() const {
		return size;
	}
};

Array populateArrayInteractively() {
	int size;
	int length;

	std::cout << "Enter size of the array: ";
	std::cin >> size; // No value checking for now

	std::cout << "Enter number of elemets to add to the array (has to be lower than max size): ";
	std::cin >> length;

	if (length > size) {
		std::cerr << "Error: Number of elements must be less than or equal to max size\n";
		throw 1;
	}

	Array builtArr(size);

	int numToAdd;
	std::cout << "Enter elements: ";
	for (int i{ 0 }; i < length; ++i) {
		std::cin >> numToAdd;
		builtArr.insert(i, numToAdd);
	}

	return builtArr;
}

void displayArray(const Array& arr) {
	std::cout << "Printing the contents of the array:\n";
	for (int i{ 0 }; i < arr.getSize(); ++i) {
		std::cout << arr.getValue(i) << std::endl;
	}
}

int main() {
	
	Array arr = populateArrayInteractively();
	displayArray(arr);

	return 0;
}