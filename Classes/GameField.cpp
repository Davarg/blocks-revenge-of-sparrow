#include "SimpleUI.h"
#include "UserInput.h"
#include "GameField.h"
#include "MessagesQueue.h"
#include "MainGameScene.h"

Block ***_arrayBlocks = nullptr;
int GameField::_fieldWidth = 0;
int GameField::_fieldHeight = 0;

GameField::~GameField() {
	if (_arrayBlocks) {
		for (int i = 0; i < _fieldHeight; i++) {
			for (int k = 0; k < _fieldWidth; k++) {
				if (_arrayBlocks[i][k])
					CC_SAFE_RELEASE_NULL(_arrayBlocks[i][k]);
			}
		}
		CC_SAFE_DELETE_ARRAY(_arrayBlocks);
	}
}

void GameField::init(const int width, const int height) {
	if (!_arrayBlocks) {
		if (height && width) {
			_fieldHeight = height;
			_fieldWidth = width;
		} 
		else 
			CCASSERT(false, "width & height must be inited");

		_arrayBlocks = new Block**[_fieldHeight];
		for (int i = 0; i < _fieldHeight; i++)
			_arrayBlocks[i] = new Block*[_fieldWidth];

		for (int i = 0; i < _fieldHeight; i++)
			for (int k = 0; k < _fieldWidth; k++)
				_arrayBlocks[i][k] = nullptr;

		MessagesQueue::WrapperMessageQueueCallback_1 callback1(std::bind(&GameField::updateGameField, std::placeholders::_1)
			, "GameFieldUpdate");
		MessagesQueue::addListener(MessagesQueue::MessageType::UPDATE_GAME_FIELD, callback1);
	}
}

void GameField::updateGameField(void* args) {
	GameField::checkField();
}

void GameField::setBlock(Block* block) {
	if (!_arrayBlocks) {
		cocos2d::log("GameField must be init first!");
		throw;
	}

	const int x1 = block->getPosOnField().x;
	const int y1 = block->getPosOnField().y;

	const int x2 = block->getAttachedBlock()->getPosOnField().x;
	const int y2 = block->getAttachedBlock()->getPosOnField().y;

	if (y1 < _fieldHeight && y2 < _fieldHeight
		&& x1 < _fieldWidth && x2 < _fieldWidth) {
		_arrayBlocks[y1][x1] = block;
		_arrayBlocks[y2][x2] = block->getAttachedBlock();
	}
	else
		CCASSERT(false, "Can't set block on GameField");
}

void GameField::setBlock(int y, int x, Block* block){
	if (!_arrayBlocks) {
		cocos2d::log("GameField must be init first!");
		throw;
	}

	_arrayBlocks[y][x] = block;
}

Block* GameField::getBlock(int y, int x) {
	if (!_arrayBlocks) {
		cocos2d::log("GameField must be init first!");
		throw;
	}

	if (y < _fieldHeight && x < _fieldWidth)
		return _arrayBlocks[y][x];
	else
		return nullptr;
}

Block* GameField::getBlock(Vec2 pos) {
	if (!_arrayBlocks) {
		cocos2d::log("GameField must be init first!");
		throw;
	}

	if (pos.y < _fieldHeight && pos.x < _fieldWidth) {
		const int x = pos.x;
		const int y = pos.y;
		return _arrayBlocks[y][x];
	}
	else
		return nullptr;
}

b2Vec2 GameField::getSize() {
	b2Vec2 size; 
	size.x = _fieldWidth;
	size.y = _fieldHeight;

	return size;
}

void GameField::checkField() {
	Color3B currentColor = Color3B::WHITE;
	int counterLeftRight = 0;
	int *SCORES = new int(0);
	int counterTopDown = 0;
	int bottom = 0;
	int right = 0;
	int left = 0;
	int top = 0;
	
	std::vector<int> vectorLeftRight;
	std::vector<int> vectorUpDown;

	GameField::recountGameField();

	for (int i = 0; i < _fieldHeight; i++) {
		if (left != right && counterLeftRight >= 3) {
			vectorLeftRight.push_back(left);
			vectorLeftRight.push_back(right);
			vectorLeftRight.push_back(i - 1);
		}
		
		currentColor = Color3B::WHITE;
		counterLeftRight = 0;
		left = 0;
		right = 0;

		for (int k = 0; k < _fieldWidth; k++) {
			if (_arrayBlocks[i][k]) {
				if (currentColor != _arrayBlocks[i][k]->getColor() && counterLeftRight < 3) {
					currentColor = _arrayBlocks[i][k]->getColor();
					counterLeftRight = 1;
					left = k;
					right = k;
				}
				else if (currentColor != _arrayBlocks[i][k]->getColor() && counterLeftRight >= 3) {
					vectorLeftRight.push_back(left);
					vectorLeftRight.push_back(right);
					vectorLeftRight.push_back(i);

					currentColor = _arrayBlocks[i][k]->getColor();
					counterLeftRight = 1;
					left = k;
					right = k;
				}
				else {
					counterLeftRight++;
					right = k;
				}
			}
			else {
				currentColor = Color3B::WHITE;
				if (left != right && counterLeftRight >= 3) {
					vectorLeftRight.push_back(left);
					vectorLeftRight.push_back(right);
					vectorLeftRight.push_back(i);

					currentColor = Color3B::WHITE;
					counterLeftRight = 0;
					left = 0;
					right = 0;
				}
			}
		}
	}

	currentColor = Color3B::WHITE;
	for (int i = 0; i < _fieldWidth; i++) {
		if (top != bottom && counterTopDown >= 3) {
			vectorUpDown.push_back(bottom);
			vectorUpDown.push_back(top);
			vectorUpDown.push_back(i - 1);
		}

		currentColor = Color3B::WHITE;
		counterTopDown = 0;
		top = 0;
		bottom = 0;

		for (int k = 0; k < _fieldHeight; k++) {
			if (_arrayBlocks[k][i]) {
				if (currentColor != _arrayBlocks[k][i]->getColor() && counterTopDown < 3) {
					currentColor = _arrayBlocks[k][i]->getColor();
					counterTopDown = 1;
					top = k;
					bottom = k;
				}
				else if (currentColor != _arrayBlocks[k][i]->getColor() && counterTopDown >= 3) {
					vectorUpDown.push_back(bottom);
					vectorUpDown.push_back(top);
					vectorUpDown.push_back(i);

					currentColor = _arrayBlocks[k][i]->getColor();
					counterTopDown = 1;
					top = k;
					bottom = k;
				}
				else {
					counterTopDown++;
					top = k;
				}
			}
			else {
				currentColor = Color3B::WHITE;
				if (top != bottom && counterTopDown >= 3) {
					vectorUpDown.push_back(bottom);
					vectorUpDown.push_back(top);
					vectorUpDown.push_back(i);

					currentColor = Color3B::WHITE;
					counterTopDown = 0;
					top = 0;
					bottom = 0;
				}
			}
		}
	}
	
	if (vectorLeftRight.size() % 3 == 0) {
		for (std::vector<int>::iterator it = vectorLeftRight.begin(); it != vectorLeftRight.end(); ++it) {
			int l = *it;
			++it;
			int r = *it;
			++it;
			const int row = *it;

			for (; l <= r; l++) {
				if (_arrayBlocks[row][l]) {
					*SCORES += _arrayBlocks[row][l]->getScores();
					_arrayBlocks[row][l]->destroy();
					CC_SAFE_RELEASE(_arrayBlocks[row][l]);
					_arrayBlocks[row][l] = nullptr;
				}
			}
		}
	}

	if (vectorUpDown.size() % 3 == 0) {
		for (std::vector<int>::iterator it = vectorUpDown.begin(); it != vectorUpDown.end(); ++it) {
			int b = *it;
			++it;
			int t = *it;
			++it;
			const int column = *it;

			for (; b <= t; b++) {
				if (_arrayBlocks[b][column]) {
					*SCORES += _arrayBlocks[b][column]->getScores();
					_arrayBlocks[b][column]->destroy();
					CC_SAFE_RELEASE(_arrayBlocks[b][column]);
					_arrayBlocks[b][column] = nullptr;
				}
			}
		}
	}

	if (*SCORES)
		MessagesQueue::addMessageToQueue(
			MessagesQueue::Message{ MessagesQueue::MessageType::UPDATE_SCORES, SCORES });

	SimpleUI *simpleUI = MainGameScene::getUI();
	UserInput *input = (UserInput*)simpleUI->getChildrenByName(UserInput::name());
	input->dropInputEvents();
}

void GameField::recountGameField() {
	for (int i = 0; i < _fieldHeight; i++) {
		for (int k = 0; k < _fieldWidth; k++) {
			if (_arrayBlocks[i][k]) {
				const int y1 = _arrayBlocks[i][k]->getPosOnField().y;
				const int x1 = _arrayBlocks[i][k]->getPosOnField().x;

				if (x1 != k || y1 != i) {
					auto block1 = _arrayBlocks[i][k];
					_arrayBlocks[i][k] = nullptr;
					_arrayBlocks[y1][x1] = block1;
				}
			}
		}
	}
}
