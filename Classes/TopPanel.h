#ifndef __TOP_PANEL_H__
#define __TOP_PANEL_H__

#include <cocos2d.h>
#include "AbstractElementUI.h"

USING_NS_CC;

class TopPanel : public AbstractElementUI {
private:
	static const char* _name;
	const char* _panelPath = "ui/top.png";

	bool _disable;
	Layer *_layerBack;
	Layer *_layerParent;
	Sprite *_spritePanel;

public:
	TopPanel(Layer*, Size);
	~TopPanel();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	Layer* getLayer() { return _layerBack; }
	virtual void disable(bool) override;
	virtual void show() override;

	static const char* name() { return _name; }
};

#endif