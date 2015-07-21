#ifndef __COMMAND_MOVE_COUNTER_CLOCKWISE_H__
#define __COMMAND_MOVE_COUNTER_CLOCKWISE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Command.h"

class CommandMoveCounterClockwise : public Command {
private:
	b2Vec2 _positionOld;
	b2Vec2 _positionNew;
	Block *_block;
	bool _isUndo;
	bool _isRedo;
	bool _isExecute;

public:
	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void execute(Block *block) override;
	virtual void undo() override;
	virtual void redo() override;
	virtual bool isExecute() override { return _isExecute; }

	CREATE_FUNC(CommandMoveCounterClockwise);
};

#endif