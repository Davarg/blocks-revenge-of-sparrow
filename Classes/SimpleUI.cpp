#include "SimpleUI.h"
#include "BackgroundElementUI.h"
#include "TopPanel.h"
#include "BottomPanel.h"
#include "UserInput.h"

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

	BottomPanel *bp = new BottomPanel(layer, winSize);
	_childrens.push_back(bp);

	UserInput *ui = new UserInput(bp->getLayer(), bp->getLayer()->getContentSize());
	_childrens.push_back(ui);
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