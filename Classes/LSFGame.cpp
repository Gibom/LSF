#include "LSFGame.h"

using namespace cocos2d;

int ropeHealth = 10000;
bool fishingStat;
Scene* LSFGame::createScene()
{
	auto scene = Scene::create();
	auto layer = LSFGame::create();
	scene->addChild(layer);

	return scene;
}

bool LSFGame::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}

	//////////////////////////////

	ropes = new std::vector<VRope*>;
	winSize = Director::getInstance()->getWinSize();
	//flow
	
	//flow
	btnCount = 0;
	cbtnCount = 0;
	waterCount = 0;
	fishingStat = false;
	ropeTickCount = false;
	craftSwitch = false;

	
	
	//스프라이트 위치설정 및 addChild
	auto GameFrameCache = SpriteFrameCache::getInstance();
	GameFrameCache->addSpriteFramesWithJson("Sprites/Game.json");

	back = Sprite::createWithSpriteFrame(GameFrameCache->getSpriteFrameByName("Game 0.png"));
	back->setAnchorPoint(Vec2::ZERO);
	back->setPosition(Vec2(0,100));
	//this->addChild(back);

	auto weatherFrameCache = SpriteFrameCache::getInstance();
	weatherFrameCache->addSpriteFramesWithJson("Sprites/RainDrop.json");

	rainDrop = Sprite::createWithSpriteFrame(weatherFrameCache->getSpriteFrameByName("RainDrop 0.png"));
	rainDrop->setAnchorPoint(Vec2(0.5, 0.5));
	rainDrop->setPosition(Vec2(180, 320));
	this->addChild(rainDrop);

	weatherFrameCache = SpriteFrameCache::getInstance();
	weatherFrameCache->addSpriteFramesWithJson("Sprites/SnowDrop.json");

	snowDrop = Sprite::createWithSpriteFrame(weatherFrameCache->getSpriteFrameByName("SnowDrop 0.png"));
	snowDrop->setAnchorPoint(Vec2(0.5, 0.5));
	snowDrop->setPosition(Vec2(190, 320));
	this->addChild(snowDrop);

	//가방 레이어 추가
	//가방------------------------------------------------------------------------------------------------
	invenLayer = LayerColor::create(Color4B(255, 255, 255, 125),
		winSize.width, winSize.height);
	invenLayer->setAnchorPoint(Vec2::ZERO);
	invenLayer->setPosition(Vec2(0, 0));
	invenLayer->setVisible(false);
	//invenLayer->setScaleY(2.f);
	//invenLayer->setCascadeOpacityEnabled(true);
	this->addChild(invenLayer, 4);

	craftUsel = Sprite::create("Sprites/inventory_bg2.png");
	craftUsel->setAnchorPoint(Vec2::ZERO);
	craftUsel->setPosition(Vec2::ZERO);
	craftUsel->setCascadeOpacityEnabled(true);
	craftUsel->setOpacity(125);
	invenLayer->addChild(craftUsel);

	craftSel = Sprite::create("Sprites/inventory_bg3.png");
	craftSel->setAnchorPoint(Vec2::ZERO);
	craftSel->setPosition(Vec2::ZERO);
	craftSel->setCascadeOpacityEnabled(true);
	craftSel->setOpacity(125);
	craftSel->setVisible(false);
	invenLayer->addChild(craftSel);


	inventory = Sprite::create("Sprites/inventory_bg.png");
	inventory->setAnchorPoint(Vec2::ZERO);
	inventory->setPosition(Vec2::ZERO);
	//inventory->setScale(0.5f,1.5f);
	inventory->setCascadeOpacityEnabled(true);
	inventory->setOpacity(255);
	inventory->setVisible(false);
	invenLayer->addChild(inventory);

	GameFrameCache = SpriteFrameCache::getInstance();
	GameFrameCache->addSpriteFramesWithJson("Sprites/Button_craft.json");

	craft = Sprite::createWithSpriteFrame(GameFrameCache->getSpriteFrameByName("Button_craft 0.png"));
	craft->setAnchorPoint(Vec2(0.5, 0.5));
	craft->setPosition(Vec2(130, winSize.height / 5 - 100));
	craft->setScale(1.5f);
	//craft->setVisible(false);
	invenLayer->addChild(craft);
	//가방----------------------------------------------------------------------------------------------------


	//환경 구조물배치-----------------------------------------------------------------------------------------
	weatherCount = 2;
	auto ShipFrameCache = SpriteFrameCache::getInstance();
	if (weatherCount == 1) {
		ShipFrameCache->addSpriteFramesWithJson("Sprites/Ship_normal.json");
	}
	else if (weatherCount == 2) {
		ShipFrameCache->addSpriteFramesWithJson("Sprites/Ship_windy.json");
	}
	//ShipFrameCache->addSpriteFramesWithJson("Sprites/Ship_normal.json");
	ship = Sprite::createWithSpriteFrame(ShipFrameCache->getSpriteFrameByName("Ship 0.png"));
	ship->setAnchorPoint(Vec2(0.5, 0.5));
	ship->setPosition(Vec2(winSize.width-120, winSize.height/3));
	ship->setLocalZOrder(2);
	ship->setScale(1.5f);
	this->addChild(ship);

	fisherman = Sprite::create("Sprites/Fisherman.png");
	fisherman->setAnchorPoint(Vec2::ZERO);
	fisherman->setPosition(Vec2(-4, 24));
	fisherman->setLocalZOrder(1);
	ship->addChild(fisherman);



	//환경 구조물배치----------------------------------------------------------------------------------------

	//메뉴
	btn_inventory = MenuItemImage::create("Sprites/Button_bagclose.png",
		"Sprites/Button_bagopen.png",
		CC_CALLBACK_1(LSFGame::doPushInventory, this));
	btn_inventory->setScale(1.5f);
	inventoryMenu = Menu::create(btn_inventory, nullptr);
	inventoryMenu->setAnchorPoint(Vec2(0.5, 0.5));
	inventoryMenu->setPosition(Vec2(winSize.width-140, winSize.height/5-100));
	inventoryMenu->alignItemsHorizontally();
	this->addChild(inventoryMenu, 4);

	////애니메이션
	//Background
	auto mainAnim = animCreate->CreateAnim("Sprites/Game.json", "Game", 15, 0.1f);
	auto mainAnimate = Animate::create(mainAnim);
	auto repMain = RepeatForever::create(mainAnimate);
	back->runAction(repMain);

	//Ship	(Normal(1), Windy(2), Snowy(3), Rainny(4), ThunderStorm(5))
	ShipStat = 2;

	if(ShipStat == 1){
		if (ship->getNumberOfRunningActions() != 0) {
			ship->cleanup();
		}
		auto shipNormalAnim = animCreate->CreateAnim("Sprites/Ship_normal.json", "Ship", 5, 0.1f);
		auto shipAnimate = Animate::create(shipNormalAnim);
		auto repShip = RepeatForever::create(shipAnimate);
		ship->runAction(repShip);
	}
	else if(ShipStat == 2){
		if (ship->getNumberOfRunningActions() != 0) {
			ship->cleanup();
		}
		auto shipWindyAnim = animCreate->CreateAnim("Sprites/Ship_windy.json", "Ship", 5, 0.1f);
		auto shipAnimate = Animate::create(shipWindyAnim);
		auto repShip = RepeatForever::create(shipAnimate);
		ship->runAction(repShip);
	}

	//Button_craft
	auto craftAnim = animCreate->CreateAnim("Sprites/Button_craft.json", "Button_craft", 3, 0.1f);
	auto craftAnimate = Animate::create(craftAnim);
	auto repCraft = RepeatForever::create(craftAnimate);
	craft->runAction(repCraft);

	//Weather effect	(Wind, Rain, Snow, Tunder)
	//Rain
	auto rainAnim = animCreate->CreateAnim("Sprites/RainDrop.json", "RainDrop", 3, 0.1f);
	auto rainAnimate = Animate::create(rainAnim);
	auto repRain = RepeatForever::create(rainAnimate);
	rainDrop->runAction(repRain);

	//Snow
	auto snowAnim = animCreate->CreateAnim("Sprites/SnowDrop.json", "SnowDrop", 3, 0.1f);
	auto snowAnimate = Animate::create(snowAnim);
	auto repSnow = RepeatForever::create(snowAnimate);
	snowDrop->runAction(repSnow);


	//월드 생성
	if (this->createBox2dWorld(true))
	{
		this->schedule(schedule_selector(LSFGame::tick));
		water = WaterNode::create();
		this->addChild(water, 3);
	}

	return true;


}

void LSFGame::onEnter()
{
	Layer::onEnter();
	//Touch 
	//싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	//Swallow touches only available in OneByOne mode.
	//핸들링된 터치 이벤트를 터치 이벤트 array에서 지우겠다는 의미다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(LSFGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(LSFGame::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(LSFGame::onTouchEnded, this);

	// The prioriry of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void LSFGame::onExit()
{
	_eventDispatcher->removeAllEventListeners();
	Layer::onExit();
}
bool LSFGame::onTouchBegan(Touch* touch, Event* event)
{
	weatherCount = 1;
	Vec2 touchPoint = touch->getLocation();

	log("onTOuchBegan id = %d, x = %f, y= %f", touch->getID(), touchPoint.x, touchPoint.y);

	//touch check --------------------------------
	bool bTouch_craft = craft->getBoundingBox().containsPoint(touchPoint);
	bool bTouch_mode; //모드 스위치 구현 시 사용

	// 게임 화면
	if (cbtnCount == 0) {

		if (fishingStat == false) {
			b2Body* body1 = this->addNewSpriteAt(touchPoint,"Sprites/needle.png", 1);
			Vec2 fVec = fisherman->convertToWorldSpace(fisherman->getPosition());

			this->createRope(groundBody,
				b2Vec2((fVec.x + 16) / PTM_RATIO, (fVec.y - 4) / PTM_RATIO),
				body1,
				body1->GetLocalCenter(),
				1.1f);

			ropeTouchCount = true;

		}
	}
	// 가방이 열려있고 craft가 선택 됐을 때
	if (bTouch_craft && cbtnCount == 1)
	{
		if (craftSwitch == true)
		{
			craftUsel->setVisible(true);
			craftSel->setVisible(false);
			craftSwitch = false;
			//log("craftSwitch Status: Off", craftSwitch);
		}
		else {
			craftUsel->setVisible(false);
			craftSel->setVisible(true);
			craftSwitch = true;
			//log("craftSwitch Status: On", craftSwitch);
		}

	}

	return true;
}
void LSFGame::onTouchMoved(Touch* touch, Event* event)
{

}
void LSFGame::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();
	touchRope = touchPoint;
	float test;

	test = touchPoint.y;
	log("%f", test);
	if (cbtnCount != 1 && fishingStat == false) {
		this->scheduleOnce(schedule_selector(LSFGame::ropeTouch), test / 460);
		fishingStat = true;
	}
	//log("onTouchEnded id = %d, x = %f, y = %f", touch->getID(), touchPoint.x, touchPoint.y);
}
void LSFGame::doPushInventory(Ref * pSender)
{
	if (btnCount == 0) {
		invenLayer->setVisible(true);
		inventory->setVisible(true);
		//craft->setVisible(true);
		btn_inventory->selected();
		btnCount++;
		cbtnCount = 1;
	}
	else {
		invenLayer->setVisible(false);
		inventory->setVisible(false);
		//craft->setVisible(false);
		btn_inventory->unselected();
		btnCount = 0;
		cbtnCount = 0;
	}
}
bool LSFGame::createBox2dWorld(bool debug)
{
	//월드 생성 시작-----------------------------------------------------

	//중력의 방향을 결정한다.
	b2Vec2 gravity = b2Vec2(0.0f, -5.0f);

	//월드를 생성한다.
	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	//디버그 드로잉 설정
	if (debug) {
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;
		m_debugDraw->SetFlags(flags);
	}
	//휴식 상태일 때 포함된 바디들을 멈추게(sleep)할 것인지 결정한다.
	_world->SetAllowSleeping(true);
	//지속적인 물리작용을 할 것인지 결정한다. : 테스트
	_world->SetContinuousPhysics(true);
	////가장자리(테두리)를 지정해 공간 (Ground Box)을 만든다.
	//바디데프에 좌표를 설정한다.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	//월드에 바디데프의 정보(좌표)로 바디를 만든다.
	groundBody = _world->CreateBody(&groundBodyDef);

	//가장자리(테두리) 경계선을 그릴 수 있는 모양의 객체를 만든다.
	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;
	//에지 모양의 객체에 Set( 점1, 점2)로 선을 만든다.
	//그리고 바디(groundBody)에 모양(groundEdge)을 고정시킨다.

	//바다 아래
	groundEdge.Set(b2Vec2(0, 3.4f), b2Vec2(winSize.width / PTM_RATIO, 3.4f));
	groundBody->CreateFixture(&boxShapeDef);
	groundEdge.Set(b2Vec2(0, 2.8f), b2Vec2(winSize.width / PTM_RATIO, 2.8f));
	groundBody->CreateFixture(&boxShapeDef);
	groundEdge.Set(b2Vec2(0, 2.2f), b2Vec2(winSize.width / PTM_RATIO, 2.2f));
	groundBody->CreateFixture(&boxShapeDef);
	groundEdge.Set(b2Vec2(0, 1.6f), b2Vec2(winSize.width / PTM_RATIO, 1.6f));
	groundBody->CreateFixture(&boxShapeDef);
	groundEdge.Set(b2Vec2(0, 1.1f), b2Vec2(winSize.width / PTM_RATIO, 1.1f));
	groundBody->CreateFixture(&boxShapeDef);

	//왼쪽
	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//위쪽
	groundEdge.Set(b2Vec2(0, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//오른쪽
	groundEdge.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO),
		b2Vec2(winSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);


	//바닥에 직사각형 지형을 만든다.
	b2BodyDef bottomBodyDef;
	bottomBodyDef.type = b2_dynamicBody;
	bottomBodyDef.position.Set(winSize.width/2/PTM_RATIO, 1.2f);
	bottomBodyDef.linearDamping = 0.3f;
	
	b2Body* bottomBody;
	bottomBody = _world->CreateBody(&bottomBodyDef);
	
	b2FixtureDef botfixtureDef;
	b2PolygonShape edgeBox;
		
	edgeBox.SetAsBox(3.4f, 0.18f);
	botfixtureDef.shape = &edgeBox;
	botfixtureDef.density = 1.0f;
	botfixtureDef.friction = 0;
	botfixtureDef.restitution = 0.0;
	//botfixtureDef.filter.categoryBits = 0x02;
	//botfixtureDef.filter.groupIndex = 1;
	bottomBody->CreateFixture(&botfixtureDef);
	
	//월드 생성 끝 ---------------------------------------------------


	//Flow
	//top
	flowBody0 = this->addNewSpriteFlow(Vec2((winSize.width / 2), winSize.height /3-100), Size(100, 5), b2_dynamicBody, "fish1", 0);
	flowBody1 = this->addNewSpriteFlow(Vec2((winSize.width / 2)-40, winSize.height / 3 -120), Size(10, 10), b2_dynamicBody, nullptr, 0);
	flowBody2 = this->addNewSpriteFlow(Vec2((winSize.width / 2)+40, winSize.height / 3 -120), Size(10, 10), b2_dynamicBody, nullptr, 0);

	b2Vec2 axis(0.0f, 1.0f);

	flowJd1.Initialize(flowBody0, flowBody1, flowBody1->GetPosition(), axis);
	flowJd1.motorSpeed = 15.0f;
	flowJd1.maxMotorTorque = 15.0f;
	flowJd1.enableMotor = true;
	flowJd1.frequencyHz = 4.0f;
	flowJd1.dampingRatio = 0.7f;


	flowJd2.Initialize(flowBody0, flowBody2, flowBody2->GetPosition(), axis);
	flowJd2.motorSpeed = 15.0f;
	flowJd2.maxMotorTorque = 15.0f;
	flowJd2.enableMotor = true;
	flowJd2.frequencyHz = 4.0f;
	flowJd2.dampingRatio = 1.0f;

	//center
	flowBody3 = this->addNewSpriteFlow(Vec2((winSize.width / 2), winSize.height/3 -160), Size(100, 10), b2_dynamicBody, "fish1", 0);
	flowBody4 = this->addNewSpriteFlow(Vec2((winSize.width / 2)-40, winSize.height/3 -180), Size(20, 20), b2_dynamicBody, nullptr, 0);
	flowBody5 = this->addNewSpriteFlow(Vec2((winSize.width / 2)+40, winSize.height/3 -180), Size(20, 20), b2_dynamicBody, nullptr, 0);

	flowJd3.Initialize(flowBody3, flowBody4, flowBody4->GetPosition(), axis);
	flowJd3.motorSpeed = 25.0f;
	flowJd3.maxMotorTorque = 25.0f;
	flowJd3.enableMotor = true;
	flowJd3.frequencyHz = 4.0f;
	flowJd3.dampingRatio = 0.7f;


	flowJd4.Initialize(flowBody3, flowBody5, flowBody5->GetPosition(), axis);
	flowJd4.motorSpeed = 25.0f;
	flowJd4.maxMotorTorque = 25.0f;
	flowJd4.enableMotor = true;
	flowJd4.frequencyHz = 4.0f;
	flowJd4.dampingRatio = 1.0f;

	//bottom
	flowBody6 = this->addNewSpriteFlow(Vec2((winSize.width / 2), winSize.height/3-220), Size(100, 15), b2_dynamicBody, "fish1", 0);
	flowBody7 = this->addNewSpriteFlow(Vec2((winSize.width / 2)-40, winSize.height/3 - 240), Size(20, 20), b2_dynamicBody, nullptr, 0);
	flowBody8 = this->addNewSpriteFlow(Vec2((winSize.width / 2)+40, winSize.height/3 - 240), Size(20, 20), b2_dynamicBody, nullptr, 0);


	flowJd5.Initialize(flowBody6, flowBody7, flowBody7->GetPosition(), axis);
	flowJd5.motorSpeed = 30.0f;
	flowJd5.maxMotorTorque = 30.0f;
	flowJd5.enableMotor = true;
	flowJd5.frequencyHz = 4.0f;
	flowJd5.dampingRatio = 1.0f;

	flowJd6.Initialize(flowBody6, flowBody8, flowBody8->GetPosition(), axis);
	flowJd6.motorSpeed = 20.0f;
	flowJd6.maxMotorTorque = 30.0f;
	flowJd6.enableMotor = true;
	flowJd6.frequencyHz = 4.0f;
	flowJd6.dampingRatio = 0.7f;

	m_spring1 = (b2WheelJoint*)_world->CreateJoint(&flowJd1);
	m_spring2 = (b2WheelJoint*)_world->CreateJoint(&flowJd2);
	m_spring3 = (b2WheelJoint*)_world->CreateJoint(&flowJd3);
	m_spring4 = (b2WheelJoint*)_world->CreateJoint(&flowJd4);
	m_spring5 = (b2WheelJoint*)_world->CreateJoint(&flowJd5);
	m_spring6 = (b2WheelJoint*)_world->CreateJoint(&flowJd6);
	//Flow



	//컨택 리스너
	myContactListener = new ContactListener();
	_world->SetContactListener((b2ContactListener*)myContactListener);

	//밧줄 이미지 로드
	ropeSpriteSheet = SpriteBatchNode::create("Sprites/rope_texture.png");
	this->addChild(ropeSpriteSheet);

	//바디 아이템 추가
	//Add a bunch of ropes
	/*this->createRope(groundBody, b2Vec2((winSize.width / 2) / PTM_RATIO, winSize.height / PTM_RATIO),
		body1,
		body1->GetLocalCenter(),
		1.1f);*/
	return true;
}
b2Body* LSFGame::addNewSpriteAt(Vec2 point, const std::string & imagepath, int tag)
{
	b2BodyDef bodyDef;
	b2Body* body;
	b2FixtureDef fixtureDef;
	b2CircleShape spriteShape;
	//Get the sprite frome the sprite sheet
	Sprite* sprite = Sprite::create(imagepath);
	sprite->setAnchorPoint(Vec2(0.5, 0.8));
	this->addChild(sprite);

	
	//Defines the body of needle
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = sprite;
	bodyDef.linearDamping = 0.3f;
	body = _world->CreateBody(&bodyDef);
	if (tag == 1) {
		spriteShape.m_radius = 0.1;
		fixtureDef.shape = &spriteShape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.2f;
		fixtureDef.restitution = 0.7f;
		fixtureDef.filter.categoryBits = 0x01;
		fixtureDef.filter.groupIndex = -1;
		fixtureDef.filter.groupIndex = 1;
	}
	else if (tag == 2) {
		spriteShape.m_radius = 0.03;
		fixtureDef.shape = &spriteShape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.2f;
		fixtureDef.restitution = 0.7f;
		fixtureDef.filter.categoryBits = 0x02;
	}

	body->CreateFixture(&fixtureDef);

	return body;
}
int flowCount = 0;
b2Body* LSFGame::addNewSpriteFlow(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type)
{
	//스프라이트를 파라미터로 넘어온 위치에 만든다.
	//Sprite* pSprite = Sprite::createWithTexture(texture, Rect(0, 0, 37, 37));
	//pSprite->setPosition(Vec2(location.x, location.y));
	//this->addChild(pSprite);

	//바디데프를 만들고 속성들을 지정한다.
	b2BodyDef bodyDef;
	bodyDef.type = bodytype;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

	if (spriteName) {
		if (strcmp(spriteName, "fish1") == 0) {
			//int idx = (CCRANDOM_0_1() > .5 ? 0 : 1);
			//int idy = (CCRANDOM_0_1() > .5 ? 0 : 1);
			auto flowSprite = Sprite::createWithTexture(texture = Director::getInstance()->getTextureCache()->addImage("Sprites/Fishes/Fish011.png"),
				Rect(0, 0, 24, 16));
			flow.pushBack(flowSprite);
			flow.at(flowCount)->setPosition(point);
			flow.at(flowCount)->setTag(flowCount);
			this->addChild(flow.at(flowCount));

			bodyDef.userData = flow.at(flowCount);
			flowCount++;
		}
		else {
			Sprite* flow = Sprite::create(spriteName);
			flow->setPosition(point);
			this->addChild(flow);

			bodyDef.userData = flow;
		}
	}
	//월드에 바디데프의 정보로 바디를 만든다.
	b2Body* body = _world->CreateBody(&bodyDef);

	//바디에 적용할 물리 속성용 바디의 모양을 만든다.
	b2FixtureDef fixtureDef;
	b2CircleShape circle;			//원 형태를 선택해 반지름을 지정한다.
	b2PolygonShape dynamicBox;

	if (type == 0) {
		dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

		fixtureDef.shape = &dynamicBox;
	}
	else {
		circle.m_radius = (size.width / 2) / PTM_RATIO;

		fixtureDef.shape = &circle;
	}

	fixtureDef.density = 1.0f;		//밀도
	fixtureDef.friction = 0.2f;		//마찰력 - 0 ~ 1
	fixtureDef.restitution = 0.5f;	//반발력 - 물체가 다른 물체에 닿았을 때 튕기는 값
	fixtureDef.filter.categoryBits = 0x03;
	fixtureDef.filter.groupIndex = -1;
	body->CreateFixture(&fixtureDef);

	return body;
}

VRope* newRope;
b2RopeJoint* ropeJoint;
void LSFGame::createRope(b2Body* bodyA, b2Vec2 anchorA, b2Body* bodyB, b2Vec2 anchorB, float32 sag)
{
	b2RopeJointDef jd;
	jd.bodyA = bodyA;
	jd.bodyB = bodyB;
	jd.localAnchorA = anchorA;
	jd.localAnchorB = anchorB;

	//Max length of joint = current distance between bodies * sag
	float32 ropeLength = (bodyA->GetWorldPoint(anchorA) - bodyB->GetWorldPoint(anchorB)).Length()*sag;
	log("ropeLength: %f", ropeLength);
	if (ropeLength >= 1 && ropeLength <= 4) {
		log("ropeLength: %f", ropeLength);
		log("ropeLength: %f", ropeLength);
		log("ropeLength: %f", ropeLength);
		jd.maxLength = ropeLength;
		ropeJoint = (b2RopeJoint*)_world->CreateJoint(&jd);

		newRope = new VRope(ropeJoint, ropeSpriteSheet);
		ropes->push_back(newRope);

		return;
	}
	else if (ropeLength < 1 || ropeLength>4) {
		log("else if ropeLength: %f", ropeLength);
		log("else if ropeLength: %f", ropeLength);
		log("else if ropeLength: %f", ropeLength);
		jd.maxLength = 4;
		ropeJoint = (b2RopeJoint*)_world->CreateJoint(&jd);

		newRope = new VRope(ropeJoint, ropeSpriteSheet);
		ropes->push_back(newRope);
		return;
	}
	else {

		return;
	}

	//Create Joints..

}
b2Body* LSFGame::createRopeTipBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 0.5f;
	b2Body* body = _world->CreateBody(&bodyDef);

	b2FixtureDef circleDef;
	b2CircleShape circle;
	circle.m_radius = 0.4f / PTM_RATIO;
	circleDef.shape = &circle;
	circleDef.density = 10.0f;

	//Since these tips don't have to collide with anything
	//set the mask bits to zero
	circleDef.filter.maskBits = 0;
	body->CreateFixture(&circleDef);
	return body;
}
void LSFGame::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(LSFGame::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}
void LSFGame::onDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	//draw
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void LSFGame::tick(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;

	//Step: 물리 세계를 시뮬레이션한다.
	_world->Step(dt, velocityIterations, positionIterations);

	//모든 물리 객체들은 링크드 리스트에 저장되어 참조해 볼 수 있게 구현돼 있다.
	//만들어진 객체만큼 루프를 돌리면서 바디에 붙인 스프라이트를 여기서 제어한다.
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr) {
			Sprite* spriteData = (Sprite *)b->GetUserData();
			spriteData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y *PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
	//RUT(Rope Update Timer)
	if (ropeTickCount == false) {
		//this->schedule(schedule_selector(LSFGame::ropeTick), 0.095);
		this->schedule(schedule_selector(LSFGame::ropeTick));
		ropeTickCount = true;
	}
	/*
	//WA(Water Adder)
	if (waterCount < 30) {
		//top
		water1 = this->addNewSpriteAt(Vec2((winSize.width / 2), (winSize.height / 3 - 110)), "Sprites/WaterSplash.png",2);
		water1 = this->addNewSpriteAt(Vec2((winSize.width / 2) - 2, (winSize.height / 3 - 110)), "Sprites/WaterSplash.png", 2);
		//center
		water2 = this->addNewSpriteAt(Vec2((winSize.width / 2), (winSize.height / 3 - 170)), "Sprites/WaterSplash.png", 2);
		water2 = this->addNewSpriteAt(Vec2((winSize.width / 2) - 2, (winSize.height / 3 - 170)), "Sprites/WaterSplash.png", 2);
		//bottom
		water3 = this->addNewSpriteAt(Vec2((winSize.width / 2), (winSize.height / 3 - 230)), "Sprites/WaterSplash.png", 2);
		water3 = this->addNewSpriteAt(Vec2((winSize.width / 2) - 2, (winSize.height / 3 - 230)), "Sprites/WaterSplash.png", 2);
		water3 = this->addNewSpriteAt(Vec2((winSize.width / 2), (winSize.height / 3 - 230)), "Sprites/WaterSplash.png", 2);
		water3 = this->addNewSpriteAt(Vec2((winSize.width / 2) - 2, (winSize.height / 3 - 230)), "Sprites/WaterSplash.png", 2);
		waterCount++;
	}
	*/
	//WF(Water flow)
	//log("flowbody %f", flowBody0->GetPosition().x);
	
	if (flowBody0->GetPosition().x <=0.6)
	{
		m_spring1->SetMotorSpeed(-30.0f);
		m_spring2->SetMotorSpeed(-30.0f);
		flow.at(0)->setFlippedX(true);
		
	}
	else if (flowBody0->GetPosition().x >= 6.6) {
		m_spring1->SetMotorSpeed(30.0f);
		m_spring2->SetMotorSpeed(30.0f);
		flow.at(0)->setFlippedX(false);
	}

	if (flowBody3->GetPosition().x <= 0.6)
	{
		m_spring3->SetMotorSpeed(-25.0f);
		m_spring4->SetMotorSpeed(-25.0f);
		flow.at(1)->setFlippedX(true);
	}
	else if (flowBody3->GetPosition().x >= 6.6) {
		m_spring3->SetMotorSpeed(25.0f);
		m_spring4->SetMotorSpeed(25.0f);
		flow.at(1)->setFlippedX(false);
	}

	if (flowBody6->GetPosition().x <= 0.6)
	{
		m_spring5->SetMotorSpeed(-20.0f);
		m_spring6->SetMotorSpeed(-20.0f);
		flow.at(2)->setFlippedX(true);
	}
	else if (flowBody6->GetPosition().x >= 6.6) {
		m_spring5->SetMotorSpeed(20.0f);
		m_spring6->SetMotorSpeed(20.0f);
		flow.at(2)->setFlippedX(false);
	}
	


	//RHC(Rope Health Counter)
	if (ropeHealth == 0) {
		log("Fishing fail!");
		ropeHealth = 10000;
		ropes->clear();
		_world->DestroyJoint(ropeJoint);
		this->unschedule(schedule_selector(LSFGame::ropeTick));
		newRope->removeSprites();
		fishingStat = false;
		ropeTickCount = false;
	}

}
void LSFGame::ropeTick(float dt)
{
	//밧줄 시뮬레이션
	std::vector<VRope *>::iterator rope;
	for (rope = ropes->begin(); rope != ropes->end(); ++rope)
	{

		(*rope)->update(dt);
		(*rope)->updateSprites();
	}
}
void LSFGame::ropeTouch(float dt) {
	/*WaterSplash Effect Sound 추가 예정*/
	water->splash(touchRope.x, -100);
}
void ContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();


	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();

	if (bodyA->GetType() == b2_dynamicBody || bodyB->GetType() == b2_dynamicBody) {
		//log("needle & water contact .. ");
		if (ropeHealth > 0 && fishingStat == true)
		{
			ropeHealth--;
			log("ropeHealth :%d", ropeHealth);
		}
	}
}
int LSFGame::statusCheck(const std::string & kindof)
{
	if (kindof == "ship")
	{

	}
	return 0;
}
LSFGame::~LSFGame()
{
	//월드를 C++의 new로 생성했으므로 여기서 지워준다.
	delete _world;
	_world = nullptr;
}

