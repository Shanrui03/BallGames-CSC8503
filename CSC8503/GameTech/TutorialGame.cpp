#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()	{
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);

	forceMagnitude	= 10.0f;
	useGravity		= false;
	inSelectionMode = false;

	Debug::SetRenderer(renderer);

	InitialiseAssets();
}
TutorialGame::~TutorialGame() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete dogeTex;
	delete iceTex;
	delete catTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}
/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/

//Getters
GameWorld* TutorialGame::GetGameWorld()
{
	return world;
}
PhysicsSystem* TutorialGame::GetPhysics()
{
	return physics;
}



//Initialise and Update function
void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh"		 , &cubeMesh);
	loadFunc("sphere.msh"	 , &sphereMesh);
	loadFunc("Male1.msh"	 , &charMeshA);
	loadFunc("courier.msh"	 , &charMeshB);
	loadFunc("security.msh"	 , &enemyMesh);
	loadFunc("coin.msh"		 , &bonusMesh);
	loadFunc("capsule.msh"	 , &capsuleMesh);

	basicTex	= (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	dogeTex = (OGLTexture*)TextureLoader::LoadAPITexture("doge.png");
	catTex = (OGLTexture*)TextureLoader::LoadAPITexture("nyan.png");
	iceTex = (OGLTexture*)TextureLoader::LoadAPITexture("Ice.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

}
void TutorialGame::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}

	if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(5, 95));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(5, 95));
	}

	SelectObject();
	MoveSelectedObject();
	physics->Update(dt);

	//print player's score
	Debug::Print("Total score : " + std::to_string(physics->score), Vector2(10, 10));

	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0,1,0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);
		/*world->GetMainCamera()->UpdateCamera(dt);*/

		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}

	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();

	//flip the trigger of the gameobject
	if (selectionObject)
	{
		selectionObject->isSelected = !selectionObject->isSelected;
		string linearspeed = "(" +
			std::to_string((int)selectionObject->GetPhysicsObject()->GetLinearVelocity().x) + "," +
			std::to_string((int)selectionObject->GetPhysicsObject()->GetLinearVelocity().y) + "," +
			std::to_string((int)selectionObject->GetPhysicsObject()->GetLinearVelocity().z) + ")";
		string angularspeed = "(" +
			std::to_string((int)selectionObject->GetPhysicsObject()->GetAngularVelocity().x) + "," +
			std::to_string((int)selectionObject->GetPhysicsObject()->GetAngularVelocity().y) + "," +
			std::to_string((int)selectionObject->GetPhysicsObject()->GetAngularVelocity().z) + ")";
		string position = "(" +
			std::to_string((int)selectionObject->GetTransform().GetPosition().x) + "," +
			std::to_string((int)selectionObject->GetTransform().GetPosition().y) + "," +
			std::to_string((int)selectionObject->GetTransform().GetPosition().z) + ")";
		string TypeName = "";
		switch (selectionObject->GetBoundingVolume()->type)
		{
		case VolumeType::AABB:
			TypeName = "AABB";
			break;
		case VolumeType::Capsule:
			TypeName = "Capsule";
			break;
		case VolumeType::OBB:
			TypeName = "OBB";
			break;
		case VolumeType::Sphere:
			TypeName = "Sphere";
			break;
		default:
			break;
		}
		Debug::Print("Object name : " + selectionObject->GetName(), Vector2(50, 10));
		Debug::Print("Object's InverseMass : " + std::to_string((int)selectionObject->GetPhysicsObject()->GetInverseMass()), Vector2(50, 15));
		Debug::Print("Object's LinearSpeed : " + linearspeed, Vector2(30, 25));
		Debug::Print("Object's AngularSpeed : " + angularspeed, Vector2(30, 30));
		if (selectionObject->GetName() == "coin" || selectionObject->GetName() == "Attachment" || 
			selectionObject->GetName() == "Enemy" || selectionObject->GetName() == "EnemyBall")
		{
			Debug::Print("Object's State : " + selectionObject->statename, Vector2(30, 35));
		}
		Debug::Print("Object's typename: " + TypeName, Vector2(30, 38));
		Debug::Print("Object's position: " + position, Vector2(30, 43));
	}

	//constraint objects
	if (constraintSphere && constraintSphere->statename != "Build Constraint!")
	{
		constraintSphere->Update(dt);
	}
	if (constraintSphere && constraintSphere->statename == "Build Constraint!" && !attachmentConstraint)
	{
		attachmentConstraint = new PositionConstraint(player, constraintSphere, 5.0f);
		world->AddConstraint(attachmentConstraint);
	}
	if (constraintSphere2 && constraintSphere2->statename != "Build Constraint!")
	{
		constraintSphere2->Update(dt);
	}
	if (constraintSphere2 && constraintSphere2->statename == "Build Constraint!" && !attachmentConstraint2)
	{
		attachmentConstraint2 = new PositionConstraint(player2, constraintSphere2, 5.0f);
		world->AddConstraint(attachmentConstraint2);
		physics->score = physics->score + 50;
	}

	UpdateNormalStateObject(dt);
}
void TutorialGame::UpdateNormalStateObject(float dt)
{
	if (pointcoin)
	{
		pointcoin->Update(dt);
	}
	if (pointcoin2)
	{
		pointcoin2->Update(dt);
	}
	if (pointcoin3)
	{
		pointcoin3->Update(dt);
	}
	if (pointcoin4)
	{
		pointcoin4->Update(dt);
	}
	if (pointcoin5)
	{
		pointcoin5->Update(dt);
	}
	if (pointcoin6)
	{
		pointcoin6->Update(dt);
	}
	if (enemy1)
	{
		enemy1->Update(dt);
	}
	if (enemy2)
	{
		enemy2->Update(dt);
	}
	if (enemyball)
	{
		enemyball->Update(dt);
	}
}
void TutorialGame::UpdateKeys(bool isHard) {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		if (!isHard)
		{
			InitWorld();//We can reset the simulation at any time with F2
		}
		else
		{
			InitHardGameWorld();
		}
		selectionObject = nullptr;
		lockedObject = nullptr;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F3)) {
		if (!isHard)
		{
			InitWorld(true);//Debug Mode
		}
		else
		{
			InitHardGameWorld(true);
		}
		selectionObject = nullptr;
		lockedObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}
}
void TutorialGame::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward  = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 10.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		Vector3 worldPos = selectionObject->GetTransform().GetPosition();
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NEXT)) {
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0,-10,0));
	}
}
void TutorialGame::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}

}
void TutorialGame::InitCamera(bool isHard) {
	if (!isHard)
	{
		world->GetMainCamera()->SetNearPlane(0.1f);
		world->GetMainCamera()->SetFarPlane(500.0f);
		world->GetMainCamera()->SetPitch(-15.0f);
		world->GetMainCamera()->SetYaw(270.0f);
		world->GetMainCamera()->SetPosition(Vector3(-150, 20, 0));
		lockedObject = nullptr;
	}
	else
	{
		world->GetMainCamera()->SetNearPlane(0.1f);
		world->GetMainCamera()->SetFarPlane(500.0f);
		world->GetMainCamera()->SetPitch(-90.0f);
		world->GetMainCamera()->SetYaw(360.0f);
		world->GetMainCamera()->SetPosition(Vector3(30, 100, 120));
		lockedObject = nullptr;
	}
}
void TutorialGame::InitWorld(bool isDebug) {
	InitCamera(false);
	world->ClearAndErase();
	physics->Clear();
	enemyball = nullptr;
	attachmentConstraint = nullptr;
	attachmentConstraint2 = nullptr;
	if (isDebug)
	{
		physics->score = 1000000;
	}
	else
	{
		physics->score = 200;
	}
	physics->isGameEnd = false;
	physics->isGameDead = false;
	physics->StartTime = clock();
	InitSpecialItems();
	InitStateObjects();
	InitDefaultFloor();
	InitCapsuleAndOBB();
}
void TutorialGame::InitHardGameWorld(bool isDebug)
{
	InitCamera(true);
	world->ClearAndErase();
	physics->Clear();
	attachmentConstraint = nullptr;
	attachmentConstraint2 = nullptr;
	enemyball = nullptr;
	if (isDebug)
	{
		physics->score = 1000000;
	}
	else
	{
		physics->score = 300;
	}
	physics->isGameEnd = false;
	physics->isGameDead = false;
	physics->StartTime = clock();
	AddFloorToWorld(Vector3(45, -4, 45),Vector3(50,2,50));
	InitMap();
	InitSpecialItems2();
	InitEnemyAndPlayer();
}
void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(8, 8, 8);

	float invCubeMass = 5;//how heavy the middle pieces are
	int numLinks = 10;
	float maxDistance = 30;//constraint distance
	float cubeDistance = 20;//distance between links

	Vector3 startPos = Vector3(10, 100, 10);

	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0);
	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 0);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i)
	{
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, "" ,invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position,Vector3 dimension) {
	GameObject* floor = new GameObject("floor1");

	Vector3 floorSize = dimension;
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}
GameObject* TutorialGame::AddSlopingFloorToWorld(const Vector3& position, const Quaternion& orientation)
{
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(100, 2, 100);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetOrientation(orientation)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, string name,float inverseMass) {
	GameObject* sphere = new GameObject(name);

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	if (sphere->GetName() == "Player")
	{
		sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, dogeTex, basicShader));
	}
	else
	{
		sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	}
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}
GameObject* TutorialGame::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, string name, float inverseMass,float elasticity) {
	GameObject* capsule = new GameObject(name);

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius* 2, halfHeight, radius * 2))
		.SetPosition(position);
	if (capsule->GetName() == "InvSteering")
	{
		capsule->GetTransform()
			.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
			.SetOrientation(Quaternion(0.0f, 0.0f, 1.0f, 1.0f))
			.SetPosition(position);
	}

	if (name == "Dest")
	{
		capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, nullptr, basicShader));
		capsule->GetRenderObject()->SetColour(Vector4(1.5, 0.5, 0.5, 1));
	}
	else
	{
		capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	}
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();
	if (elasticity != 0.8f)
	{
		capsule->GetPhysicsObject()->SetElasticity(elasticity);
	}

	world->AddGameObject(capsule);

	return capsule;

}
GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, string name,float inverseMass,float friction) {
	GameObject* cube = new GameObject(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	if (cube->GetName() == "Ice")
	{
		cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, iceTex, basicShader));
	}
	else if (cube->GetName() == "Mud")
	{
		cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, nullptr, basicShader));
		cube->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));
	}
	else
	{
		cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	}
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->SetFriction(friction);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}
GameObject* TutorialGame::AddOBBToWorld(const Vector3& position, const Quaternion& orientation,Vector3 dimensions,string name,float inverseMass)
{
	GameObject* cube = new GameObject(name);

	OBBVolume* volume = new OBBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetOrientation(orientation)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}
void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius);
		}
	}
	/*AddFloorToWorld(Vector3(0, -2, 0));*/
}
void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
}
void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols+1; ++x) {
		for (int z = 1; z < numRows+1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, "" , 1.0f);
		}
	}
}
void TutorialGame::InitGameExamples() {
	AddPlayerToWorld(Vector3(0, 5, 0));
	AddEnemyToWorld(Vector3(5, 5, 0));
	AddBonusToWorld(Vector3(10, 5, 0));
}
void TutorialGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(0, -2, 0), Vector3(100, 2, 100));
	AddFloorToWorld(Vector3(102, 6, 0), Vector3(2, 20, 100));
	AddFloorToWorld(Vector3(0, 6, 102), Vector3(100, 20, 2));
	AddFloorToWorld(Vector3(0, 6, -102), Vector3(100, 20, 2));
}
//Mode 1 Initialise functions
void TutorialGame::InitStateObjects()
{
	player = AddSphereToWorld(Vector3(-95, 10, 0), 1.0f, "Player");
	enemy1 = AddEnemyToWorld(Vector3(-30.0f, 2.0f, 10.0f));
	enemy2 = AddEnemy02ToWorld(Vector3(-30.0f, 2.0f, -10.0f));
	pointcoin = AddPointCoinsToWorld(Vector3(0, 5, 0));
	pointcoin2 = AddPointCoinsToWorld(Vector3(5, 5, 0));
	pointcoin3 = AddPointCoinsToWorld(Vector3(10, 5, 0));
}
void TutorialGame::InitSpecialItems()
{
	AddCubeToWorld(Vector3(-95, 1, 0), Vector3(5.0f, 0.5f, 5.0f), "Shooter", 0.1f);
	AddCubeToWorld(Vector3(-10, 1, 0), Vector3(5.0f, 0.1f, 5.0f), "Ice", 0.0f);
	AddSphereToWorld(Vector3(30, 2, 49), 10.0f, "Rotate", 0.0f);
}
void TutorialGame::InitCapsuleAndOBB()
{
	constraintSphere = AddConstraintObjectToWorld(Vector3(0, 12, -20), 2.0f, 1.0f, player, "Attachment");
	AddCapsuleToWorld(Vector3(40, 8.0, 80), 8.0f, 8.0f, "Dest", 0.0f);
	AddCapsuleToWorld(Vector3(-10, 8, 40), 3.0f, 2.0f, "", 10.0f, 0.2f);
	AddCapsuleToWorld(Vector3(-30, 8, -30), 3.0f, 2.0f, "", 10.0f, 0.9f);
	AddOBBToWorld(Vector3(15, 1, 0), Quaternion(0.0f, 2.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 2.0f), "Steering", 0.0f);
	AddOBBToWorld(Vector3(45, 1.5, -10), Quaternion(0.0f, -2.0f, 0.0f, 1.0f), Vector3(4.0f, 1.0f, 4.0f), "", 0.0f);
	AddOBBToWorld(Vector3(0, 4, -20), Quaternion(0.0f, 10.0f, 0.0f, 1.0f), Vector3(8.0f, 4.0f, 8.0f), "InvSteering", 0.0f);
}
//Mode 2 Initialise functions
void TutorialGame::InitMap()
{
	gridMap = new NavigationGrid("TestGrid1.txt");
	GridNode* nodes = gridMap->GetNodes();
	int gridWidth = gridMap->GetWidth();
	int gridHeight = gridMap->GetHeight();
	for (int y = 0; y < gridHeight; ++y) 
	{
		for (int x = 0; x < gridWidth; ++x) 
		{
			GridNode& n = nodes[(gridWidth * y) + x];
			if (n.gridType == GRIDTYPE::Block) 
			{
				AddCubeToWorld(n.position, Vector3(0.5, 0.5, 0.5) * gridMap->nodeSize, "Wall", 0);
			}
			else if (n.gridType == GRIDTYPE::Obstacle)
			{
				AddSphereToWorld(n.position, 0.5 * gridMap->nodeSize, "ConstraintObstacle", 1.0f);
			}
		}
	}
}
void TutorialGame::InitEnemyAndPlayer()
{
	player2 = AddSphereToWorld(Vector3(30, 10, 120), 1.0f, "Player");
	constraintSphere2 = AddConstraintObjectToWorld(Vector3(80, 2, 80), 2.0f ,1.0f, player2, "Attachment");
	enemyball = AddEnemyBallToWorld(Vector3(70, 5, 0));
}
void TutorialGame::InitSpecialItems2()
{
	pointcoin4 = AddPointCoinsToWorld(Vector3(20, 5, 30));
	pointcoin5 = AddPointCoinsToWorld(Vector3(30, 5, 30));
	pointcoin6 = AddPointCoinsToWorld(Vector3(40, 5, 30));
	AddCubeToWorld(Vector3(30, 0, 120), Vector3(5.0f, 0.5f, 5.0f), "Shooter2", 0.0f);
	AddCubeToWorld(Vector3(70, 0, 0), Vector3(5.0f, 0.5f, 5.0f), "Shooter2", 0.0f);
	AddCubeToWorld(Vector3(20, -2, 20), Vector3(2.5f, 0.5f, 2.5f), "Freeze", 0.0f);
	AddCubeToWorld(Vector3(30, -2, 80), Vector3(4.0f, 0.5f, 4.0f), "Mud", 0.0f, 0.9f);
	AddCapsuleToWorld(Vector3(70, 0.5, -20), 2.0f, 2.0f, "Dest", 0.0f);
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}
StateGameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
	float meshSize		= 3.0f;
	float inverseMass	= 10.0f;

	StateGameObject* character = new StateGameObject("Enemy");

	CapsuleVolume* volume = new CapsuleVolume(meshSize, meshSize / 2);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}
InvStateGameObject* TutorialGame::AddEnemy02ToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 10.0f;

	InvStateGameObject* character = new InvStateGameObject("Enemy");

	CapsuleVolume* volume = new CapsuleVolume(meshSize, meshSize / 2);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}
GameObject* TutorialGame::AddBonusToWorld(const Vector3& position,string name) {
	GameObject* apple = new GameObject(name);

	SphereVolume* volume = new SphereVolume(0.25f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.5f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}
StateGameObject* TutorialGame::AddStateObjectToWorld(const Vector3& position, float halfHeight, float radius, string name, float inverseMass)
{
	StateGameObject* opponent = new StateGameObject();

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	opponent->SetBoundingVolume((CollisionVolume*)volume);

	opponent->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetPosition(position);

	opponent->SetRenderObject(new RenderObject(&opponent->GetTransform(), capsuleMesh, basicTex, basicShader));
	opponent->SetPhysicsObject(new PhysicsObject(&opponent->GetTransform(), opponent->GetBoundingVolume()));

	opponent->GetPhysicsObject()->SetInverseMass(inverseMass);
	opponent->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(opponent);

	return opponent;

}
PointCoins* TutorialGame::AddPointCoinsToWorld(const Vector3& position)
{
	PointCoins* coins = new PointCoins("coin");

	SphereVolume* volume = new SphereVolume(0.25f);
	coins->SetBoundingVolume((CollisionVolume*)volume);
	coins->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	coins->SetRenderObject(new RenderObject(&coins->GetTransform(), bonusMesh, nullptr, basicShader));
	coins->SetPhysicsObject(new PhysicsObject(&coins->GetTransform(), coins->GetBoundingVolume()));
	coins->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));

	coins->GetPhysicsObject()->SetInverseMass(0.5f);
	coins->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(coins);

	return coins;
}
ConstraintObject* TutorialGame::AddConstraintObjectToWorld(const Vector3& position, float halfHeight,float radius,GameObject* player, string name,float inverseMass)
{
	ConstraintObject* attachment = new ConstraintObject(player, name);

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	attachment->SetBoundingVolume((CollisionVolume*)volume);

	attachment->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetOrientation(Quaternion(0.0f,0.0f,1.0f,1.0f))
		.SetPosition(position);


	attachment->SetRenderObject(new RenderObject(&attachment->GetTransform(), capsuleMesh, nullptr, basicShader));

	attachment->SetPhysicsObject(new PhysicsObject(&attachment->GetTransform(), attachment->GetBoundingVolume()));

	attachment->GetPhysicsObject()->SetInverseMass(inverseMass);
	attachment->GetPhysicsObject()->InitCubeInertia();
	attachment->GetPhysicsObject()->SetElasticity(0.9f);
	attachment->GetRenderObject()->SetColour(Vector4(1, 0, 1, 1));

	world->AddGameObject(attachment);

	return attachment;
}
EnemyBall* TutorialGame::AddEnemyBallToWorld(const Vector3& position)
{
	EnemyBall* enemy = new EnemyBall(gridMap, player2, constraintSphere2, pointcoin4, world ,"EnemyBall");
	SphereVolume* volume = new SphereVolume(1.0f);
	enemy->SetBoundingVolume((CollisionVolume*)volume);
	enemy->GetTransform()
		.SetScale(Vector3(1.0, 1.0, 1.0))
		.SetPosition(position);

	enemy->SetRenderObject(new RenderObject(&enemy->GetTransform(), sphereMesh, catTex, basicShader));
	enemy->SetPhysicsObject(new PhysicsObject(&enemy->GetTransform(), enemy->GetBoundingVolume()));

	enemy->GetPhysicsObject()->SetInverseMass(1.5f);
	enemy->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(enemy);

	return enemy;
}
/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
				lockedObject	= nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}

	if (lockedObject) {
		renderer->DrawString("Press L to unlock object!", Vector2(5, 80));
	}

	else if(selectionObject){
		renderer->DrawString("Press L to lock selected object object!", Vector2(5, 80));
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
		if (selectionObject) {
			if (lockedObject == selectionObject) {
				lockedObject = nullptr;
			}
			else {
				lockedObject = selectionObject;
			}
		}

	}

	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void TutorialGame::MoveSelectedObject() {
	renderer->DrawString("Click Force:" + std::to_string(forceMagnitude),Vector2(10, 20));//Draw debug text at 10,20
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject)
	{
		return;//we haven't selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT))
	{
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
		{
			if (closestCollision.node == selectionObject)
			{
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}

//pathfinding test
void TutorialGame::TestPathfinding()
{
	Vector3 startPos(150, 0, 0);
	/*Vector3 endPos(30, 0, 90);*/

	bool found = gridMap->FindPath(startPos, player->GetTransform().GetPosition(), outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos))
	{
		testNodes.push_back(pos);
	}
}
void TutorialGame::DisplayPathfinding()
{
	for (int i = 1; i < testNodes.size(); ++i)
	{
		Vector3 a = testNodes[i - 1];
		Vector3 b = testNodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}
