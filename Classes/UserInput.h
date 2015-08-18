#ifndef __USER_INPUT_H__
#define __USER_INPUT_H__

#include <cocos2d.h>
#include <ui\CocosGUI.h>
#include "AbstractElementUI.h"

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
	Button *_btnRotate;
	Layer *_layerParent;

public:
	UserInput(Layer*, Size);
	~UserInput();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	Layer* getLayer() { return _layerBack; }
	virtual void disable(bool) override;
	virtual void show() override;
	void buttonClick(Ref*);
};

#endif