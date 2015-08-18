#ifndef __BOTTOM_PANEL_H__
#define __BOTTOM_PANEL_H__

#include <cocos2d.h>
#include "AbstractElementUI.h"

USING_NS_CC;

class BottomPanel : public AbstractElementUI {
private:
	static const char* _name;
	const char* _panelPath = "ui/bottom.png";

	bool _disable;
	Layer *_layerBack;
	Layer *_layerParent;
	Sprite *_spritePanel;

public:
	BottomPanel(Layer*, Size);
	~BottomPanel();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	Layer* getLayer() { return _layerBack; }
	virtual void disable(bool) override;
	virtual void show() override;

	static const char* name() { return _name; }
};

#endif