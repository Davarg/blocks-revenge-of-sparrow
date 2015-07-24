#ifndef __BACKGROUND_ELEMENT_UI_H__
#define __BACKGROUND_ELEMENT_UI_H__

#include "AbstractElementUI.h"
#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class BackgroundElementUI : public AbstractElementUI {
private:
	static const char* _name;
	const char* _glassPath = "../Resources/glass.png";

	Layer *_layer;
	bool _disable;
	b2Body *_bodyGlass;
	Sprite *_spriteGlass;

	void createGlass();

public:
	BackgroundElementUI(Layer *layer, Vec2 pos);
	~BackgroundElementUI();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	virtual void disable(bool flag) override;
	virtual void show() override;

	static const char* name() { return _name; }

	Sprite* getSprite() const { return _spriteGlass; }
};

#endif