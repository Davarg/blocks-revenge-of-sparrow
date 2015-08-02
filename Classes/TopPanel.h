#ifndef __TOP_PANEL_H__
#define __TOP_PANEL_H__

#include <cocos2d.h>
#include "AbstractElementUI.h"

USING_NS_CC;

class TopPanel : public AbstractElementUI {
private:
	static const char* _name;
	const Size _realSize = { 16, 1.5f };
	const float _realBottomMargin = 20.3f;
	const char* _panelPath = "../Resources/ui/top.png";
	const char* _labelNextPath = "../Resources/ui/label_next.png";
	const char* _labelScorePath = "../Resources/ui/label_score.png";
	const char* _fieldScorePath = "../Resources/ui/field_score.png";

	bool _disable;
	Layer *_layerBack;
	Sprite *_labelNext;
	Sprite *_labelScore;
	Layer *_layerParent;
	Sprite *_fieldScore;
	Sprite *_spritePanel;

public:
	TopPanel(Layer*, Size);
	~TopPanel();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	virtual void disable(bool) override;
	virtual void show() override;

	static const char* name() { return _name; }
};

#endif