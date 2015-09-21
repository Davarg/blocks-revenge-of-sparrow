#include "UserInput.h"

const char* UserInput::_name = "USER_INPUT_HANDMADE";

UserInput::~UserInput() {
	CC_SAFE_RELEASE_NULL(_layerBack);
	CC_SAFE_RELEASE_NULL(_btnDown);
	CC_SAFE_RELEASE_NULL(_btnLeft);
	CC_SAFE_RELEASE_NULL(_btnRight);
	CC_SAFE_RELEASE_NULL(_btnRotate);
	CC_SAFE_RELEASE_NULL(_moveLeft);
	CC_SAFE_RELEASE_NULL(_moveRight);
	CC_SAFE_RELEASE_NULL(_moveCounterClockwise);
	CC_SAFE_RELEASE_NULL(_moveClockwise);
	CC_SAFE_RELEASE_NULL(_moveDown);
}

UserInput::UserInput(Layer* layer, Size winSize) {
	_layerParent = layer;
	_layerBack = Layer::create();
	_btnDown = Button::create(_downNormalPath, _downPressedPath);
	_btnLeft = Button::create(_leftNormalPath, _leftPressedPath);
	_btnRight = Button::create(_rightNormalPath, _rightPressedPath);
	_btnRotate = Button::create(_rotateNormalPath, _rotatePressedPath);

	_btnDown->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRotate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	_moveLeft = CommandMoveLeft::create();
	_moveRight = CommandMoveRight::create();
	_moveCounterClockwise = CommandMoveCounterClockwise::create();

	if (_moveLeft && _moveRight && _moveCounterClockwise) {
		CC_SAFE_RETAIN(_moveLeft);
		CC_SAFE_RETAIN(_moveRight);
		CC_SAFE_RETAIN(_moveCounterClockwise);
	}
	_moveClockwise = nullptr;//new CommandMoveClockwise(); //Memory Leak
	_moveDown = nullptr;//CommandMoveDown::create();
	
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

	Director::getInstance()->getScheduler()->scheduleUpdate(this, 4, false);
	_isKeyPressed = false;
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

void UserInput::update(float dt) {
	/*if (_isKeyPressed)
		onKeyPressed(_currentPressedKey, nullptr, _currentBlock);*/
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void UserInput::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
		if (_currentPressedKey == keyCode) {
			_isKeyPressed = false;
			_currentBlock = nullptr;
		}
	}

	void UserInput::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event, Block* currentBlock) {
		_isKeyPressed = true;
		_currentBlock = currentBlock;
		_currentPressedKey = keyCode;
		
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (!_moveRight->isExecute()) {
				if (!_moveLeft->isExecute()) {
					_moveLeft->execute(currentBlock);
					Director::getInstance()->getScheduler()->scheduleUpdate(_moveLeft, 3, false);
				}
			}
			else
				_moveRight->undo();
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (!_moveLeft->isExecute()) {
				if (!_moveRight->isExecute()) {
					_moveRight->execute(currentBlock);
					Director::getInstance()->getScheduler()->scheduleUpdate(_moveRight, 3, false);
				}
			}
			else
				_moveLeft->undo();
			break;

		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (!_moveCounterClockwise->isExecute()) {
				_moveCounterClockwise->execute(currentBlock);
				Director::getInstance()->getScheduler()->scheduleUpdate(_moveCounterClockwise, 3, false);
			}
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			/*if (!_moveDown->isExecute()) {
			_moveDown->execute(_currentBlock);
			Director::getInstance()->getScheduler()->scheduleUpdate(_moveDown, 3, false);
			}*/
			break;
		}
	}
#endif