#include "MessagesQueue.h"
#include "Block.h"

struct funcAndObj {
	void* obj = nullptr;
	void(*ptrFunc)(void*) = nullptr;
	void(*ptrFuncExt)(void*, void*) = nullptr;
};

std::vector<MessagesQueue::Message> msgQueue;
std::map<MessagesQueue::MessageType, std::vector<funcAndObj> > mapListeners;

void MessagesQueue::update(float dt) {
	for (Vector<Message>::iterator it = msgQueue.begin();
			it != msgQueue.end();) {
		auto vec = mapListeners.at(it->mt);
		if (vec.size()) {
			for (auto vecIt = vec.begin(); vecIt != vec.end(); vecIt++) {
				if (vecIt->obj != nullptr) 
					static_cast<void(*)(void*, void*)>(vecIt->ptrFuncExt)(vecIt->obj, it->args);
				else
					static_cast<void(*)(void*)>(vecIt->ptrFunc)(it->args);
			}
		}
		it = msgQueue.erase(it);
	}
	msgQueue.shrink_to_fit();
}

void MessagesQueue::addMessageToQueue(Message msg) {
	msgQueue.push_back(msg);
}

void MessagesQueue::addListener(MessageType mt, void(*ptrFunc)(void*)) {
	funcAndObj fao;
	fao.ptrFunc = ptrFunc;
	
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto iterator = mapListeners.find(mt);
		iterator->second.push_back(fao);
	}
	else {
		std::vector<funcAndObj> vec;
		vec.push_back(fao);
		mapListeners.insert(std::make_pair(mt, vec));
	}
}

void MessagesQueue::addListener(MessageType mt, void* ptrObj, void(*ptrFunc)(void*, void*)) {
	funcAndObj fao;
	fao.ptrFuncExt = ptrFunc;
	fao.obj = ptrObj;

	if (mapListeners.end() != mapListeners.find(mt)) {
		auto iterator = mapListeners.find(mt);
		iterator->second.push_back(fao);
	}
	else {
		std::vector<funcAndObj> vec;
		vec.push_back(fao);
		mapListeners.insert(std::make_pair(mt, vec));
	}
}

void MessagesQueue::removeListener(MessageType mt, void(*ptrFunc)(void*)) {
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto it = mapListeners.find(mt);
		for (auto itVec = it->second.begin(); itVec != it->second.end();) {
			if (itVec->ptrFunc == ptrFunc) {
				itVec = it->second.erase(itVec);
			}
		}
		it->second.shrink_to_fit();
	}
}

void MessagesQueue::removeListener(MessageType mt, void* ptrObj, void(*ptrFunc)(void*, void*)) {
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto it = mapListeners.find(mt);
		for (auto itVec = it->second.begin(); itVec != it->second.end();) {
			if (itVec->ptrFuncExt == ptrFunc && itVec->obj == ptrObj) {
				itVec = it->second.erase(itVec);
			}
		}
		it->second.shrink_to_fit();
	}
}