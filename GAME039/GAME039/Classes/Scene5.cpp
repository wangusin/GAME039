#include "Scene5.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

static const char* vagetables[8][3] = {
    {"p0501p05010001", "02浮/01紅K/p0501p050200%02d.png", "03沉/01紅K/p0501p050300%02d.png"},
    {"p0501p05010002", "02浮/02白K/p0501p050200%02d.png", "03沉/02白K/p0501p050300%02d.png"},
    {"p0501p05010003", "02浮/03萵苣/p0501p050200%02d.png", "03沉/03萵苣/p0501p050300%02d.png"},
    {"p0501p05010004", "02浮/04馬鈴薯/p0501p050200%02d.png", "03沉/04馬鈴薯/p0501p050300%02d.png"},
    {"p0501p05010005", "02浮/05大白菜/p0501p050200%02d.png", "03沉/05大白菜/p0501p050300%02d.png"},
    {"p0501p05010006", "02浮/06玉米/p0501p050200%02d.png", "03沉/06玉米/p0501p050300%02d.png"},
    {"p0501p05010007", "02浮/07葡萄/p0501p050200%02d.png", "03沉/07葡萄/p0501p050300%02d.png"},
    {"p0501p05010008", "02浮/08蘋果/p0501p050200%02d.png", "03沉/08蘋果/p0501p050300%02d.png"}};

static int vagetableStartActionNumber[8] = {1, 13, 25, 37, 49, 61, 73, 85};

static bool vagetableLock[8] = {false, false, false, false, false, false, false, false};

std::string vagetableInNormalName[8] = {"p0501p05020001", "p0501p05020013", "p0501p05020025", "p0501p05020037", "p0501p05020049", "p0501p05020061", "p0501p05020073", "p0501p05020085"};

static const char* rightAnswer[4][2] = {{"01鹽/p0502g0401.png", "01鹽/p0502g0401%s.png"}, {"02糖/p0502g0402.png", "02糖/p0502g0402%s.png"}, {"03蘇打粉/p0502g0403.png", "03蘇打粉/p0502g0403%s.png"}, {"04奶粉/p0502g0404.png", "04奶粉/p0502g0404%s.png"}};
static const char* wrongAnswer[8][2] = {{"05棉花/p0502g0405.png", "05棉花/p0502g0405%s.png"}, {"06布/p0502g0406.png", "06布/p0502g0406%s.png"}, {"07米/p0502g0407.png", "07米/p0502g0407%s.png"}, {"08書/p0502g0408.png", "08書/p0502g0408%s.png"}, {"09木頭/p0502g0409.png", "09木頭/p0502g0409%s.png"}, {"10蘋果/p0502g0410.png", "10蘋果/p0502g0410%s.png"}, {"11泡麵/p0502g0411.png", "11泡麵/p0502g0411%s.png"}, {"12海綿/p0502g0412.png", "12海綿/p0502g0412%s.png"}};

Scene5::Scene5(): functionMenu(NULL), actorsLines(NULL), goToGameMenu(NULL), imageCache(NULL), storyLayer(NULL), gameLayer(NULL), background(NULL), master(NULL), donkey(NULL), world(NULL), m_debugDraw(NULL), contactListener(NULL), mouseJoint(NULL), masterNormalAndRightAction(NULL), donkeyNormalAction(NULL), waveAction(NULL), wave(NULL), playAgainLayer(NULL), playAgainMenu(NULL), playAgainWord(NULL), playAgainLayerMoveOut(NULL), playAgainLayerMoveIn(NULL), answerLayer(NULL), answerLayerMoveOut(NULL), answerLayerMoveIn(NULL), questionFadeIn(NULL), questionFadeOut(NULL), questionWordFadeIn(NULL), questionWordFadeOut(NULL), questionBackground(NULL), questionWord(NULL), waveRightAction(NULL), waveWrongAction(NULL), donkeyWrongAction(NULL), masterRightAction(NULL), masterWrongAction(NULL), masterQuestionNormalAction(NULL), questionWaveSprite(NULL), finishAction(NULL), finishBackground(NULL), finishWord(NULL), answerCover(NULL)
{
    for (int i = 0; i < vagetableCount; i++)
    {
        vagetablesSprite[i] = NULL;
        vagetableFallInWater[i] = NULL;
        vagetableNormalAction[i] = NULL;
        vagetableBodiesInNormal[i] = NULL;
        vagetableBodies[i] = NULL;
    }
    
    for (int i = 0; i < answerCount; i++)
    {
        stage[i] = NULL;
        answerSprite[i] = NULL;
        answerWord[i] = NULL;
    }
}
Scene5::~Scene5()
{
    CCLog("~Scene5()");
}

CCScene* Scene5::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Scene5* layer = Scene5::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Scene5::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    isGameOpenning = false;
    selectIndexId = -1;
    questionIndex = 0;
    questionOSId = 0;
    
    //因應壓縮
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    
    imageCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    imageCache->removeUnusedSpriteFrames();
    
    imageCache->addSpriteFramesWithFile("S5Story.plist");
    imageCache->addSpriteFramesWithFile("S5Story1.plist");
    imageCache->addSpriteFramesWithFile("S5Story2.plist");
    imageCache->addSpriteFramesWithFile("S5Story3.plist");
    imageCache->addSpriteFramesWithFile("S5Story4.plist");
    imageCache->addSpriteFramesWithFile("S5Game.plist");
    imageCache->addSpriteFramesWithFile("S5Game1.plist");
    imageCache->addSpriteFramesWithFile("S5Game2.plist");
    imageCache->addSpriteFramesWithFile("GameShareObject.plist");
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_BACKGROUND_MUSIC);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_VAGETABLE_SELECTED_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_VAGETABLE_FELL_INTO_WATER_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_ANSWER_SELECTED_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_ANSWER_DROPPED_SOUND);
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_QUESTION_WAVE_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_CORRECT_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_DONKEY_LAUGH_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_DONKEY_DISAPPOINT_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_FINISH_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S5_PLAY_AGAIN_SOUND);
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCString::createWithFormat(S5_QUESTION_OS, "T")->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCString::createWithFormat(S5_QUESTION_OS, "E")->getCString());
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    // Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	
	// Do we want to let bodies sleep?
	bool doSleep = true;
    
	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);
    world->SetAllowSleeping(doSleep);
	world->SetContinuousPhysics(true);
    
    
    m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    world->SetDebugDraw(m_debugDraw);
    
    //多邊形物理物件的繪製設定
    uint32 flags = 0;
//    flags += b2Draw::e_shapeBit;
//    flags += b2Draw::e_jointBit;
//    flags += b2Draw::e_aabbBit;
//    flags += b2Draw::e_pairBit;
//    flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(screenSize.width/2/PTM_RATIO,
                               screenSize.height/2/PTM_RATIO); // bottom-left corner
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	groundBody = world->CreateBody(&groundBodyDef);
	
	// Define the ground box shape.
	b2PolygonShape groundBox;
	
    // bottom
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, -screenSize.height/2/PTM_RATIO), 0);
 	groundBody->CreateFixture(&groundBox, 0);
	
    // top
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, screenSize.height/2/PTM_RATIO), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // left
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(-screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // right
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    
    // load physics shapes
    GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("S5Vagetable.plist");
    
    vagetablePosition[0] = ccp(65, 165);
    vagetablePosition[1] = ccp(180, 165);
    vagetablePosition[2] = ccp(310, 165);
    vagetablePosition[3] = ccp(460, 165);
    vagetablePosition[4] = ccp(600, 165);
    vagetablePosition[5] = ccp(750, 165);
    vagetablePosition[6] = ccp(852, 165);
    vagetablePosition[7] = ccp(969, 165);
    
    answerPosition[0] = ccp(919, 661);
    answerPosition[1] = ccp(919, 498);
    answerPosition[2] = ccp(919, 335);
    answerPosition[3] = ccp(919, 170);
    
    donkeyPosition = ccp(499, 402);
    
    setAllAction();
    
    createStoryObjects();
    createGameObjects();
    
    donkey->runAction(donkeyNormalAction);
    master->runAction(masterNormalAndRightAction);
    wave->runAction(waveAction);
    
    //進遊戲按鈕
    CCSprite* goToGameButton = CCSprite::create();
    CCRect goToGameZoneSize = CCRectMake(0, 0, 1024, 508);
    goToGameButton->setTextureRect(goToGameZoneSize);
    goToGameButton->setOpacity(0);
    
    CCMenuItemSprite* goToGameSpriteItem = CCMenuItemSprite::create(goToGameButton, goToGameButton, this, menu_selector(Scene5::gotoGame));
    goToGameSpriteItem->setPosition(ccp(0, 260));
    goToGameSpriteItem->setAnchorPoint(ccp(0.0f, 0));
    
    goToGameMenu = CCMenu::create(goToGameSpriteItem, NULL);
    goToGameMenu->setPosition(ccp(0, 0));
    goToGameMenu->setZOrder(40);
    goToGameMenu->setTouchPriority(-109);
    this->addChild(goToGameMenu);
    
    functionMenu = FunctionMenu::create();
    functionMenu->setAnchorPoint(ccp(0, 0));
    functionMenu->setPosition(ccp(0, 0));
    
    //通用元件
    functionMenu->setInitialObject(5, this, callfunc_selector(Scene5::preSceneCallBackFunc), callfunc_selector(Scene5::nextSceneCallBackFunc), callfuncND_selector(Scene5::choseSceneClickedCallFunc), callfuncND_selector(Scene5::functionStatusCallBackFunc), callfunc_selector(Scene5::onLanguageChanged));
    
    this->addChild(functionMenu, 51);
    
    actorsLines = ActorsLines::create();
    
    actorsLines->setInitialItmes(5, this, callfunc_selector(Scene5::closeWordsWithClickedBackgroundCallBack), callfunc_selector(Scene5::openWordsCallBack));
    
    this->addChild(actorsLines, 50);
    
    changeLanguage();
    
    actorsLines->playWordsMoveDown();
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(S5_BACKGROUND_MUSIC, true);
    
    schedule(schedule_selector(Scene5::tick));
    
    actorsLines->setVisible(false);
    
    gotoGame();
    
    return true;
}

void Scene5::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
    CCNode::onEnter();
}

void Scene5::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
    CCNode::onExit();
}

void Scene5::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    world->DrawDebugData();
    
    kmGLPopMatrix();
}

void Scene5::tick(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	int velocityIterations = 8;
	int positionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
    
    if (!world)
    {
        CCLog("world is null");
    }
    
	world->Step(dt, velocityIterations, positionIterations);
    
//    std::vector<b2Body *>toDestroy;
    
	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
		}
	}
    
    for (int i = 0; i < vagetableCount; i++)
    {
        if (vagetableBodies[i])
        {
//            CCLog("vagetableBodies[%d] != NULL", i);
            
            if (!vagetableLock[i])
            {
                CCSprite* templateObject = (CCSprite*)vagetableBodies[i]->GetUserData();
                
//                CCLOG("Scene5 templateObject y = %f, vagetablePosition[i].y = %f", templateObject->getPositionY(), vagetablePosition[i].y - 20);
                
                if (templateObject->getPositionY() <= vagetablePosition[i].y - 20)
                {
                    CCLOG("Scene5 deleted vagetable i = %d", i);
                    
//                    toDestroy.push_back(vagetableBodies[i]);
                    
                    vagetablePosition[i] = ccp(templateObject->getPositionX(), vagetablePosition[i].y);
                    
                    if (mouseJoint != NULL)
                    {
                        CCLOG("Scene5 delete mouseJoint");
                        world->DestroyJoint(mouseJoint);
                        mouseJoint = NULL;
                        selectIndexId = -1;
                    }
                    
                    createVagetableNormalB2Body(i);
                    
                    world->DestroyBody(vagetableBodies[i]);
                    storyLayer->removeChild(templateObject);
                    
                    vagetableLock[i] = true;
                    
                    vagetableBodies[i] = NULL;
                    
                    vagetablesSprite[i]->setVisible(true);
                    
                    vagetablesSprite[i]->setPosition(vagetablePosition[i]);
                    
                    vagetablesSprite[i]->stopAction(vagetableNormalAction[i]);
                    vagetablesSprite[i]->runAction(vagetableFallInWater[i]);
                    
                    if (!isGameOpenning)
                        SimpleAudioEngine::sharedEngine()->playEffect(S5_VAGETABLE_FELL_INTO_WATER_SOUND, false);
                }
            }
        }
    }
    
//    std::vector<b2Body *>::iterator pos2;
//    for(pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2) {
//        b2Body *body = *pos2;
//        if (body->GetUserData() != NULL) {
//            CCSprite *sprite = (CCSprite *) body->GetUserData();
//            storyLayer->removeChild(sprite, true);
//        }
//        world->DestroyBody(body);
//    }
    
}

#pragma mark -
#pragma mark 初始函數

void Scene5::createStoryObjects()
{
    storyLayer = CCLayer::create();
    
    this->addChild(storyLayer, 5);
    
    for (int i = 0; i < vagetableCount; i++)
    {
        const char* vagetableName = CCString::createWithFormat(vagetables[i][1], vagetableStartActionNumber[i])->getCString();
        
        CCLog("vagetableName = %s", vagetableName);
        
        vagetablesSprite[i] = TouchSpriteWithPassEvent::createBySpriteFrameName(vagetableName);
        
        vagetablesSprite[i]->setAnchorPoint(ccp(0.5f, 0.3f));
        
        vagetablesSprite[i]->setPosition(vagetablePosition[i]);
        
        int level = 5;
        
        i < (vagetableCount / 2) ? level = 5 : level = 15;
        
        vagetablesSprite[i]->setAllEventCallBack(this, menu_selector(Scene5::vagetableTouchBegan), menu_selector(Scene5::vagetableTouchEnded), menu_selector(Scene5::vagetableTouchMoved));
        
        vagetablesSprite[i]->setTag(i);
        
        storyLayer->addChild(vagetablesSprite[i], level);
        
        createVagetableNormalB2Body(i);
        
        vagetablesSprite[i]->runAction(vagetableNormalAction[i]);
        
    }
    
    setSpriteObjectAtContent(background, ccp(0, 0), 0, S5_BACKGROUND_PICTURE, this);
    background->setAnchorPoint(ccp(0, 0));
    
    const char* masterName = CCString::createWithFormat(S5_MASTER_PICTURE, 1)->getCString();
    setSpriteObjectAtContent(master, ccp(130, 586), 0, masterName, this);
    
    const char* donkeyName = CCString::createWithFormat(S5_DONKEY_PICTURE, 1)->getCString();
    setSpriteObjectAtContent(donkey, donkeyPosition, 3, donkeyName, this);
    
    const char* waveName = CCString::createWithFormat(S5_WAVE_PICTURE, 1)->getCString();
    setSpriteObjectAtContent(wave, ccp(512, 252), 0, waveName, this);
}

void Scene5::setAllAction()
{
    for (int i = 0; i < vagetableCount; i++)
    {
        actionSettingWithBeginNumber(vagetableNormalAction[i], vagetables[i][1], 12, -1, 0.083f, vagetableStartActionNumber[i]);
        vagetableNormalAction[i]->retain();
        
        CCActionInterval* fallIntoWater;
        
        actionSettingWithBeginNumber(fallIntoWater, vagetables[i][2], 12, 1, 0.083f, vagetableStartActionNumber[i]);
        
        vagetableFallInWater[i] = CCSequence::create(fallIntoWater, CCCallFuncN::create(this, callfuncN_selector(Scene5::vagetableFellDown)), NULL);
        
        vagetableFallInWater[i]->retain();
    }
    
    actionSettingWithBeginNumber(masterNormalAndRightAction, S5_MASTER_PICTURE, 12, -1, 0.083f, 1);
    masterNormalAndRightAction->retain();
    actionSettingWithBeginNumber(donkeyNormalAction, S5_DONKEY_PICTURE, 12, -1, 0.083f, 1);
    donkeyNormalAction->retain();
    actionSettingWithBeginNumber(waveAction, S5_WAVE_PICTURE, 26, -1, 0.083f, 1);
    waveAction->retain();
    
    
    //遊戲
    CCMoveTo* answerMoveIn = CCMoveTo::create(1.0f, ccp(0, 0));
    
    answerLayerMoveIn = CCSequence::create(answerMoveIn, CCCallFunc::create(this, callfunc_selector(Scene5::onAnswerLayerMovedIn)), NULL);
    
    answerLayerMoveIn->retain();
    
    CCMoveTo* answerMoveOut = CCMoveTo::create(1.0f, ccp(300, 0));
    
    answerLayerMoveOut = CCSequence::create(answerMoveOut, CCCallFunc::create(this, callfunc_selector(Scene5::onAnswerLayerMovedOut)), NULL);
    
    answerLayerMoveOut->retain();
    
    questionFadeIn = CCFadeIn::create(1.0f);
    questionFadeOut = CCFadeOut::create(1.0f);
    
    questionFadeIn->retain();
    questionFadeOut->retain();
    
    questionWordFadeIn = CCFadeIn::create(1.0f);
    questionWordFadeOut = CCFadeOut::create(1.0f);
    
    questionWordFadeIn->retain();
    questionWordFadeOut->retain();
    
    actionSettingWithBeginNumber(masterQuestionNormalAction, S5_MASTER_QUESTION_PICTURE, 12, -1, 0.083f, 25);
    masterQuestionNormalAction->retain();
    
    actionSettingWithBeginNumber(masterRightAction, S5_MASTER_PICTURE, 12, 1, 0.083f, 1);
    masterRightAction->retain();

    actionSettingWithBeginNumber(masterWrongAction, S5_MASTER_WRONG_PICTURE, 12, 1, 0.083f, 13);
    masterWrongAction->retain();
    
    CCActionInterval* donkeyWrong;
    actionSettingWithBeginNumber(donkeyWrong, S5_DONKEY_WRONG_ANSWER, 12, 1, 0.083f, 25);
    
    CCActionInterval* donkeyRight;
    actionSettingWithBeginNumber(donkeyRight, S5_DONKEY_RIGHT_ANSWER, 12, 1, 0.083f, 13);
    
    CCActionInterval* questionWave;
    actionSettingWithBeginNumber(questionWave, S5_QUESTION_WAVE_PICTURE, 8, 1, 0.083f, 1);
    
    waveWrongAction = CCSequence::create(questionWave, CCCallFunc::create(this, callfunc_selector(Scene5::onWrongActionDone)), NULL);
    
    waveRightAction = CCSequence::create(questionWave, CCCallFunc::create(this, callfunc_selector(Scene5::onRightActionDone)), NULL);
    
    donkeyWrongAction = CCSequence::create(donkeyWrong, CCCallFunc::create(this, callfunc_selector(Scene5::onDonkeyWrongActionDone)), NULL);
    
    donkeyRightAction = CCSequence::create(donkeyRight, CCCallFunc::create(this, callfunc_selector(Scene5::onDonkeyRightActionDone)), NULL);
    
    waveWrongAction->retain();
    waveRightAction->retain();
    
    donkeyRightAction->retain();
    donkeyWrongAction->retain();
    
    actionSettingWithBeginNumber(finishAction, S5_FINISH_BACKGROUND_PICTURE, 2, -1, 0.083f, 1);
    finishAction->retain();
    
    //重打
    CCMoveTo* playAgainMoveOut = CCMoveTo::create(0.5f, ccp(300, 0));
    playAgainLayerMoveOut = CCSequence::create(playAgainMoveOut, CCCallFunc::create(this, callfunc_selector(Scene5::onPlayAgainLayerMovedOut)),NULL);
    playAgainLayerMoveOut->retain();
    
    CCMoveTo* playAgainMoveIn = CCMoveTo::create(0.5f, ccp(0, 0));
    playAgainLayerMoveIn = CCSequence::create(playAgainMoveIn, CCCallFunc::create(this, callfunc_selector(Scene5::onPlayAgainLayerMovedIn)),NULL);
    playAgainLayerMoveIn->retain();
   
}

void Scene5::createGameObjects()
{
    gameLayer = CCLayer::create();
    this->addChild(gameLayer, 5);
    
    answerLayer = CCLayer::create();
    answerLayer->setPosition(ccp(300, 0));
    
    gameLayer->addChild(answerLayer, 5);
    
    for (int i = 0; i < answerCount; i ++)
    {
        setSpriteObjectAtContent(stage[i], answerPosition[i], 5, S5_STAGE_PICTURE, answerLayer);
    }
    
    CCPoint questionPoint = ccp(577, 658);
    
    setSpriteObjectAtContent(questionBackground, questionPoint, 0, S5_QUESTION_BACKGROUND, gameLayer);
    
    const char* languageName = "T";
    
    const char* questionWordName = CCString::createWithFormat(S5_QUESTION_WORD, languageName)->getCString();
    
    setSpriteObjectAtContent(questionWord, questionPoint, 0, questionWordName, gameLayer);
    
    questionBackground->setOpacity(0);
    questionWord->setOpacity(0);
    
    gameLayer->setVisible(false);
    
    createAnswerObject();
    
    //再玩一次
    playAgainLayer = CCLayer::create();
    
    playAgainLayer->setPosition(ccp(300, 0));
    
    gameLayer->addChild(playAgainLayer, 20);
    
    CCSprite* replayBackground;
    
    CCPoint replayBackgroundPosition = ccp(878, 136);
    
    setSpriteObjectAtContent(replayBackground, replayBackgroundPosition, 5, S5_REPLAY_GAME_BACKGROUND, playAgainLayer);
    
    const char* replayName = CCString::createWithFormat(S5_REPLAY_GAME_WORDS, languageName)->getCString();
    
    setSpriteObjectAtContent(playAgainWord, ccp(0, 0), 5, replayName, replayBackground);
    
    playAgainWord->setAnchorPoint(ccp(0, 0));
    
    playAgainLayer->setVisible(false);
    
    const char* questionWaveName = CCString::createWithFormat(S5_QUESTION_WAVE_PICTURE, 8)->getCString();
    setSpriteObjectAtContent(questionWaveSprite, donkeyPosition, 10, questionWaveName, gameLayer);
    
    //重打按鈕
    CCSprite* replayButton = CCSprite::create();
    CCRect replayZoneSize = CCRectMake(0, 0, 256, 154);
    replayButton->setTextureRect(replayZoneSize);
    replayButton->setOpacity(0);
    
    CCMenuItemSprite* replaySpriteItem = CCMenuItemSprite::create(replayButton, replayButton, this, menu_selector(Scene5::onReplayClicked));
    replaySpriteItem->setPosition(replayBackgroundPosition);
    
    playAgainMenu = CCMenu::create(replaySpriteItem, NULL);
    playAgainMenu->setPosition(ccp(0, 0));
    playAgainLayer->addChild(playAgainMenu);
    
    CCPoint centerPoint = ccp(512, 384);
    
    //結束圖示
    const char* finishBackgroundName = CCString::createWithFormat(S5_FINISH_BACKGROUND_PICTURE, 1)->getCString();
    
    setSpriteObjectAtContent(finishBackground, centerPoint, 40, finishBackgroundName, gameLayer);
    
    const char* finishWordName = CCString::createWithFormat(S5_FINISH_WORD_PICTURE, languageName)->getCString();
    
    setSpriteObjectAtContent(finishWord, ccp(0, 0), 5, finishWordName, finishBackground);
    finishWord->setAnchorPoint(ccp(0, 0));
    
    finishBackground->setVisible(false);
    
    //重打按鈕
    CCSprite* answerCoverButton = CCSprite::create();
    CCRect answerCoverZoneSize = CCRectMake(0, 0, 160, 630);
    answerCoverButton->setTextureRect(answerCoverZoneSize);
    answerCoverButton->setOpacity(0);
    
    CCMenuItemSprite* answerCoverSpriteItem = CCMenuItemSprite::create(answerCoverButton, answerCoverButton, this, NULL);
    answerCoverSpriteItem->setPosition(ccp(839, 93));
    answerCoverSpriteItem->setAnchorPoint(ccp(0, 0));
    
    answerCover = CCMenu::create(answerCoverSpriteItem, NULL);
    answerCover->setPosition(ccp(0, 0));
    answerCover->setAnchorPoint(ccp(0, 0));
    
    gameLayer->addChild(answerCover, 90);
    
    answerCover->setTouchPriority(-130);
    
    answerCover->setVisible(false);
    
}

void Scene5::createAnswerObject()
{
    getRandomAndNoRepeat(rightAnswerArray, answerCount, 1);
    
    int wrongAnswerArray[wrongAnswerCount];
    
    getRandomAndNoRepeat(wrongAnswerArray, wrongAnswerCount, 1);
    
    int position[answerCount];
    
    getRandomAndNoRepeat(position, answerCount, 1);
    
    answerSprite[0] = MoveableSpr::createBySpriteFrameName(rightAnswer[rightAnswerArray[questionIndex]][0]);
    
    answerSprite[0]->setTarget(this, menu_selector(Scene5::onAnswerDrag), menu_selector(Scene5::onAnswerDrop));
    
    answerSprite[0]->setPosition(answerPosition[position[0]]);
    
    answerSprite[0]->setTag(1);
    
    answerLayer->addChild(answerSprite[0], 10);
    
//    CCLog("questionIndex = %d", questionIndex);
//    
//    for (int i = 0; i < answerCount; i++)
//    {
//        CCLog("rightAnswerArray[%d] = %d", i, rightAnswerArray[i]);
//    }
//
//    CCLog(CCString::createWithFormat(rightAnswer[rightAnswerArray[questionIndex]][1], "T")->getCString());
    
    const char* languageName = getLanguageName(false);
    
    const char* rightWordName = CCString::createWithFormat(rightAnswer[rightAnswerArray[questionIndex]][1], languageName)->getCString();
    
    CCLog("rightWordName = %s", rightWordName);
    
    answerWord[0] = CCSprite::createWithSpriteFrameName(rightWordName);
    answerWord[0]->setPosition(answerPosition[position[0]]);
    
    answerLayer->addChild(answerWord[0], 10);
    
    for (int i = 0; i < 3; i++)
    {
        answerSprite[i + 1] = MoveableSpr::createBySpriteFrameName(wrongAnswer[wrongAnswerArray[i]][0]);
        
        answerSprite[i + 1]->setTarget(this, menu_selector(Scene5::onAnswerDrag), menu_selector(Scene5::onAnswerDrop));
        
        answerSprite[i + 1]->setPosition(answerPosition[position[i + 1]]);
        
        answerSprite[i + 1]->setTag(i + 2);
        
        answerLayer->addChild(answerSprite[i + 1], 10);
        
        const char* wrongWordName = CCString::createWithFormat(wrongAnswer[wrongAnswerArray[i]][1], languageName)->getCString();
        
        CCLog("wrongWordName = %s", wrongWordName);
        
        answerWord[i + 1] = CCSprite::createWithSpriteFrameName(wrongWordName);
        answerWord[i + 1]->setPosition(answerPosition[position[i + 1]]);
        
        answerLayer->addChild(answerWord[i + 1], 10);
    }
}

#pragma mark -
#pragma mark 內部函數

const char* Scene5::getLanguageName(bool isTwoType)
{
    int languageType = (ShareData::getInstance())->getLanguageOfScene();
    const char* languageName = "";
    
    if (isTwoType)
    {
        if (languageType == TRADITIONAL_CHINESE)
        {
            languageName = "T";
        }
        else if (languageType == SIMPLE_CHINESE)
        {
            languageName = "T";
        }
        else
        {
            languageName = "E";
        }
    }
    else
    {
        if (languageType == TRADITIONAL_CHINESE)
        {
            languageName = "T";
        }
        else if (languageType == SIMPLE_CHINESE)
        {
            languageName = "C";
        }
        else
        {
            languageName = "E";
        }
    }
    
    return languageName;
}

void Scene5::changeLanguage()
{
    actorsLines->changeLanguage();
    
    const char* languageName = getLanguageName(false);
    
    const char* finishWordName = CCString::createWithFormat(S5_FINISH_WORD_PICTURE, languageName)->getCString();
    
    finishWord->setDisplayFrame(imageCache->spriteFrameByName(finishWordName));
    
    const char* replayName = CCString::createWithFormat(S5_REPLAY_GAME_WORDS, languageName)->getCString();
    
    playAgainWord->setDisplayFrame(imageCache->spriteFrameByName(replayName));
    
    const char* questionWordName = CCString::createWithFormat(S5_QUESTION_WORD, languageName)->getCString();
    
    questionWord->setDisplayFrame(imageCache->spriteFrameByName(questionWordName));
    
//    const char* rightWordName = CCString::createWithFormat(rightAnswer[rightAnswerArray[questionIndex]][1], languageName)->getCString();
//    
//    answerWord[0]->setDisplayFrame(imageCache->spriteFrameByName(rightWordName));
//    
//    const char* wrongWordName = CCString::createWithFormat(wrongAnswer[wrongAnswerArray[i]][1], languageName)->getCString();
//    
//    CCLog("wrongWordName = %s", wrongWordName);
//    
//    answerWord[i + 1] = CCSprite::createWithSpriteFrameName(wrongWordName);
    
    
}

void Scene5::setSpriteObjectAtContent(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* content)
{
    target = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(name));
    
    target->setPosition(position);
    
    content->addChild(target, layerLevel);
}

void Scene5::actionSettingWithBeginNumber(CCActionInterval*& targetAction, const char* name, int count, int repeatCount, float delayPerUnit, int beginNumber)
{
    CCAnimation* targetAnimation = CCAnimation::create();
    for (int i = beginNumber ; i < count + beginNumber; i++) {
        const char* tempString = CCString::createWithFormat(name, i)->getCString();
        //CCLog("tempString = %s", tempString);
        
        if (!imageCache->spriteFrameByName(tempString))
        {
            CCLog("missing tempString = %s", tempString);
        }
        
        targetAnimation->addSpriteFrame(imageCache->spriteFrameByName(tempString));
    }
    
    targetAnimation->setDelayPerUnit(delayPerUnit);
    
    targetAction = (CCRepeat::create(CCAnimate::create(targetAnimation), repeatCount));
    
//    targetAction->retain();
    
}

void Scene5::createStoryInterActiveObject(int index)
{
    CCLog("Scene5 createStoryInterActiveObject index = %d", index);
    
    //蔬菜
    CCPoint position = vagetablePosition[index];
    
    position = ccpAdd(position, ccp(0, 0));
    
    string vagetableName = vagetables[index][0];
    
    CCSprite* vagetableSprite = CCSprite::create((vagetableName + ".png").c_str());
    
    vagetableSprite->setPosition(position);
    
    vagetableSprite->setOpacity(255);
    
    vagetableSprite->setTag(index);
    
    storyLayer->addChild(vagetableSprite, 10);
    
    b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
    
	bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	bodyDef.userData = vagetableSprite;
	vagetableBodies[index] = world->CreateBody(&bodyDef);
    
    // add the fixture definitions to the body
    
    GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
    sc->addFixturesToBody(vagetableBodies[index], vagetableName.c_str());
    vagetableSprite->setAnchorPoint(sc->anchorPointForShape(vagetableName.c_str()));
}

void Scene5::createVagetableNormalB2Body(int index)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    
    bodyDef.position.Set(vagetablePosition[index].x/PTM_RATIO, vagetablePosition[index].y/PTM_RATIO);
    bodyDef.userData = vagetablesSprite[index];
    vagetableBodiesInNormal[index] = world->CreateBody(&bodyDef);
    
    // add the fixture definitions to the body
    
    GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
    sc->addFixturesToBody(vagetableBodiesInNormal[index], vagetableInNormalName[index]);
    vagetablesSprite[index]->setAnchorPoint(sc->anchorPointForShape(vagetableInNormalName[index]));
}

void Scene5::getRandomAndNoRepeat(int *randomArray, int xCount, int yCount)
{
    int quantity = xCount * yCount;
    int start = 0;
    int total = xCount * yCount;
    if (quantity > total) {
        CCLog("随机数错误");
    }
    int sequence[total];  //存放随机数的数组
    //int output[quantity]; //最终生成的不重复一系列随机数
    
    //将sequence 初始化
    for (int i = 0; i < total; i++) {
        sequence[i] = start + i;
    }
    
    //随机数种子
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    unsigned long int seed = psv.tv_sec*1000 + psv.tv_usec/1000;
    srand(seed);
    
    for (int i = 0; i < quantity; i++) {
        int num = arc4random() % total;//在指定范围下产生随机数
        randomArray[i] = sequence[num];//将产生的随机数存储
        sequence[num] = sequence[total-1];//将最后个下标的值填充到随机产生的下标中
        total--;//在指定范围 向前移
    }
}

void Scene5::showTheQuestion()
{
    const char* languageName = getLanguageName(false);
    
    int wrongAnswerArray[wrongAnswerCount];
    
    getRandomAndNoRepeat(wrongAnswerArray, wrongAnswerCount, 1);
    
    int position[answerCount];
    
    getRandomAndNoRepeat(position, answerCount, 1);
    
    answerSprite[0]->setDisplayFrame(imageCache->spriteFrameByName(rightAnswer[rightAnswerArray[questionIndex]][0]));
    
    answerSprite[0]->setPosition(answerPosition[position[0]]);
    
    const char* rightWordName = CCString::createWithFormat(rightAnswer[rightAnswerArray[questionIndex]][1], languageName)->getCString();
    
    CCLog("rightWordName = %s", rightWordName);
    
    answerWord[0]->setDisplayFrame(imageCache->spriteFrameByName(rightWordName));
    answerWord[0]->setPosition(answerPosition[position[0]]);
    
    for (int i = 0; i < 3; i++)
    {
        answerSprite[i + 1]->setDisplayFrame(imageCache->spriteFrameByName(wrongAnswer[wrongAnswerArray[i]][0]));
        
        answerSprite[i + 1]->setPosition(answerPosition[position[i + 1]]);
        
        answerSprite[i + 1]->setTag(i + 2);
        
        const char* wrongWordName = CCString::createWithFormat(wrongAnswer[wrongAnswerArray[i]][1], languageName)->getCString();
        
        CCLog("wrongWordName = %s", wrongWordName);

        answerWord[i + 1]->setDisplayFrame(imageCache->spriteFrameByName(wrongWordName));
        answerWord[i + 1]->setPosition(answerPosition[position[i + 1]]);

    }
    
    setAllAnswerMoveableAndShow(true);
    
    SimpleAudioEngine::sharedEngine()->stopEffect(questionOSId);
    
    languageName = getLanguageName(true);
    
    if (questionIndex == 0)
    {
        questionOSId = SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat(S5_QUESTION_OS, languageName)->getCString(), false);
    }
}

bool Scene5::checkPointIsInAnswerLayer(CCPoint point)
{
    CCRect checkAreaBounds = CCRectMake(297, 150, 400, 400);
    
    if (checkAreaBounds.containsPoint(point))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Scene5::setAllAnswerMoveableAndShow(bool moveable)
{
    for (int i = 0; i < answerCount; i++)
    {
        answerSprite[i]->setMoveable(moveable);
        answerSprite[i]->setVisible(moveable);
    }
}

void Scene5::playGameNormalAction()
{
    master->stopAllActions();
    master->runAction(masterQuestionNormalAction);
    
    donkey->stopAllActions();
    donkey->runAction(donkeyNormalAction);
}

#pragma mark -
#pragma mark 事件函數

void Scene5::onLanguageChanged()
{
    //    changeLanguage();
    
    CCLog("onStartStory()");
    
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    CCTextureCache::sharedTextureCache()->removeTextureForKey((ShareData::getInstance())->getPictureName());
    CCTextureCache::sharedTextureCache()->removeTextureForKey((ShareData::getInstance())->getPictureNameTs());
    
//    (ChangeScene::getInstance())->changeToScene(5);
    
    (ChangeScene::getInstance())->changeToTemplateScene(5);
    
}

void Scene5::onDonkeyRightActionDone()
{
    answerCover->setVisible(false);
    questionIndex++;
    answerLayer->runAction(answerLayerMoveOut);
    playGameNormalAction();
}

void Scene5::onDonkeyWrongActionDone()
{
    answerCover->setVisible(false);
    playGameNormalAction();
}

void Scene5::onRightActionDone()
{
    donkey->stopAllActions();
    master->stopAllActions();
    
    donkey->runAction(donkeyRightAction);
    master->runAction(masterRightAction);
    
    SimpleAudioEngine::sharedEngine()->playEffect(S5_DONKEY_LAUGH_SOUND, false);
}

void Scene5::onWrongActionDone()
{
    donkey->stopAllActions();
    master->stopAllActions();
    
    donkey->runAction(donkeyWrongAction);
    master->runAction(masterWrongAction);
    
    SimpleAudioEngine::sharedEngine()->playEffect(S5_DONKEY_DISAPPOINT_SOUND, false);
}

void Scene5::onReplayClicked()
{
    questionIndex = 0;
    playAgainMenu->setVisible(false);
    playAgainLayer->runAction(playAgainLayerMoveOut);
    
    finishBackground->setVisible(false);
    
    SimpleAudioEngine::sharedEngine()->playEffect(S5_PLAY_AGAIN_SOUND, false);
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(S5_BACKGROUND_MUSIC, true);
    
}

void Scene5::onPlayAgainLayerMovedIn()
{
    playAgainMenu->setVisible(true);
}

void Scene5::onPlayAgainLayerMovedOut()
{
    CCLog("onPlayAgainLayerMovedOut()");
    if (isGameOpenning)
    {
        CCLog("onPlayAgainLayerMovedOut() showTheQuestion");
        showTheQuestion();
        answerLayer->runAction(answerLayerMoveIn);
    }
    else
    {
        playAgainLayer->setVisible(false);

    }
    
}

void Scene5::onAnswerDrag(cocos2d::CCObject *pSender)
{
    MoveableSpr* target = (MoveableSpr*)pSender;
    answerOriginalPosition = target->getPosition();
    
    SimpleAudioEngine::sharedEngine()->playEffect(S5_ANSWER_SELECTED_SOUND, false);
}

void Scene5::onAnswerDrop(cocos2d::CCObject *pSender)
{
    MoveableSpr* target = (MoveableSpr*)pSender;
    CCPoint anchorPoint = target->convertToWorldSpace(target->getAnchorPointInPoints());
    
    bool inArea = checkPointIsInAnswerLayer(anchorPoint);
    
    if (inArea)
    {
        answerCover->setVisible(true);
        
        target->setMoveable(false);
        target->setVisible(false);
        
        master->stopAllActions();
        donkey->stopAllActions();
        
        SimpleAudioEngine::sharedEngine()->playEffect(S5_ANSWER_DROPPED_SOUND, false);
        
        if (target->getTag() - 1 == 0)
        {
            CCLog("correct");
            
            questionWaveSprite->stopAllActions();
            questionWaveSprite->runAction(waveRightAction);
            
            SimpleAudioEngine::sharedEngine()->playEffect(S5_QUESTION_WAVE_SOUND, false);
            
        }
        else
        {
            CCLog("wrong");
            questionWaveSprite->stopAllActions();
            questionWaveSprite->runAction(waveWrongAction);
            
            SimpleAudioEngine::sharedEngine()->playEffect(S5_QUESTION_WAVE_SOUND, false);
        }
        
    }
    else
    {
        target->setPosition(answerOriginalPosition);
    }

}

void Scene5::onAnswerLayerMovedIn()
{
    
}

void Scene5::onAnswerLayerMovedOut()
{
    if (!isGameOpenning)
    {
        gameLayer->setVisible(false);
        storyLayer->setVisible(true);
        goToGameMenu->setVisible(true);
    }
    else
    {
        if (questionIndex >= 4)
        {
            playAgainLayer->setVisible(true);
            playAgainLayer->runAction(playAgainLayerMoveIn);
            
            finishBackground->setVisible(true);
            finishBackground->stopAllActions();
            finishBackground->runAction(finishAction);
            
            SimpleAudioEngine::sharedEngine()->playEffect(S5_FINISH_SOUND, false);
        }
        else
        {
            showTheQuestion();
            answerLayer->runAction(answerLayerMoveIn);
        }
    }
}

void Scene5::vagetableFellDown(cocos2d::CCObject *pSender)
{
    TouchSpriteWithPassEvent* target = (TouchSpriteWithPassEvent*)pSender;
    
    int i = target->getTag();
    
    vagetablesSprite[i]->setVisible(true);
    vagetablesSprite[i]->setTouchable(true);
    
    vagetableLock[i] = false;
    
    vagetablesSprite[i]->runAction(vagetableNormalAction[i]);
}

void Scene5::vagetableTouchBegan(cocos2d::CCObject *pSender)
{
    TouchSpriteWithPassEvent* target = (TouchSpriteWithPassEvent*)pSender;
    
    CCLog("Scene5 vagetableTouchBegan target tag = %d", target->getTag());
    
    target->setTouchable(false);
    target->setVisible(false);
    
    world->DestroyBody(vagetableBodiesInNormal[target->getTag()]);
    vagetableBodiesInNormal[target->getTag()] = NULL;
    
    createStoryInterActiveObject(target->getTag());
    
    SimpleAudioEngine::sharedEngine()->playEffect(S5_VAGETABLE_SELECTED_SOUND, false);
    
}

void Scene5::vagetableTouchEnded(cocos2d::CCObject *pSender)
{
    
}

void Scene5::vagetableTouchMoved(cocos2d::CCObject *pSender)
{
    
}

bool Scene5::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    CCLOG("Scene5 ccTouchBegan");
    
    CCPoint tpos=touch->getLocation();
    CCLOG("Scene5 tpos = %f, %f", tpos.x, tpos.y);
    b2Vec2 posInWorld(tpos.x/PTM_RATIO, tpos.y/PTM_RATIO);
    
//    b2Fixture *temp = vageTablesBody[0]->GetFixtureList();
    
    bool isMouseOnTheHand = false;
    
    b2Body *pBody =world->GetBodyList();
    for(; pBody != NULL; pBody = pBody->GetNext())
    {
        b2Fixture *temp = pBody->GetFixtureList();

        for(;temp != NULL;temp = temp->GetNext())
        {
            bool isIn = temp->TestPoint(posInWorld);
            
            if (isIn)
            {
                CCLOG("Scene5 in bodyTest index = %d", ((CCSprite*)pBody->GetUserData())->getTag());
                b2MouseJointDef mjDef;
//                mjDef.bodyA=world->CreateBody(new b2BodyDef);
                mjDef.bodyA=groundBody;
                mjDef.bodyB=pBody;
                mjDef.target=posInWorld;
                mjDef.collideConnected=true;
                mjDef.maxForce=1000.0f*pBody->GetMass();
                mouseJoint=(b2MouseJoint*)world->CreateJoint(&mjDef);
                
                isMouseOnTheHand = isIn;
                
                selectIndexId = ((CCSprite*)pBody->GetUserData())->getTag();
                
                break;
                
            }
            else
            {
                CCLOG("Scene5 not in bodyTest");
            }
        }
        
    }
    
    if (isMouseOnTheHand)
        CCLOG("Scene5 finaly in");
    else
        CCLOG("Scene5 still not in");
    
    return isMouseOnTheHand;
    
}

void Scene5::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    //    struct cc_timeval now;
    //    CCTime::gettimeofdayCocos2d(&now, NULL);
    //
    //    struct tm *tm;
    //    time_t timep = now.tv_sec;
    //    tm = localtime(&timep);
    //    int year = tm->tm_year + 1900;
    //    int month = tm->tm_mon + 1;
    //    int day = tm->tm_mday;
    //    int hour=tm->tm_hour;
    //    int min=tm->tm_min;
    //    int second=tm->tm_sec;
    //
    //    CCLOG("CCTouch Moved : %d/%d/%d %d:%d:%d", year, month, day, hour, min, second);
    
    if(!mouseJoint)
        return;
    
    CCLOG("Scene5 ccTouchMoved mouseJoint");
    
    CCPoint tpos=touch->getLocation();
    
    if (tpos.y <= vagetablePosition[selectIndexId].y + 20)
    {
        tpos.y = vagetablePosition[selectIndexId].y + 20;
    }
    
    b2Vec2 posInWorld(tpos.x/PTM_RATIO, tpos.y/PTM_RATIO);
    mouseJoint->SetTarget(posInWorld);
}

void Scene5::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    CCLOG("Scene5 ccTouchEnded");
    if(mouseJoint != NULL)
    {
        CCPoint tpos=touch->getLocation();
        CCLOG("Scene5 moved tpos = %f, %f", tpos.x, tpos.y);
        
        world->DestroyJoint(mouseJoint);
        mouseJoint = NULL;
        
        selectIndexId = -1;
    }
}

//共用函數
void Scene5::whenLeaving()
{
    for (int i = 0; i < vagetableCount; i++)
    {
        vagetableNormalAction[i]->release();
        vagetableFallInWater[i]->release();
    }
    masterNormalAndRightAction->release();
    donkeyNormalAction->release();
    
    waveAction->release();
    
    playAgainLayerMoveOut->release();
    playAgainLayerMoveIn->release();
    
    answerLayerMoveOut->release();
    answerLayerMoveIn->release();
    
    questionFadeIn->release();
    questionFadeOut->release();
    questionWordFadeIn->release();
    questionWordFadeOut->release();
    
    waveRightAction->release();
    waveWrongAction->release();
    
    donkeyWrongAction->release();
    donkeyRightAction->release();
    
    masterRightAction->release();
    masterWrongAction->release();
    
    masterQuestionNormalAction->release();
    
    finishAction->release();
    
    functionMenu->releaseObject();
    actorsLines->releaseObject();
    
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_BACKGROUND_MUSIC);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_VAGETABLE_SELECTED_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_VAGETABLE_FELL_INTO_WATER_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_ANSWER_SELECTED_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_ANSWER_DROPPED_SOUND);
    
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_QUESTION_WAVE_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_CORRECT_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_DONKEY_LAUGH_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_DONKEY_DISAPPOINT_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_FINISH_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S5_PLAY_AGAIN_SOUND);
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCString::createWithFormat(S5_QUESTION_OS, "T")->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCString::createWithFormat(S5_QUESTION_OS, "E")->getCString());
    
    CCTextureCache::sharedTextureCache()->removeTextureForKey("GameShareObject.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Game.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Game1.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Game2.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Story.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Story1.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Story2.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Story3.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S5Story4.pvr.ccz");
    
    imageCache->removeSpriteFrames();
    imageCache->removeUnusedSpriteFrames();
    
    CCLog("whenLeaving!!!");
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

void Scene5::gotoGame()
{
    CCLog("gotoGame()");
    
    if (!isGameOpenning)
    {
        goToGameMenu->setVisible(false);
        
        gameLayer->setVisible(true);
        storyLayer->setVisible(false);
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        
        getRandomAndNoRepeat(rightAnswerArray, answerCount, 1);
        
        showTheQuestion();
        
        questionBackground->runAction(questionFadeIn);
        questionWord->runAction(questionWordFadeIn);

        answerLayer->runAction(answerLayerMoveIn);
        
        playGameNormalAction();
        
        isGameOpenning = true;
    }
}

void Scene5::preSceneCallBackFunc()
{
    CCLog("onStartStory()");
    
    whenLeaving();
    
//    (ChangeScene::getInstance())->changeToScene(4);
    (ChangeScene::getInstance())->changeToTemplateScene(0);
}

void Scene5::nextSceneCallBackFunc()
{
    CCLog("onStartStory()");
    
    whenLeaving();
    
//    (ChangeScene::getInstance())->changeToScene(6);
    (ChangeScene::getInstance())->changeToTemplateScene(6);
}

void Scene5::choseSceneClickedCallFunc(cocos2d::CCObject *pSender, int chapterID)
{
    CCLog("choseSceneClickedCallFunc() id = %d", chapterID);
    
    whenLeaving();
    
//    (ChangeScene::getInstance())->changeToScene(chapterID);
    (ChangeScene::getInstance())->changeToTemplateScene(chapterID);
}

void Scene5::functionStatusCallBackFunc(cocos2d::CCObject *pSender, int functionMenuStatus)
{
    const char* functionMenuStatusName = functionMenuStatus == 0 ? "false" : "true";
    
    bool menuStatus;
    
    if (functionMenuStatus == FUNCTION_MENU_CLOSING)
    {
        CCLog("menuStatus = false");
        menuStatus = false;
    }
    else
    {
        CCLog("menuStatus = true");
        menuStatus = true;
    }
    
    CCLog("functionMenuStatus = %s", functionMenuStatusName);
    
    //    setAllMenuEnable(!menuStatus);
}

void Scene5::closeWordsWithClickedBackgroundCallBack()
{
    
}

void Scene5::openWordsCallBack()
{
    
    if (isGameOpenning)
    {
        isGameOpenning = false;
        answerCover->setVisible(false);
        
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
        
        questionBackground->runAction(questionFadeOut);
        questionWord->runAction(questionWordFadeOut);
        
        answerLayer->runAction(answerLayerMoveOut);
        
        donkey->stopAllActions();
        donkey->runAction(donkeyNormalAction);
        
        master->stopAllActions();
        master->runAction(masterNormalAndRightAction);
        
        finishBackground->setVisible(false);
        
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(S5_BACKGROUND_MUSIC, true);
        
    }
}

//end

