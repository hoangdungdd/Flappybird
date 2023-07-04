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

#include "GameOver.h"
#include "GameScene.h"
#include "ReadyScene.h"
USING_NS_CC;

Scene* GameOver::createScene(int score , int bestScore)
{

	auto scene = Scene::create();
	auto layer = GameOver::create(score , bestScore);
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
bool GameOver::init(int score , int bestScore)
{
	
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	Node *gameOverPanelNode = Node::create();
	
	auto gameOverLabel = Sprite::create("gameover.png");
	//gameOverLabel->setPosition(origin.x + visibleSize.width / 2, 500);
	gameOverPanelNode->addChild(gameOverLabel);

	auto panel = Sprite::create("board.png");
	gameOverLabel->setPositionY(panel->getContentSize().height);
	gameOverPanelNode->addChild(panel);

	
	auto curScore = Label::createWithTTF(std::to_string(score), "fonts/flappy-font.ttf", 20);// hien thi diem
	curScore->setColor(Color3B::WHITE);
	this->addChild(curScore, 4);

	auto bestScoreLabel = Label::createWithTTF(std::to_string(bestScore), "fonts/flappy-font.ttf", 20);
	bestScoreLabel->setColor(Color3B::WHITE);
	this->addChild(bestScoreLabel, 3);
	
	curScore->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height);
	curScore->runAction(MoveTo::create(2.0f, Vec2(363, 363)));
	bestScoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height);
	bestScoreLabel->runAction(MoveTo::create(2.0f, Vec2(363, 323)));

	this->addChild(gameOverPanelNode,2);
	gameOverPanelNode->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height);
	gameOverPanelNode->runAction(MoveTo::create(2.0f, Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2)));
	

	auto play = MenuItemImage::create("start_btn.png", "start_btn_pressed.png", CC_CALLBACK_1(GameOver::restartGame, this));
	auto menu = Menu::create(play, NULL);
	menu->setPosition(origin.x + visibleSize.width / 2, 250); 
	this->addChild(menu, 1);

	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background);

    return true;
}

void GameOver::restartGame(Ref* pSender)
{
	auto gameScene = ReadyScene::createScene();
	TransitionScene *transition = TransitionFade::create(1.0f, gameScene);
	Director::getInstance()->replaceScene(transition);
}


GameOver * GameOver::create(int score , int bestScore) 
{ 

    GameOver * pRet = new(std::nothrow) GameOver(); 
    if (pRet && pRet->init(score , bestScore)) 
    { 
    pRet->autorelease(); 
    return pRet; 
} 
    else 
    { 
    delete pRet; 
    pRet = nullptr; 
    return nullptr; 
    } 
}
