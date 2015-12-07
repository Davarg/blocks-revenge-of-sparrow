#include <math.h>
#include "Block.h"
#include "MainGameScene.h"
#include "MessagesQueue.h"
#include "ConstantsRegistry.h"
#include "BlockContactListener.h"

bool BlockContactListener::isJointNeeded(b2Contact *contact) {
	b2Vec2 relativeContactPoint;
	b2Body *bottomBody = nullptr;
	b2WorldManifold worldManifold;
	b2PolygonShape *shapeBottomBody = nullptr;
	const int numPoints = contact->GetManifold()->pointCount;
	
	if (contact->GetFixtureB()->GetFilterData().categoryBits == Block::blockFlags::PASSIVE) {
		bottomBody = contact->GetFixtureB()->GetBody();
		shapeBottomBody = (b2PolygonShape*)contact->GetFixtureB()->GetShape();
	}
	else {
		bottomBody = contact->GetFixtureA()->GetBody();
		shapeBottomBody = (b2PolygonShape*)contact->GetFixtureA()->GetShape();
	}

	b2Vec2 borderLeft = shapeBottomBody->GetVertex(0);
	b2Vec2 borderRight = shapeBottomBody->GetVertex(1);

	contact->GetWorldManifold(&worldManifold);
	
	for (int i = 0; i < numPoints; i++) {
		relativeContactPoint = bottomBody->GetLocalPoint(worldManifold.points[i]);

		if (relativeContactPoint.x > borderLeft.x
			&& relativeContactPoint.x < borderRight.x
			&& relativeContactPoint.y > borderRight.y)
			return true;
	}

	return false;
}

void BlockContactListener::BeginContact(b2Contact *contact) {
	bool isBlockAdded = false;
	auto bodyA = contact->GetFixtureA()->GetBody();
	auto bodyB = contact->GetFixtureB()->GetBody();
	auto bodyUserDataA = (Sprite*)contact->GetFixtureA()->GetBody()->GetUserData();
	auto bodyUserDataB = (Sprite*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (isJointNeeded(contact)) {
		auto fixture = bodyA->GetFixtureList();
		if (!fixture->GetNext()) /*IsBlock?*/ {
			b2Filter filter;
			for (auto jointEdge = bodyA->GetJointList(); jointEdge != nullptr; jointEdge = jointEdge->next) {
				auto bA = jointEdge->joint->GetBodyA();
				auto bB = jointEdge->joint->GetBodyB();
				
				filter = bA->GetFixtureList()->GetFilterData();
				if (filter.categoryBits == Block::blockFlags::ACTIVE) {
					filter.categoryBits = Block::blockFlags::PASSIVE;
					bA->GetFixtureList()->SetFilterData(filter);
					
					auto contactEdge = bA->GetContactList();
					if (contactEdge)
						contactEdge->contact->SetEnabled(false);

					Block::bodiesStructArgs *bodies = new Block::bodiesStructArgs;
					bodies->b1 = bA;
					bodies->b2 = bodyB;
					MessagesQueue::addMessageToQueue(
						MessagesQueue::Message{ MessagesQueue::MessageType::CREATE_JOINT, bodies });
				}

				filter = bB->GetFixtureList()->GetFilterData();
				if (filter.categoryBits == Block::blockFlags::ACTIVE) {
					filter.categoryBits = Block::blockFlags::PASSIVE;
					bB->GetFixtureList()->SetFilterData(filter);

					auto contactEdge = bB->GetContactList();
					if (contactEdge)
						contactEdge->contact->SetEnabled(false);

					Block::bodiesStructArgs *bodies = new Block::bodiesStructArgs;
					bodies->b1 = bB;
					bodies->b2 = bodyB;
					MessagesQueue::addMessageToQueue(
						MessagesQueue::Message{ MessagesQueue::MessageType::CREATE_JOINT, bodies });
				}
			}

			if (!isBlockAdded) {
				MessagesQueue::addMessageToQueue(
					MessagesQueue::Message{ MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, nullptr });
				isBlockAdded = true;
			}
		}

		fixture = bodyB->GetFixtureList();
		if (!fixture->GetNext()) /*IsBlock?*/ {
			b2Filter filter;
			for (auto jointEdge = bodyB->GetJointList(); jointEdge != nullptr; jointEdge = jointEdge->next) {
				auto bA = jointEdge->joint->GetBodyA();
				auto bB = jointEdge->joint->GetBodyB();

				filter = bA->GetFixtureList()->GetFilterData();
				if (filter.categoryBits == Block::blockFlags::ACTIVE) {
					filter.categoryBits = Block::blockFlags::PASSIVE;
					bA->GetFixtureList()->SetFilterData(filter);

					auto contactEdge = bA->GetContactList();
					if (contactEdge)
						contactEdge->contact->SetEnabled(false);

					Block::bodiesStructArgs *bodies = new Block::bodiesStructArgs;
					bodies->b1 = bodyA;
					bodies->b2 = bA;
					MessagesQueue::addMessageToQueue(
						MessagesQueue::Message{ MessagesQueue::MessageType::CREATE_JOINT, bodies });
				}

				filter = bB->GetFixtureList()->GetFilterData();
				if (filter.categoryBits == Block::blockFlags::ACTIVE) {
					filter.categoryBits = Block::blockFlags::PASSIVE;
					bB->GetFixtureList()->SetFilterData(filter);
					
					auto contactEdge = bB->GetContactList();
					if (contactEdge)
						contactEdge->contact->SetEnabled(false);

					Block::bodiesStructArgs *bodies = new Block::bodiesStructArgs;
					bodies->b1 = bodyA;
					bodies->b2 = bB;
					MessagesQueue::addMessageToQueue(
						MessagesQueue::Message{ MessagesQueue::MessageType::CREATE_JOINT, bodies });
				}
			}

			if (!isBlockAdded) {
				MessagesQueue::addMessageToQueue(
					MessagesQueue::Message{ MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, nullptr });
				isBlockAdded = true;
			}
		}
		
		MessagesQueue::addMessageToQueue(
			MessagesQueue::Message{ MessagesQueue::MessageType::UPDATE_GAME_FIELD, nullptr });
	}
}