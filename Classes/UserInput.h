#ifndef __USER_INPUT_H__
#define __USER_INPUT_H__

#include <cocos2d.h>
#include <ui\CocosGUI.h>
#include "AbstractElementUI.h"
#include "Command.h"
#include "CommandMoveLeft.h"
#include "CommandMoveRight.h"
#include "CommandMoveCounterClockwise.h"
#include "CommandMoveClockwise.h"
#include "CommandMoveDown.h"

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
	Button *_btnLeft;
	Button *_btnDown;
	Layer *_layerBack;
	Button *_btnRight;
	bool _isKeyPressed;
	Button *_btnRotate;
	Command *_moveLeft;
	Command *_moveDown;
	Command *_moveRight;
	Layer *_layerParent;
	Block *_currentBlock;
	Command *_moveClockwise;
	Command *_moveCounterClockwise;
	EventKeyboard::KeyCode _currentPressedKey;
	std::chrono::system_clock::time_point _startTime;
	
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void onKeyPressed(EventKeyboard::KeyCode, Event*, Block*); //Custom function
	void onKeyReleased(EventKeyboard::KeyCode, Event*);
#endif
};

#endif