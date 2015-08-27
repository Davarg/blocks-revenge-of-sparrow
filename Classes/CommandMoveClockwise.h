#ifndef __COMMAND_MOVE_CLOCKWISE_H__
#define __COMMAND_MOVE_CLOCKWISE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Command.h"

class CommandMoveClockwise : public Command {
private:
	b2Vec2 _positionOld;
	b2Vec2 _positionNew;
	bool _isExecute;
	Block *_block;
	bool _isUndo;
	bool _isRedo;

public:
	CommandMoveClockwise() : _isExecute(false), _isRedo(false), _isUndo(false){};

	virtual void undo() override;
	virtual void redo() override;
	virtual void update(float dt) override;
	virtual void execute(Block *block) override;
	virtual bool isExecute() override { return _isExecute; }
};

#endif