#include "SimpleUI.h"
#include "BackgroundElementUI.h"
#include "TopPanel.h"

SimpleUI::~SimpleUI() {
	for (auto iterator : _childrens) {
		CC_SAFE_DELETE(iterator);
	}
	//CC_SAFE_RELEASE_NULL(_layer);Throw error
	_childrens.clear();
	
}

SimpleUI::SimpleUI(Layer* layer) {
	const Size winSize = Director::sharedDirector()->getVisibleSize();
	_layer = layer;
	
	BackgroundElementUI *beui = new BackgroundElementUI(layer, winSize);
	_childrens.push_back(beui);

	TopPanel *tp = new TopPanel(layer, winSize);
	_childrens.push_back(tp);
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