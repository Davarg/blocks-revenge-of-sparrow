#ifndef __COMMAND_MOVE_COUNTER_CLOCKWISE_H__
#define __COMMAND_MOVE_COUNTER_CLOCKWISE_H__

#include "Command.h"
#include <cocos2d.h>
#include <Box2D\Box2D.h>

class CommandMoveCounterClockwise : public Command {
private:
	b2Vec2 _positionOld;
	b2Vec2 _positionNew;
	bool _isExecute;
	Block *_block;
	bool _isUndo;
	bool _isRedo;

public:
	virtual void undo() override;
	virtual void redo() override;
	virtual bool init() override;
	virtual void update(float) override;
	virtual void execute(Block*) override;
	virtual bool isExecute() override { return _isExecute; }

	CREATE_FUNC(CommandMoveCounterClockwise);
};

#endif