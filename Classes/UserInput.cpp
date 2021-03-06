#include "UserInput.h"
#include "MainGameScene.h"

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
	CC_SAFE_RELEASE_NULL(_moveDown);
}

UserInput::UserInput(Layer* layer, Size winSize) {
	_disable = false;
	_layerParent = layer;
	_currentBlock = nullptr;
	_layerBack = Layer::create();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	_btnDown = Button::create(_downNormalPath, _downPressedPath);
	_btnLeft = Button::create(_leftNormalPath, _leftPressedPath);
	_btnRight = Button::create(_rightNormalPath, _rightPressedPath);
	_btnRotate = Button::create(_rotateNormalPath, _rotatePressedPath);

	_btnDown->addTouchEventListener(CC_CALLBACK_2(UserInput::onTouch, this));
	_btnLeft->addTouchEventListener(CC_CALLBACK_2(UserInput::onTouch, this));
	_btnRight->addTouchEventListener(CC_CALLBACK_2(UserInput::onTouch, this));
	_btnRotate->addTouchEventListener(CC_CALLBACK_2(UserInput::onTouch, this));

	_btnDown->setTag(buttonsTags::DOWN);
	_btnLeft->setTag(buttonsTags::LEFT);
	_btnRight->setTag(buttonsTags::RIGHT);
	_btnRotate->setTag(buttonsTags::ROTATE);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_btnDown = Sprite::create(_downNormalPath);
	_btnLeft = Sprite::create(_leftNormalPath);
	_btnRight = Sprite::create(_rightNormalPath);
	_btnRotate = Sprite::create(_rotateNormalPath);

	_btnLeftPressed = Sprite::create(_leftPressedPath);
	_btnDownPressed = Sprite::create(_downPressedPath);
	_btnRightPressed = Sprite::create(_rightPressedPath);
	_btnRotatePressed = Sprite::create(_rotatePressedPath);

	_btnLeftPressed->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnDownPressed->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRightPressed->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRotatePressed->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	_btnDownPressed->setLocalZOrder(-1);
	_btnDownPressed->setScaleX(0.7f);
	_btnDownPressed->setScaleY(0.6f);
	_btnDownPressed->setPosition({ 390, 5 });

	_btnLeftPressed->setLocalZOrder(-1);
	_btnLeftPressed->setScaleX(0.7f);
	_btnLeftPressed->setScaleY(0.6f);
	_btnLeftPressed->setPosition({ 180, 5 });

	_btnRightPressed->setLocalZOrder(-1);
	_btnRightPressed->setScaleX(0.7f);
	_btnRightPressed->setScaleY(0.6f);
	_btnRightPressed->setPosition({ 280, 5 });

	_btnRotatePressed->setLocalZOrder(-1);
	_btnRotatePressed->setScaleX(0.7f);
	_btnRotatePressed->setScaleY(0.6f);
	_btnRotatePressed->setPosition({ 48, 5 });

	_layerBack->addChild(_btnDownPressed);
	_layerBack->addChild(_btnLeftPressed);
	_layerBack->addChild(_btnRightPressed);
	_layerBack->addChild(_btnRotatePressed);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(UserInput::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(UserInput::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

#ifdef _DEBUG
	_btnLeftPressed->setOpacity(80);
	_btnDownPressed->setOpacity(80);
	_btnRightPressed->setOpacity(80);
	_btnRotatePressed->setOpacity(80);
#endif
#endif

	_btnDown->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_btnRotate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	_moveLeft = CommandMoveLeft::create();
	_moveDown = CommandMoveDown::create();
	_moveRight = CommandMoveRight::create();
	_moveCounterClockwise = CommandMoveCounterClockwise::create();

	if (_moveLeft && _moveRight && _moveCounterClockwise && _moveDown) {
		CC_SAFE_RETAIN(_moveLeft);
		CC_SAFE_RETAIN(_moveDown);
		CC_SAFE_RETAIN(_moveRight);
		CC_SAFE_RETAIN(_moveCounterClockwise);
	}
	
	_btnDown->setScaleX(0.7f);
	_btnDown->setScaleY(0.6f);
	_btnDown->setPosition({ 390, 5 });

	_btnLeft->setScaleX(0.7f);
	_btnLeft->setScaleY(0.6f);
	_btnLeft->setPosition({ 180, 5 });

	_btnRight->setScaleX(0.7f);
	_btnRight->setScaleY(0.6f);
	_btnRight->setPosition({ 280, 5 });

	_btnRotate->setScaleX(0.7f);
	_btnRotate->setScaleY(0.6f);
	_btnRotate->setPosition({ 48, 5 });

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
	using namespace std::chrono;
	if (_isKeyPressed) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		if (_currentPressedKey == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			auto mls = duration_cast<milliseconds>(high_resolution_clock::now() - _startTime);
			if (mls.count() >= 350) {
				onKeyPressed(_currentPressedKey, nullptr);
				_startTime = high_resolution_clock::now();
			}
		}
		else
			onKeyPressed(_currentPressedKey, nullptr);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if (_currentPressedButton->getTag() == buttonsTags::ROTATE) {
			auto mls = duration_cast<milliseconds>(high_resolution_clock::now() - _startTime);
			if (mls.count() >= 350)
				onTouch(_currentPressedButton, Widget::TouchEventType::BEGAN);
		}
		else
			onTouch(_currentPressedButton, Widget::TouchEventType::BEGAN);
#endif
	}
}

void UserInput::dropInputEvents() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	switch (_currentPressedKey) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		_btnLeft->setVisible(true);
		_btnLeftPressed->setLocalZOrder(-1);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_btnRight->setVisible(true);
		_btnRightPressed->setLocalZOrder(-1);
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		_btnRotate->setVisible(true);
		_btnRotatePressed->setLocalZOrder(-1);
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		_btnDown->setVisible(true);
		_btnDownPressed->setLocalZOrder(-1);
		break;
	}
	_currentPressedKey = EventKeyboard::KeyCode::KEY_F9;
#endif

	_isKeyPressed = false;
	_currentBlock = nullptr;

	bool isBodyNeedToBeStopped = false;
	if (_moveDown->isExecute())
		((CommandMoveDown*)_moveDown)->stopBlock(isBodyNeedToBeStopped);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void UserInput::onTouch(Ref* ref, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN) {
			Button *btn = (Button*)ref;
			_currentPressedButton = (Button*)ref;
			_startTime = std::chrono::high_resolution_clock::now();
			_isKeyPressed = true;
			_currentBlock = MainGameScene::getCurrentBlock();

			switch (btn->getTag()) {
			case buttonsTags::DOWN:
				if (!_moveDown->isExecute())
					_moveDown->execute(_currentBlock);
				break;

			case buttonsTags::LEFT:
				if (!_moveRight->isExecute()) {
					if (!_moveLeft->isExecute()) {
						_moveLeft->execute(_currentBlock);
						Director::getInstance()->getScheduler()->scheduleUpdate(_moveLeft, 3, false);
					}
				}
				else
					_moveRight->undo();
				break;

			case buttonsTags::RIGHT:
				if (!_moveLeft->isExecute()) {
					if (!_moveRight->isExecute()) {
						_moveRight->execute(_currentBlock);
						Director::getInstance()->getScheduler()->scheduleUpdate(_moveRight, 3, false);
					}
				}
				else
					_moveLeft->undo();
				break;

			case buttonsTags::ROTATE:
				if (!_moveCounterClockwise->isExecute()) {
					_moveCounterClockwise->execute(_currentBlock);
					Director::getInstance()->getScheduler()->scheduleUpdate(_moveCounterClockwise, 3, false);
				}
				break;
			}
		}
		else if (type == Widget::TouchEventType::ENDED) {
			_isKeyPressed = false;
			_currentBlock = nullptr;
			if (_moveDown->isExecute())
				((CommandMoveDown*)_moveDown)->stopBlock();
		}
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void UserInput::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
		if (_currentPressedKey == keyCode) {
			switch (_currentPressedKey) {
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				_btnLeft->setVisible(true);
				_btnLeftPressed->setLocalZOrder(-1);
				break;

			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				_btnRight->setVisible(true);
				_btnRightPressed->setLocalZOrder(-1);
				break;

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				_btnRotate->setVisible(true);
				_btnRotatePressed->setLocalZOrder(-1);
				break;

			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				_btnDown->setVisible(true);
				_btnDownPressed->setLocalZOrder(-1);
				break;
			}

			_isKeyPressed = false;
			_currentBlock = nullptr;
			if (_moveDown->isExecute())
				((CommandMoveDown*)_moveDown)->stopBlock();
		}
	}

	void UserInput::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
		_startTime = std::chrono::high_resolution_clock::now();
		_isKeyPressed = true;
		_currentBlock = MainGameScene::getCurrentBlock();
		_currentPressedKey = keyCode;
		
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (!_moveRight->isExecute()) {
				if (!_moveLeft->isExecute()) {
					_btnLeft->setVisible(false);
					_btnLeftPressed->setLocalZOrder(1);
					_moveLeft->execute(_currentBlock);
					Director::getInstance()->getScheduler()->scheduleUpdate(_moveLeft, 3, false);
				}
			}
			else
				_moveRight->undo();
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (!_moveLeft->isExecute()) {
				if (!_moveRight->isExecute()) {
					_btnRight->setVisible(false);
					_btnRightPressed->setLocalZOrder(1);
					_moveRight->execute(_currentBlock);
					Director::getInstance()->getScheduler()->scheduleUpdate(_moveRight, 3, false);
				}
			}
			else
				_moveLeft->undo();
			break;

		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (!_moveCounterClockwise->isExecute()) {
				_btnRotate->setVisible(false);
				_btnRotatePressed->setLocalZOrder(1);
				_moveCounterClockwise->execute(_currentBlock);
				Director::getInstance()->getScheduler()->scheduleUpdate(_moveCounterClockwise, 3, false);
			}
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			if (!_moveDown->isExecute()) {
				_btnDown->setVisible(false);
				_btnDownPressed->setLocalZOrder(1);
				_moveDown->execute(_currentBlock);
			}
			break;
		}
	}
#endif
