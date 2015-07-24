#ifndef __ABSTRACT_ELEMENT_UI_H__
#define __ABSTRACT_ELEMENT_UI_H__

class AbstractElementUI {
public:
	virtual const char* getName() const = 0;
	virtual bool isDisable() const = 0;
	virtual void disable(bool) = 0;
	virtual void show() = 0;
};

#endif