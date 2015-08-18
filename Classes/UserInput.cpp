#include "UserInput.h"

const char* UserInput::_name = "USER_INPUT_HANDMADE";

UserInput::~UserInput() {
	CC_SAFE_RELEASE_NULL(_layerBack);
	CC_SAFE_RELEASE_NULL(_btnDown);
	CC_SAFE_RELEASE_NULL(_btnLeft);
	CC_SAFE_RELEASE_NULL(_btnRight);
	CC_SAFE_RELEASE_NULL(_btnRotate);
}

UserInput::UserInput(Layer* layer, Size winSize) {
	_layerParent = layer;
	_layerBack = Layer::create();
	_btnDown = Button::create(_downNormalPath, _downPressedPath);
	_btnLeft = Button::create(_leftNormalPath, _leftPressedPath);
	_btnRight = Button::create(_rightNormalPath, _rightPressedPath);
	_btnRotate = Button::create(_rotateNormalPath, _rotatePressedPath);

	_btnDown->addClickEventListener(CC_CALLBACK_1(UserInput::buttonClick, this));
	_btnLeft->addClickEventListener(CC_CALLBACK_1(UserInput::buttonClick, this));
	_btnRight->addClickEventListener(CC_CALLBACK_1(UserInput::buttonClick, this));
	_btnRotate->addClickEventListener(CC_CALLBACK_1(UserInput::buttonClick, this));

	_btnDown->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRotate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

#ifdef _DEBUG
	_btnDown->setScaleX(0.7f);
	_btnDown->setScaleY(0.6f);
	_btnDown->setPosition({ 390, 5 });

	_btnLeft->setScaleX(0.7f);
	_btnLeft->setScaleY(0.6f);
	_btnLeft->setPosition({ 280, 5 });

	_btnRight->setScaleX(0.7f);
	_btnRight->setScaleY(0.6f);
	_btnRight->setPosition({ 180, 5 });

	_btnRotate->setScaleX(0.7f);
	_btnRotate->setScaleY(0.6f);
	_btnRotate->setPosition({ 48, 5 });
#endif

#ifdef _DEBUG
	_btnDown->setOpacity(80);
	_btnLeft->setOpacity(80);
	_btnRight->setOpacity(80);
	_btnRotate->setOpacity(80);
#endif
	_layerBack->addChild(_btnDown);
	_layerBack->addChild(_btnLeft);
	_layerBack->addChild(_btnRight);
	_layerBack->addChild(_btnRotate);
}

void UserInput::buttonClick(Ref* event) {
}

void UserInput::disable(bool flag) {
	_disable = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}

void UserInput::show() {
	_layerParent->addChild(_layerBack);
}