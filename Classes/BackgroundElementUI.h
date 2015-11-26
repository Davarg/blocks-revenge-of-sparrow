#ifndef __BACKGROUND_ELEMENT_UI_H__
#define __BACKGROUND_ELEMENT_UI_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>
#include "AbstractElementUI.h"

USING_NS_CC;

class BackgroundElementUI : public AbstractElementUI {
private:
	static const char* _name;
	const int _numAnimFiles = 18;
	const int _tagBackground = 0;
	const char* _glassPath = "ui/back.png";
	const char* _animPathFolder = "ui/back_anim/";

	bool _disable;
	Layer *_layerBack;
	b2Body *_bodyGlass;
	Layer *_layerParent;
	Sprite *_spriteGlass;
	Sprite *_animatedSprite;

	void createGlass();

public:
	BackgroundElementUI(Layer*, Size);
	~BackgroundElementUI();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	Sprite* getAnimatedSprite() { return _animatedSprite; }
	static const char* name() { return _name; }
	Layer* getLayer() { return _layerBack; }
	virtual void disable(bool) override;
	Vec2 getAnimatedPosition() const;
	virtual void show() override;
	Size getUserSize() const;
};

#endif