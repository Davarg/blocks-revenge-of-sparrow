#ifndef __SIMPLE_UI_H__
#define __SIMPLE_UI_H__

#include "AbstractUI.h"
#include "cocos2d.h"

USING_NS_CC;

class SimpleUI : public AbstractUI {
private:
	std::vector<AbstractElementUI*> _childrens;
	Layer *_layer;

public:
	SimpleUI(Layer*);
	~SimpleUI();

	virtual void show() override;
	virtual std::vector<AbstractElementUI*> getChildrens();
	virtual AbstractElementUI* getChildrenByName(const char*) override;
};

#endif