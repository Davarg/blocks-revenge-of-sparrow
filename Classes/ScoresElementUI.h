#ifndef __SCORES_ELEMENT_UI_H__
#define __SCORES_ELEMENT_UI_H__

#include <cocos2d.h>
#include "AbstractElementUI.h"

USING_NS_CC;

class ScoresElementUI : public AbstractElementUI {
private:
	static const char* _name;
	const char* _labelScorePath = "ui/label_score.png";
	const char* _fieldScorePath = "ui/field_score.png";
	const char* _fontPath = "century_gothic_regular.ttf";

	Sprite *_labelSpriteScore;
	Sprite *_fieldScore;
	Label *_labelScores;
	Layer *_layerParent;
	Layer *_layerBack;
	bool _disable;
	int _scores;

public:
	ScoresElementUI(Layer*, Size);
	~ScoresElementUI();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	Layer* getLayer() { return _layerBack; }
	virtual void disable(bool) override;
	virtual void show() override;
	void updateScores(void*);

	static const char* name() { return _name; }
};

#endif