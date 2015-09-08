#include "NextBlockElementUI.h"

const char* NextBlockElementUI::_name = "NEXT_BLOCK_ELEMENT_UI_HANDMADE";

NextBlockElementUI::~NextBlockElementUI() {
	CC_SAFE_RELEASE_NULL(_labelSpriteNext);
}

NextBlockElementUI::NextBlockElementUI(Layer *layer, Size winSize) {
	_currentBlock = nullptr;
	_nextBlock = nullptr;
	_layerParent = layer;
	_layerBack = Layer::create();
	_layerBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	_labelSpriteNext = Sprite::create(_labelNextPath);
#ifdef _DEBUG
	_labelSpriteNext->setScaleX(0.6f);
	_labelSpriteNext->setScaleY(0.4f);
	_labelSpriteNext->setPosition(335, 27);
	_labelSpriteNext->setOpacity(60);
#endif
	_labelSpriteNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	_layerBack->addChild(_labelSpriteNext);
}

Block* NextBlockElementUI::getBlock() {
	if (_currentBlock != nullptr) {
		_currentBlock = _nextBlock;
		_nextBlock = Block::generateBlock();
		show();
	}
	else {
		_currentBlock = Block::generateBlock();
		_nextBlock = Block::generateBlock();
	}
	//BUG, Need to safely disable body or remove it (_nextBlock)
	Layer *layerNextBlocksSprites = Layer::create();
	layerNextBlocksSprites->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	auto spriteA = _nextBlock->getSprite();
	auto spriteB = _nextBlock->getAttachedBlock()->getSprite();
#ifdef _DEBUG
	//layerNextBlocksSprites->setScaleX(0.6f);
	//layerNextBlocksSprites->setScaleY(0.4f);
	layerNextBlocksSprites->setPosition(335, 20);
	layerNextBlocksSprites->setOpacity(60);
#endif
	layerNextBlocksSprites->addChild(spriteA);
	layerNextBlocksSprites->addChild(spriteB);
	_layerBack->addChild(layerNextBlocksSprites);

	return _currentBlock;
}

void NextBlockElementUI::disable(bool flag) {
	_disable = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}

void NextBlockElementUI::show() {
	_layerParent->addChild(_layerBack);
}