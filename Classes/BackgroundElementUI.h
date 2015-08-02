#ifndef __BACKGROUND_ELEMENT_UI_H__
#define __BACKGROUND_ELEMENT_UI_H__

#include "AbstractElementUI.h"
#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class BackgroundElementUI : public AbstractElementUI {
private:
	static const char* _name;
	const int numAnimFiles = 18;
	const float _realBottomMargin = 1;
	const Size _realSize = { 16, 21 };
	const char* _glassPath = "../Resources/ui/back.png";
	const char* _backAnimFolderPath = "../Resources/ui/back_anim";

	bool _disable;
	Layer *_layerBack;
	b2Body *_bodyGlass;
	Layer *_layerParent;
	Sprite *_spriteGlass;

	void createGlass();

public:
	BackgroundElementUI(Layer*, Size);
	~BackgroundElementUI();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	virtual void disable(bool flag) override;
	virtual void show() override;
	Size getUserSize() const;

	static const char* name() { return _name; }
};

#endif