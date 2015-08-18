#include "TopPanel.h"

const char* TopPanel::_name = "TOP_PANEL_HANDMADE";

TopPanel::~TopPanel() {
	CC_SAFE_RELEASE_NULL(_layerBack);
	CC_SAFE_RELEASE_NULL(_labelNext);
	CC_SAFE_RELEASE_NULL(_labelScore);
	CC_SAFE_RELEASE_NULL(_fieldScore);
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

	_labelNext = Sprite::create(_labelNextPath);
#ifdef _DEBUG
	_labelNext->setScaleX(0.6f);
	_labelNext->setScaleY(0.4f);
	_labelNext->setPosition(335, 27);
#endif
	_labelNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	
	_labelScore = Sprite::create(_labelScorePath);
#ifdef _DEBUG 
	_labelScore->setScaleX(0.6f);
	_labelScore->setScaleY(0.4f);
	_labelScore->setPosition(90, 27);
#endif
	_labelScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	
	_fieldScore = Sprite::create(_fieldScorePath);
#ifdef _DEBUG 
	_fieldScore->setScaleX(0.6f);
	_fieldScore->setScaleY(0.4f);
	_fieldScore->setPosition(90, 3);
#endif
	_fieldScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

#ifdef _DEBUG
	_spritePanel->setOpacity(60);
	_labelNext->setOpacity(60);
	_labelScore->setOpacity(60);
	_fieldScore->setOpacity(60);
#endif
	_layerBack->addChild(_spritePanel);
	_layerBack->addChild(_labelNext);
	_layerBack->addChild(_labelScore);
	_layerBack->addChild(_fieldScore);
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