#include "NextBlockElementUI.h"

const char* NextBlockElementUI::_name = "NEXT_BLOCK_ELEMENT_UI_HANDMADE";

NextBlockElementUI::~NextBlockElementUI() {
	CC_SAFE_RELEASE_NULL(_labelSpriteNext);
}

NextBlockElementUI::NextBlockElementUI(Layer *layer, Size winSize) {
	_currentBlock = nullptr;
	_layerParent = layer;
	_layerBack = Layer::create();
	_layerBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	_labelSpriteNext = Sprite::create(_labelNextPath);
	_labelSpriteNext->setScaleX(0.6f);
	_labelSpriteNext->setScaleY(0.4f);
	_labelSpriteNext->setPosition(333, 27);
	_labelSpriteNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
#ifdef _DEBUG
	_labelSpriteNext->setOpacity(60);
#endif

	_layerBack->addChild(_labelSpriteNext);
}

Block* NextBlockElementUI::getBlock() {
	if (_currentBlock != nullptr) {
		_currentBlock = Block::createBlock(_nextBlock);
		_nextBlock = Block::generateBlockInfo();
	}
	else {
		_currentBlock = Block::createBlock(Block::generateBlockInfo());
		_nextBlock = Block::generateBlockInfo();
	}


	Layer *layerNextBlocksSprites = Layer::create();
	layerNextBlocksSprites->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	layerNextBlocksSprites->setName(_layerNextBlocksName);
	auto spriteA = Sprite::create(_nextBlock.spritePathFirst);
	auto spriteB = Sprite::create(_nextBlock.spritePathSecond);

	spriteA->setScale(0.4f);
	spriteB->setScale(0.4f);
	layerNextBlocksSprites->setPosition(355, 15);
	spriteB->setPosition(23, spriteA->getPositionY());
#ifdef _DEBUG
	spriteA->setOpacity(60);
	spriteB->setOpacity(60);
#endif
	layerNextBlocksSprites->addChild(spriteA);
	layerNextBlocksSprites->addChild(spriteB);

	_layerBack->removeChildByName(_layerNextBlocksName);
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