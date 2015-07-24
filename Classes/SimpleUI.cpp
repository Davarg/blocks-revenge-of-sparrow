#include "SimpleUI.h"
#include "BackgroundElementUI.h"

SimpleUI::~SimpleUI() {
	for (auto iterator : _childrens) {
		CC_SAFE_DELETE(iterator);
	}
	//CC_SAFE_RELEASE_NULL(_layer);Throw error
	_childrens.clear();
	
}

SimpleUI::SimpleUI(Layer* layer) {
	_layer = layer;
	Vec2 backPos = { 5, 3 };
	BackgroundElementUI *beui = new BackgroundElementUI(layer, backPos);
	_childrens.push_back(beui);
}

void SimpleUI::show(){
	for (auto iterator : _childrens) {
		iterator->show();
	}
}

std::vector<AbstractElementUI*> SimpleUI::getChildrens(){
	return _childrens;
}

AbstractElementUI* SimpleUI::getChildrenByName(const char* name) {
	for (auto iterator : _childrens) {
		if (iterator->getName() == name)
			return iterator;
	}
	return nullptr;
}