#include "TopPanel.h"

const char* TopPanel::_name = "TOP_PANEL_HANDMADE";

TopPanel::TopPanel(Layer *layer, Size winSize) {
	const float scaleX = winSize.width / _realSize.width;
	const float scaleY = 55 / _realSize.height;

	_layerParent = layer;
	_layerBack = Layer::create();

	_layerBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_layerBack->setScaleX(scaleX);
	_layerBack->setScaleY(scaleY);
	_layerBack->setContentSize(_realSize);
	_layerBack->setPosition(0, (_realBottomMargin * scaleY) + (_realSize.height * scaleY));

	_spritePanel = Sprite::create(_panelPath);
	_spritePanel->setScaleX(_layerBack->getContentSize().width / _spritePanel->getContentSize().width);
	_spritePanel->setScaleY(_layerBack->getContentSize().height / _spritePanel->getContentSize().height);
	_spritePanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spritePanel->setPosition(0, 0);

	_labelNext = Sprite::create(_labelNextPath);
	_labelNext->setScaleX(0.02f);
	_labelNext->setScaleY(0.01f);
	_labelNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_labelNext->setPosition(12.2f, 0.8f);

	_labelScore = Sprite::create(_labelScorePath);
	_labelScore->setScaleX(0.02f);
	_labelScore->setScaleY(0.01f);
	_labelScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_labelScore->setPosition(1.6f, 0.8f);

	_fieldScore = Sprite::create(_fieldScorePath);
	_fieldScore->setScaleX(0.02f);
	_fieldScore->setScaleY(0.01f);
	_fieldScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_fieldScore->setPosition(1.6f, 0.2f);

	_layerBack->addChild(_spritePanel);
	_layerBack->addChild(_labelNext);
	_layerBack->addChild(_labelScore);
	_layerBack->addChild(_fieldScore);
}

TopPanel::~TopPanel() {
	CC_SAFE_RELEASE_NULL(_layerBack);
	CC_SAFE_RELEASE_NULL(_labelNext);
	CC_SAFE_RELEASE_NULL(_labelScore);
	CC_SAFE_RELEASE_NULL(_fieldScore);
	CC_SAFE_RELEASE_NULL(_spritePanel);
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