#include "ScoresElementUI.h"

const char* ScoresElementUI::_name = "SCORES_ELEMENT_UI_HANDMADE";

ScoresElementUI::~ScoresElementUI() {
	CC_SAFE_RELEASE_NULL(_labelSpriteScore);
	CC_SAFE_RELEASE_NULL(_fieldScore);
	CC_SAFE_RELEASE_NULL(_labelScores);
}

ScoresElementUI::ScoresElementUI(Layer *layer, Size winSize) {
	_scores = 0;
	_layerParent = layer;
	_layerBack = Layer::create();
	_layerBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	_labelScores = Label::createWithTTF(std::to_string(_scores), _fontPath, 22);
	_labelScores->setColor(Color3B::BLACK);

	_labelSpriteScore = Sprite::create(_labelScorePath);
	_labelSpriteScore->setScaleX(0.6f);
	_labelSpriteScore->setScaleY(0.4f);
	_labelSpriteScore->setPosition(90, 27);
	_labelSpriteScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	_labelScores->setPosition(_labelSpriteScore->getContentSize().width / 2
			, _labelSpriteScore->getContentSize().height / 2);

	_fieldScore = Sprite::create(_fieldScorePath);
	_fieldScore->setScaleX(0.6f);
	_fieldScore->setScaleY(0.4f);
	_fieldScore->setPosition(90, 3);
	_fieldScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

#ifdef _DEBUG
	_labelSpriteScore->setOpacity(60);
	_fieldScore->setOpacity(60);
	//_labelScores->setOpacity(60);
#endif

	_layerBack->addChild(_labelSpriteScore);
	_layerBack->addChild(_fieldScore);
	_fieldScore->addChild(_labelScores);
}

void ScoresElementUI::disable(bool flag) {
	_disable = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}

void ScoresElementUI::show() {
	_layerParent->addChild(_layerBack);
}

void ScoresElementUI::wrapperToUpdateScores(void* ptrObj, void* args) {
	ScoresElementUI *ptrScoresElement = static_cast<ScoresElementUI*>(ptrObj);
	ptrScoresElement->updateScores(args);
}

void ScoresElementUI::updateScores(void *args) {
	_scores += *(static_cast<int*>(args));
	_labelScores->setString(std::to_string(_scores));

	CC_SAFE_DELETE(args);
}