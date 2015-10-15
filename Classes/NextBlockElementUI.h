#ifndef __NEXT_BLOCK_ELEMENT_UI_H__
#define __NEXT_BLOCK_ELEMENT_UI_H__

#include "Block.h"
#include <cocos2d.h>
#include "AbstractElementUI.h"

USING_NS_CC;

class NextBlockElementUI : public AbstractElementUI {
private:
	static const char* _name;
	const char* _labelNextPath = "ui/label_next.png";
	const char* _layerNextBlocksName = "nextBlockLayer";

	Block::blockInfo _nextBlock;
	Sprite *_labelSpriteNext;
	Block *_currentBlock;
	Layer *_layerParent;
	Layer *_layerBack;
	bool _disable;

public:
	NextBlockElementUI(Layer*, Size);
	~NextBlockElementUI();

	virtual const char* getName() const override { return _name; }
	virtual bool isDisable() const override { return _disable; }
	Layer* getLayer() { return _layerBack; }
	virtual void disable(bool) override;
	virtual void show() override;
	Block* getBlock();

	static const char* name() { return _name; }
};

#endif