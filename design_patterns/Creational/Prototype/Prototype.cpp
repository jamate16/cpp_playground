/*
I'll be implementing a Builder design pattern (to practice) that will create console controllers with different customization options
*/

#include <string>
#include <iostream>


class ControllerScheme {
public:
	typedef std::string str;

	str triggers;
	str bumpers;

	str homeButton;
	str dPad;
	str menuButtons;
	str faceButtons;

	str frontShell;
	str backShell;

	str joySticks;

	ControllerScheme(str, str, str, str, str, str, str, str, str);

	virtual ControllerScheme* clone() {
		return new ControllerScheme(*this);
	}
};

ControllerScheme::ControllerScheme(str triggersColor, str bumpersColor, str homeButtonColor, str dPadColor, str menuButtonsColor, str faceButtonsColor, str frontShellColor, str backShellColor, str joySticksColor)
	: triggers(triggersColor), bumpers(bumpersColor), homeButton(homeButtonColor), dPad(dPadColor), menuButtons(menuButtonsColor), faceButtons(faceButtonsColor), frontShell(frontShellColor), backShell(backShellColor), joySticks(joySticksColor) {}

class DualsenseControllerScheme : public ControllerScheme {

public:
	str trim;
	str touchPad;

	static DualsenseControllerScheme defaultScheme;

	DualsenseControllerScheme(const ControllerScheme& baseScheme, str trimColor, str touchPadColor)
		: ControllerScheme(baseScheme), trim{ trimColor }, touchPad{ touchPadColor } {}

	DualsenseControllerScheme* clone() override { // Here we make use of Covarian Return types
		return new DualsenseControllerScheme(*this);
	}
};

class XboxControllerScheme : public ControllerScheme {
public:
	str batteryCover;

	static XboxControllerScheme defaultScheme;

	XboxControllerScheme(const ControllerScheme& baseScheme, str batteryCoverColor)
		: ControllerScheme(baseScheme), batteryCover{ batteryCoverColor } {}

	XboxControllerScheme* clone() override { // Here we make use of Covariant Return types
		return new XboxControllerScheme(*this); // Default copy constructor since there are no raw pointers, only std::strings. TODO: Implement copy constructor.
	}
};

// Default schemes or "Prototypes" not hardcoded in the Class' constructor. Prototype pattern implemented.
DualsenseControllerScheme DualsenseControllerScheme::defaultScheme(
	ControllerScheme("black", "black", "black", "white", "white", "white", "white", "white", "black"),
	"black",
	"white"
);

XboxControllerScheme XboxControllerScheme::defaultScheme(
	ControllerScheme("black", "black", "black", "black", "black", "black", "black", "black", "black"),
	"black"
);

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

	static T* getDefaultScheme() {
		return T::defaultScheme.clone();
	}

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
		controllerScheme = getDefaultScheme();
	}

	Builder* setTrim(std::string color) { controllerScheme->trim = color; return this; }
	Builder* setTouchPad(std::string color) { controllerScheme->touchPad = color; return this; }
};


class XboxControllerSchemeBuilder : public ControllerSchemeBuilder<XboxControllerScheme> {
public:
	XboxControllerSchemeBuilder() {
		controllerScheme = getDefaultScheme();
	}

	Builder* setBatteryCover(std::string color) { controllerScheme->batteryCover = color; return this; }
};

int main() {
	DualsenseControllerSchemeBuilder* myPSControlBuilder = new DualsenseControllerSchemeBuilder();
	auto controller1 = myPSControlBuilder->setTrim("purple")->build();
	std::cout << controller1->trim << std::endl;

	auto controller2 = myPSControlBuilder->setTrim("green")->build();
	std::cout << controller2->trim << std::endl;

	delete myPSControlBuilder;
	return 0;
}