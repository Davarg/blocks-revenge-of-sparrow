#include "BlockContactListener.h"
#include "MainGameScene.h"
#include <math.h>
#include "Block.h"
#include "MessagesQueue.h"
#include "Constants.h"

void BlockContactListener::BeginContact(b2Contact* contact) {
	const int MOD = 4;
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData(); //Static body, in rare cases, it may be dynamic body
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData(); //Dynamic body, in rare cases, it may be static body
	const uint16 categoryA = contact->GetFixtureA()->GetFilterData().categoryBits;
	const uint16 categoryB = contact->GetFixtureB()->GetFilterData().categoryBits;
	
	b2Filter passiveFilter;
	passiveFilter.groupIndex = contact->GetFixtureB()->GetFilterData().groupIndex;
	passiveFilter.maskBits = contact->GetFixtureB()->GetFilterData().maskBits;
	passiveFilter.categoryBits = Block::getPassiveCategoryBits();

	b2Body *fixtureABodyA = nullptr;
	b2Body *fixtureABodyB = nullptr;
	b2Body *fixtureBBodyA = nullptr;
	b2Body *fixtureBBodyB = nullptr;
	if (contact->GetFixtureB()->GetBody()->GetJointList()) {
		fixtureBBodyA = contact->GetFixtureB()->GetBody()->GetJointList()->joint->GetBodyA();
		fixtureBBodyB = contact->GetFixtureB()->GetBody()->GetJointList()->joint->GetBodyB();
		contact->GetFixtureB()->GetBody()->GetJointList()->joint->GetBodyA()->SetLinearVelocity(b2Vec2(contact->GetFixtureB()->GetBody()->GetLinearVelocity().x / 4
			, contact->GetFixtureB()->GetBody()->GetLinearVelocity().y));
		contact->GetFixtureB()->GetBody()->GetJointList()->joint->GetBodyB()->SetLinearVelocity(b2Vec2(contact->GetFixtureB()->GetBody()->GetLinearVelocity().x / 4
			, contact->GetFixtureB()->GetBody()->GetLinearVelocity().y));
	}
	if (contact->GetFixtureA()->GetBody()->GetJointList()) {
		fixtureABodyA = contact->GetFixtureA()->GetBody()->GetJointList()->joint->GetBodyA();
		fixtureABodyB = contact->GetFixtureA()->GetBody()->GetJointList()->joint->GetBodyB();
		contact->GetFixtureA()->GetBody()->GetJointList()->joint->GetBodyA()->SetLinearVelocity(b2Vec2(contact->GetFixtureA()->GetBody()->GetLinearVelocity().x / 4
			, contact->GetFixtureA()->GetBody()->GetLinearVelocity().y));
		contact->GetFixtureA()->GetBody()->GetJointList()->joint->GetBodyB()->SetLinearVelocity(b2Vec2(contact->GetFixtureA()->GetBody()->GetLinearVelocity().x / 4
			, contact->GetFixtureA()->GetBody()->GetLinearVelocity().y));
	}

	if (bodyUserDataA && bodyUserDataB && (categoryB & Block::getActiveCategoryBits() || categoryA & Block::getActiveCategoryBits())) {
		auto spriteA = static_cast<Sprite*>(bodyUserDataA);
		auto spriteB = static_cast<Sprite*>(bodyUserDataB);
		Vec2 posA = spriteA->getPosition();
		Vec2 posB = spriteB->getPosition();

		if ((round(posA.y) <= round(posB.y) || round(posA.y) >= round(posB.y)) && abs(posA.x - posB.x) <= MOD) {
			Block::bodiesStructArgs *bodies = new Block::bodiesStructArgs;
			bodies->b1 = contact->GetFixtureA()->GetBody();
			bodies->b2 = contact->GetFixtureB()->GetBody();
			bodies->pos.x = posB.x;
			bodies->pos.y = posB.y;
			MessagesQueue::addMessageToQueue(
				MessagesQueue::Message{ MessagesQueue::MessageType::CREATE_JOINT, bodies });

			void* ptr = nullptr;
			MessagesQueue::addMessageToQueue(
				MessagesQueue::Message{ MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, ptr });

			if (fixtureABodyA && fixtureABodyB) {
				fixtureABodyA->GetFixtureList()->SetFilterData(passiveFilter);
				fixtureABodyB->GetFixtureList()->SetFilterData(passiveFilter);
			}
			if (fixtureBBodyA && fixtureBBodyB) {
				fixtureBBodyA->GetFixtureList()->SetFilterData(passiveFilter);
				fixtureBBodyB->GetFixtureList()->SetFilterData(passiveFilter);
			}
		}
	}
	else if (!bodyUserDataA && bodyUserDataB
				&& (categoryB & Block::getActiveCategoryBits() 
					&& (categoryA & Block::getActiveCategoryBits() || categoryB & Block::getPassiveCategoryBits())))  {
		auto spriteB = static_cast<Sprite*>(bodyUserDataB);
		Block::bodiesStructArgs *bodies = new Block::bodiesStructArgs;
		bodies->b1 = contact->GetFixtureA()->GetBody();
		bodies->b2 = contact->GetFixtureB()->GetBody();
		bodies->pos.x = spriteB->getPosition().x;
		bodies->pos.y = spriteB->getPosition().y;
		MessagesQueue::addMessageToQueue(
			MessagesQueue::Message{ MessagesQueue::MessageType::CREATE_JOINT, bodies });

		void* ptr = nullptr;
		MessagesQueue::addMessageToQueue(
			MessagesQueue::Message{ MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, ptr });
		if (fixtureABodyA && fixtureABodyB) {
			fixtureABodyA->GetFixtureList()->SetFilterData(passiveFilter);
			fixtureABodyB->GetFixtureList()->SetFilterData(passiveFilter);
		}
		if (fixtureBBodyA && fixtureBBodyB) {
			fixtureBBodyA->GetFixtureList()->SetFilterData(passiveFilter);
			fixtureBBodyB->GetFixtureList()->SetFilterData(passiveFilter);
		}
	}
	else {
		if (categoryA & Block::getActiveCategoryBits() || categoryB & Block::getPassiveCategoryBits()) {
			Block::bodiesStructArgs *bodies = new Block::bodiesStructArgs;
			bodies->b1 = contact->GetFixtureA()->GetBody();
			bodies->b2 = contact->GetFixtureB()->GetBody();
			auto spriteB = static_cast<Sprite*>(bodyUserDataB);
			bodies->pos.x = spriteB->getPosition().x;
			bodies->pos.y = spriteB->getPosition().y;
			MessagesQueue::addMessageToQueue(
				MessagesQueue::Message{ MessagesQueue::MessageType::CREATE_JOINT, bodies });
		}

		void* ptr = nullptr;
		MessagesQueue::addMessageToQueue(
			MessagesQueue::Message{ MessagesQueue::MessageType::UPDATE_GAME_FIELD, ptr});
	}
}