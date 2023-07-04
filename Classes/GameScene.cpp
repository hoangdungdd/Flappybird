/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameScene.h"
#include "GameOver.h"
#include "AudioEngine.h"
#include "ReadyScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = GameScene::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	
	scene->addChild(layer);
    return scene; 
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT,5);  
	edgeBody->setCollisionBitmask(2);
	edgeBody->setContactTestBitmask(true);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	

	bird = Sprite::create("bird.png");
	bird->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	
	
	auto birdPhysics = PhysicsBody::createBox(bird->getContentSize());
	birdPhysics->setContactTestBitmask(true);
	birdPhysics->setCollisionBitmask(4);
	bird->setPhysicsBody(birdPhysics);
	this->addChild(bird);

	label = Label::createWithTTF(std::to_string(score), "fonts/flappy-font.ttf",visibleSize.height*0.1);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height *0.8));
	label->setColor(Color3B::WHITE);
	addChild(label,1);

	background = Sprite::create("background.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(background, -1);

	this->scheduleUpdate();
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::CreatePipe), 0.005* visibleSize.width);
	
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::OnTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::OnContactBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}


void GameScene::CreatePipe(float dt)
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto topPipe = Sprite::create("uppipe.png");
	auto botPipe = Sprite::create("downpipe.png");
	
	
	
	
	auto topPhysics = PhysicsBody::createBox(topPipe->getContentSize());
	topPhysics->setCollisionBitmask(2);
	topPhysics->setContactTestBitmask(true);
	topPipe->setPhysicsBody(topPhysics);
	auto botPhysics = PhysicsBody::createBox(botPipe->getContentSize());
	botPhysics->setCollisionBitmask(2);
	botPhysics->setContactTestBitmask(true);
	botPipe->setPhysicsBody(botPhysics);

	topPhysics->setDynamic(false);
	botPhysics->setDynamic(false);


	auto random = CCRANDOM_0_1();
	if (random < 0.35)
	{
		random = 0.35;
	}
	else
	{
		if (random > 0.65)
			random = 0.65;
		{
		}
	}
	auto topPipePosition = visibleSize.height*random + topPipe->getContentSize().height / 2;
	topPipe->setPosition(Vec2(visibleSize.width + topPipe->getContentSize().width, topPipePosition));//set vi tri top pipe
	botPipe->setPosition(Vec2(topPipe->getPositionX(), topPipePosition - ((Sprite::create("bird.png")->getContentSize().height * 4)) - topPipe->getContentSize().height)); // khoang cach trong giua 2 cai cot
	
	addChild(topPipe);
	addChild(botPipe);

	auto topMove = MoveBy::create(0.01*visibleSize.width,Vec2(-visibleSize.width*1.5,0));
	auto botMove = MoveBy::create(0.01*visibleSize.width, Vec2(-visibleSize.width*1.5, 0));

	topPipe->runAction(topMove);
	botPipe->runAction(botMove);
	
	auto pointNode = Node::create();
	pointNode->setPosition(Vec2(topPipe->getPositionX(),topPipe->getPositionY()-topPipe->getContentSize().height / 2 - (Sprite::create("bird.png")->getContentSize().height*5) /2 )); 
	auto nodePhysics = PhysicsBody::createBox(Size(1,(Sprite::create("bird.png")->getContentSize().height*5)));
	nodePhysics->setCollisionBitmask(3);
	nodePhysics->setContactTestBitmask(true);
	nodePhysics->setDynamic(false);
	pointNode->setPhysicsBody(nodePhysics);
	auto nodeMove = MoveBy::create(0.01*visibleSize.width, Vec2(-visibleSize.width*1.5, 0));
	pointNode->runAction(nodeMove);
	
	
	addChild(pointNode); 
}
void GameScene::update(float dt)
{
	
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (isFalling == true)
	{
		//auto curVelocity = bird->getPhysicsBody()->getVelocity();
		bird->setPositionX(visibleSize.width / 2);
		bird->setPositionY(bird->getPositionY() - visibleSize.height*0.0040);
		//bird->getPhysicsBody()->setVelocity(Vec2(0, 250 > (curVelocity.y + 500) ? (curVelocity.y + 500) : 250));
	}
	else
	{
		
		bird->setPositionX(visibleSize.width / 2);
		bird->setPositionY(bird->getPositionY() + visibleSize.height*0.0040);
	}
}

bool GameScene::OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	isFalling = false;
	AudioEngine::play2d("wing.mp3");
	this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::stopFlying), 0.25);
	return true;
}

void GameScene::stopFlying(float dt)
{
	
	isFalling = true;
}

bool GameScene::OnContactBegan(cocos2d::PhysicsContact &contact)
{
	GameOver object;
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();
	if (a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 3 || a->getCollisionBitmask() == 3 && b->getCollisionBitmask() == 4)
	{
		AudioEngine::play2d("point.mp3");
		++score;
		label->setString(std::to_string(score));
		
	}
	else
	{
		if (a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 2 || a->getCollisionBitmask() == 2 && b->getCollisionBitmask() == 4)
		{
			AudioEngine::play2d("hit.mp3");
			UserDefault *def = UserDefault::getInstance();
		    bestScore = def->getIntegerForKey("BESTSCORE", 0);
			if (score >= bestScore)
			{
				bestScore = score;
				def->setIntegerForKey("BESTSCORE", bestScore);
			}
			def->flush();
			
			auto gameOver = GameOver::createScene(score , bestScore);
			TransitionScene *transition = TransitionFade::create(1.0f, gameOver);
			Director::getInstance()->replaceScene(transition);
			
		}
	}


	return true;
}
