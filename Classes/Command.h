#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Block.h"

class Command : public Ref {
public:
	virtual void update(float dt) = 0;
	virtual void execute(Block *block) = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;
	virtual bool isExecute() = 0;
	virtual bool init() = 0;
};

#endif