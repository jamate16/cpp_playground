/*
I'll be implementing a Builder design pattern (to practice) that will create console controllers with different customization options
*/

#include <string>
#include <iostream>


class ControllerScheme {
public:

	std::string triggers;
	std::string bumpers;

	std::string homeButton;
	std::string dPad;
	std::string menuButtons;
	std::string faceButtons;

	std::string frontShell;
	std::string backShell;

	std::string joySticks;

	ControllerScheme(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string);
};

ControllerScheme::ControllerScheme(std::string triggersColor, std::string bumpersColor, std::string homeButtonColor, std::string dPadColor, std::string menuButtonsColor, std::string faceButtonsColor, std::string frontShellColor, std::string backShellColor, std::string joySticksColor)
	: triggers(triggersColor), bumpers(bumpersColor), homeButton(homeButtonColor), dPad(dPadColor), menuButtons(menuButtonsColor), faceButtons(faceButtonsColor), frontShell(frontShellColor), backShell(backShellColor), joySticks(joySticksColor) {}

class DualsenseControllerScheme : public ControllerScheme {
public:
	std::string trim;
	std::string touchPad;

	DualsenseControllerScheme()
		: ControllerScheme{ "black", "black", "black", "white", "white", "white", "white", "white", "black" }, trim{ "black" }, touchPad{ "white" } {}
};

class XboxControllerScheme : public ControllerScheme {
public:
	std::string batteryCover;

	XboxControllerScheme()
		: ControllerScheme{ "black", "black", "black", "black", "black", "black", "black", "black", "black" }, batteryCover{ "black" } {}
};

template<typename T>
class ControllerSchemeBuilder {
public:
	T* controllerScheme;

	typedef ControllerSchemeBuilder Builder;

	virtual ~ControllerSchemeBuilder();
	virtual Builder* setTriggers(std::string);
	virtual Builder* setBumpers(std::string);
	virtual Builder* setHomeButton(std::string);
	virtual Builder* setDPad(std::string);
	virtual Builder* setMenuButtons(std::string);
	virtual Builder* setFaceButtons(std::string);
	virtual Builder* setFrontShell(std::string);
	virtual Builder* setBackShell(std::string);
	virtual Builder* setJoySticks(std::string);
	virtual T* build();
};

template<typename T>
ControllerSchemeBuilder<T>::~ControllerSchemeBuilder() {
	delete controllerScheme;
}

template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setTriggers(std::string color) { controllerScheme->triggers = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setBumpers(std::string color) { controllerScheme->bumpers = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setHomeButton(std::string color) { controllerScheme->homeButton = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setDPad(std::string color) { controllerScheme->dPad = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setMenuButtons(std::string color) { controllerScheme->menuButtons = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setFaceButtons(std::string color) { controllerScheme->faceButtons = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setFrontShell(std::string color) { controllerScheme->frontShell = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setBackShell(std::string color) { controllerScheme->backShell = color; return this; }
template<typename T>
ControllerSchemeBuilder<T>* ControllerSchemeBuilder<T>::setJoySticks(std::string color) { controllerScheme->joySticks = color; return this; }

template<typename T>
T* ControllerSchemeBuilder<T>::build() { return controllerScheme; }

class DualsenseControllerSchemeBuilder : public ControllerSchemeBuilder<DualsenseControllerScheme> {
public:
	DualsenseControllerSchemeBuilder() {
		controllerScheme = new DualsenseControllerScheme();
	}

	Builder* setTrim(std::string color) { controllerScheme->trim = color; return this; }
	Builder* setTouchPad(std::string color) { controllerScheme->touchPad = color; return this; }
};


class XboxControllerSchemeBuilder : public ControllerSchemeBuilder<XboxControllerScheme> {
public:
	XboxControllerSchemeBuilder() {
		controllerScheme = new XboxControllerScheme();
	}

	Builder* setBatteryCover(std::string color) { controllerScheme->batteryCover = color; return this; }
};

int main() {
	DualsenseControllerSchemeBuilder* myPSControlBuilder = new DualsenseControllerSchemeBuilder();
	auto controller = myPSControlBuilder->setTrim("purple")->build();

	std::cout << controller->trim << std::endl;

	delete myPSControlBuilder;
	return 0;
}