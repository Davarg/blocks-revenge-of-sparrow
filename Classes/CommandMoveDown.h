#ifndef __COMMAND_MOVE_DOWN_H__
#define __COMMAND_MOVE_DOWN_H__

#include "Command.h"
#include <cocos2d.h>
#include <Box2D\Box2D.h>

class CommandMoveDown : public Command {
private:
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

	CREATE_FUNC(CommandMoveDown);
	void stopBlock(bool withBody = true);
};

#endif
