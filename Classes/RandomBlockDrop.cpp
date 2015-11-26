#include "SimpleUI.h"
#include "MainGameScene.h"
#include "RandomBlockDrop.h"
#include "NextBlockElementUI.h"
#include "BackgroundElementUI.h"

Block* RandomBlockDrop::dropBlock() {
	Block *result = nullptr;
	SimpleUI *simpleUI = MainGameScene::getUI();
	NextBlockElementUI *nbeui = (NextBlockElementUI*)simpleUI->getChildrenByName(NextBlockElementUI::name());
	BackgroundElementUI *beui = (BackgroundElementUI*)simpleUI->getChildrenByName(BackgroundElementUI::name());
	b2Vec2 gameFieldSize = GameField::getSize();
	std::mt19937 generator(time(0));
	std::uniform_int_distribution<> uid(0, gameFieldSize.x - 2);

	result = nbeui->getBlock();
	Vec2 pos = { beui->getAnimatedPosition().x 
					+ (uid(generator) * result->getSprite()->getContentSize().width), 740 };
	if (result) {
		result->setPositionInPxl(pos);
		beui->getLayer()->addChild(result->getSprite());

		result->getAttachedBlock()->setPositionInPxl({ result->getSprite()->getContentSize().width
				+ result->getSprite()->getPositionX()
			, result->getSprite()->getPositionY() });
		beui->getLayer()->addChild(result->getAttachedBlock()->getSprite());
	}

	return result;
}