#include "BottomPanel.h"

const char* BottomPanel::_name = "BOTTOM_PANEL_HANDMADE";

BottomPanel::~BottomPanel() {
	CC_SAFE_RELEASE_NULL(_layerBack);
	CC_SAFE_RELEASE_NULL(_spritePanel);
}

BottomPanel::BottomPanel(Layer *layer, Size winSize) {
	const float scaleX = winSize.width / _realSize.width;
	const float scaleY = 55 / _realSize.height;

	_layerParent = layer;
	_layerBack = Layer::create();
	_spritePanel = Sprite::create(_panelPath);

	_layerBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_layerBack->setScaleX(scaleX);
	_layerBack->setScaleY(scaleY);
	_layerBack->setContentSize(_realSize);
	_layerBack->setPosition(0, 0);

	_spritePanel->setScaleX(0.0281f);
	_spritePanel->setScaleY(0.036f);
	_spritePanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spritePanel->setPosition(0, 0);

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