/*
This code is my implementation of Dmitri Nusteruk' example in the Design Patterns in Modern C++ Book

In the first commit I will implement the code that shows the need for the principle, and in the second I
will implement the code with the principle
*/
#include <fstream>
#include <string>
#include <vector>


class Journal {
public:
	std::string title;
	std::vector<std::string> entries;

	explicit Journal(const std::string& title)
		: title{ title } {}

	void add(const std::string& entry);

	void save(const std::string& filename);
};

void Journal::add(const std::string& entry) {
	static int count{ 1 };
	entries.push_back(std::to_string(count++) + ": " + entry);
}

// The responsability of saving entries is the journal's and the responsability of saving the Journal is some other object's

class JournalSaver {
public:
	static void save(Journal& j, std::string filename) {
		std::ofstream ofs{ filename };
		for (auto& entry : j.entries)
			ofs << entry << std::endl;
		ofs.close();
	}
};


int main() {
	Journal myJ{ "This is my Journal" };

	myJ.add("I got the GoW Ragnarok ps5 controller today");
	JournalSaver::save(myJ, "Todays log.txt");

	return 0;
}
