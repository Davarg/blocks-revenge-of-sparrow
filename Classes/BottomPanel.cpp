#include "BottomPanel.h"

const char* BottomPanel::_name = "BOTTOM_PANEL_HANDMADE";

BottomPanel::~BottomPanel() {
	CC_SAFE_RELEASE_NULL(_layerBack);
	CC_SAFE_RELEASE_NULL(_spritePanel);
}

BottomPanel::BottomPanel(Layer *layer, Size winSize) {
	_layerParent = layer;
	_layerBack = Layer::create();
	_spritePanel = Sprite::create(_panelPath);

	_layerBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_layerBack->setPosition(Vec2::ZERO);

	_spritePanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spritePanel->setPosition(Vec2::ZERO);

#ifdef _DEBUG
	_spritePanel->setOpacity(60);
#endif 
	_layerBack->addChild(_spritePanel);
}

void BottomPanel::disable(bool flag) {
	_disable = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}

void BottomPanel::show() {
	_layerParent->addChild(_layerBack);
}