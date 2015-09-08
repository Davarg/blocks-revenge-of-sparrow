#include "TopPanel.h"

const char* TopPanel::_name = "TOP_PANEL_HANDMADE";

TopPanel::~TopPanel() {
	CC_SAFE_RELEASE_NULL(_layerBack);
	CC_SAFE_RELEASE_NULL(_spritePanel);
}

TopPanel::TopPanel(Layer *layer, Size winSize) {
	_layerParent = layer;
	_layerBack = Layer::create();
	_layerBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
#ifdef _DEBUG //Need to properly calculate position on screen size, avoid absolute position
	_layerBack->setPosition(0, 745);
#endif

	_spritePanel = Sprite::create(_panelPath);
	_spritePanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spritePanel->setPosition(Vec2::ZERO);

#ifdef _DEBUG
	_spritePanel->setOpacity(60);
#endif
	_layerBack->addChild(_spritePanel);
}

void TopPanel::disable(bool flag) {
	_disable = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}

void TopPanel::show() {
	_layerParent->addChild(_layerBack);
}