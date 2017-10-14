#include "Gluttony.h"
#include "Board.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Gluttony::createScene()
{
    return Gluttony::create();
}

bool Gluttony::init()
{
    
    if ( !Scene::init() )
    {
        return false;
    }
    
    addChild(drawNode);
    configKeyboard();
    
    auto screenSize = Director::getInstance()->getVisibleSize();
    borders = screenSize / (charactersSize * 2);
    
    loadGame();

    return true;
}
static auto puntos = 0;
static auto speed = 1;

void Gluttony::loadGame()
{
	
    speed = 1.0f;
    this->schedule(schedule_selector(Gluttony::update), speed);
    
    playerDirection = RIGHT;
    auto screenSize = Director::getInstance()->getVisibleSize();
    playerPosition = screenSize / (charactersSize * 4);
    drawCharacter(PLAYER, playerPosition);
	
    createPoint();
	createEnemy();
}

void Gluttony::configKeyboard()
{
    auto eventListener = EventListenerKeyboard::create();
    
    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch(keyCode)
        {
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                playerDirection = UP;
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                playerDirection = DOWN;
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                playerDirection = RIGHT;
                break;
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                playerDirection = LEFT;
                break;
            default:
                break;
        }
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void Gluttony::update(float deltaTime)
{
	Size screenSize = Director::getInstance()->getVisibleSize();
    movePlayer();

	if (playerPosition.x==pointPosition.x && playerPosition.y == pointPosition.y) {
		auto labelPoints = Label::create();
		removeChild(labelPoints);
		labelPoints->setPosition(Vec2(screenSize / 3.f * 2.f));
		puntos++;
		std::string str = std::to_string(puntos);
		labelPoints->setString(str);
		labelPoints->setWidth(20);
		labelPoints->setSystemFontSize(24);
		addChild(labelPoints);
		createPoint();
		createEnemy();
		speed -= 0.1;
		this->unschedule(schedule_selector(Gluttony::update));
		this->schedule(schedule_selector(Gluttony::update), speed);

	}
	if (playerPosition.x == enemyPosition.x && playerPosition.y == enemyPosition.y) {
		auto scene = Board::create();
		Director::getInstance()->replaceScene(scene);
	}

	if (playerPosition.x > 24|| playerPosition.x <0 || playerPosition.y < 0 || playerPosition.y > 17) {
		auto scene = Board::create();
		Director::getInstance()->replaceScene(scene);
	}

    drawNode->clear();
    
    drawCharacter(PLAYER, playerPosition);
    drawCharacter(POINT, pointPosition);
	drawCharacter(ENEMY, enemyPosition);
}

void Gluttony::drawCharacter(CharacterType type, Vec2 gridPosition)
{
    Color4F color;
    switch (type) {
        case PLAYER:
            color = Color4F::WHITE;
            break;
        case POINT:
            color = Color4F::GREEN;
            break;
        case ENEMY:
            color = Color4F::RED;
            break;
    }
    drawNode->drawDot(gridPosition * (charactersSize * 2) + Vec2(charactersSize, charactersSize), charactersSize, color);
}

void Gluttony::movePlayer()
{
    switch (playerDirection) {
        case UP:
            playerPosition.y++;
            break;
        case DOWN:
            playerPosition.y--;
            break;
        case RIGHT:
            playerPosition.x++;
            break;
        case LEFT:
            playerPosition.x--;
            break;
    }
}

void Gluttony::createPoint() {
    pointPosition = Vec2(cocos2d::random() % (int) borders.x, cocos2d::random() % (int) borders.y);
    drawCharacter(POINT, pointPosition);
}

void Gluttony::createEnemy() {
	enemyPosition = Vec2(cocos2d::random() % (int)borders.x, cocos2d::random() % (int)borders.y);
	drawCharacter(ENEMY, enemyPosition);
}


