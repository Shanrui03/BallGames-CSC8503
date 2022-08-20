#include "ConstraintObject.h"
using namespace NCL;
using namespace CSC8503;

ConstraintObject::ConstraintObject( GameObject* player, string name)
{
	this->name = name;
	this->player = player;
	rootsequence = new BehaviourSequence("Root Sequence");

	delta = Vector3(0.0f,0.0f,0.0f);
	relativeDistance = 100000.0f;

	BehaviourAction* buildConstraint = new BehaviourAction("Build Constraint",
		[&](float dt, BehaviourState state)->BehaviourState
		{
			if (state == Initialise)
			{
				this->statename = "Waiting for player..";
				state = Ongoing;
			}
			else if (state == Ongoing)
			{
				if (relativeDistance <= 10.0f)
				{
					return Success;
				}
			}
			return state;
		}
	);

	BehaviourSequence* sequence = new BehaviourSequence("Leaf Sequence");
	sequence->AddChild(buildConstraint);

	rootsequence->AddChild(sequence);
	rootsequence->Reset();
}
ConstraintObject::~ConstraintObject()
{
	delete rootsequence;
}
void ConstraintObject::Update(float dt)
{
	delta = player->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	relativeDistance = delta.Length();

	BehaviourState state = Ongoing;
	if (state == Ongoing)
	{
		state = rootsequence->Execute(dt);
	}
	if (state == Success)
	{
		this->statename = "Build Constraint!";
	}
	else if (state == Failure)
	{
		this->statename =  "What a waste of time!";
	}
}