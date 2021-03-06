#ifndef __COMMAND_MOVE_RIGHT_H__
#define __COMMAND_MOVE_RIGHT_H__

#include "Command.h"
#include <cocos2d.h>
#include <Box2D\Box2D.h>

class CommandMoveRight : public Command {
private:
	b2Vec2 _positionOldSecond;
	b2Vec2 _positionOldFirst;
	Size _blockSize;
	bool _isExecute;
	Block *_block;
	bool _isUndo;
	bool _isRedo;

	void stopBlock();
	
public:
	virtual void undo() override;
	virtual void redo() override;
	virtual bool init() override;
	virtual void update(float) override;
	virtual void execute(Block*) override;
	virtual bool isExecute() override { return _isExecute; }
	
	CREATE_FUNC(CommandMoveRight);
};

#endif