#ifndef __USER_INPUT_H__
#define __USER_INPUT_H__

#include <cocos2d.h>
#include "Command.h"
#include <ui\CocosGUI.h>
#include "CommandMoveLeft.h"
#include "CommandMoveDown.h"
#include "CommandMoveRight.h"
#include "AbstractElementUI.h"
#include "CommandMoveCounterClockwise.h"

USING_NS_CC;
using namespace ui;

class UserInput : public AbstractElementUI {
private:
	static const char* _name;
	const char* _leftNormalPath = "btns/left_up.png";
	const char* _downNormalPath = "btns/down_up.png";
	const char* _rightNormalPath = "btns/right_up.png";
	const char* _rotateNormalPath = "btns/rotate_up.png";

	const char* _leftPressedPath = "btns/left_down.png";
	const char* _downPressedPath = "btns/down_down.png";
	const char* _rightPressedPath = "btns/right_down.png";
	const char* _rotatePressedPath = "btns/rotate_down.png";

	bool _disable;
	Layer *_layerBack;
	bool _isKeyPressed;
	Command *_moveLeft;
	Command *_moveDown;
	Command *_moveRight;
	Layer *_layerParent;
	Block *_currentBlock;
	Command *_moveCounterClockwise;
	std::chrono::system_clock::time_point _startTime;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Button *_btnLeft;
	Button *_btnDown;
	Button *_btnRight;
	Button *_btnRotate;

	enum buttonsTags {
		DOWN,
		LEFT,
		RIGHT,
		ROTATE
	};
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Sprite *_btnLeft;
	Sprite *_btnDown;
	Sprite *_btnRight;
	Sprite *_btnRotate;

	Sprite *_btnLeftPressed;
	Sprite *_btnDownPressed;
	Sprite *_btnRightPressed;
	Sprite *_btnRotatePressed;

	EventKeyboard::KeyCode _currentPressedKey;
#endif 

public:
	UserInput(Layer*, Size);
	~UserInput();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	static const char* name() { return _name; }
	Layer* getLayer() { return _layerBack; }
	virtual void disable(bool) override;
	virtual void show() override;
	void dropInputEvents();
	void update(float);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void onKeyPressed(EventKeyboard::KeyCode, Event*);
	void onKeyReleased(EventKeyboard::KeyCode, Event*);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void onTouch(Ref*, Widget::TouchEventType);
#endif
};

#endif