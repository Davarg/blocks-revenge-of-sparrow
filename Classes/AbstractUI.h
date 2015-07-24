#ifndef __ABSTRACT_UI_H__
#define __ABSTRACT_UI_H__

#include "AbstractElementUI.h"

class AbstractUI {
public:
	virtual void show() = 0;
	virtual AbstractElementUI* getChildrenByName(const char*) = 0;
};

#endif