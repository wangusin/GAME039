#ifndef __HELLOWORLD_Scene5_H__
#define __HELLOWORLD_Scene5_H__

#include "cocos2d.h"
#include "FunctionMenu.h"
#include "ActorsLines.h"
#include "TouchSpriteWithPassEvent.h"

#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#include "MyContactListener.h"
#include "GB2ShapeCache-x.h"
#include "MoveableSpr.h"

#include "ChangeScene.h"
#include "ShareData.h"

#define S5_BACKGROUND_PICTURE "p0501p01.png"
#define S5_MASTER_PICTURE "p0502g0100%02d.png"
#define S5_WAVE_PICTURE "p0501p0300%02d.png"

#define S5_DONKEY_PICTURE "02一般/p0502g0200%02d.png"

#define S5_STAGE_PICTURE "p0502g04.png"

#define S5_QUESTION_BACKGROUND "p0202g03.png"
#define S5_QUESTION_WORD "p0202g03%s.png"

#define S5_REPLAY_GAME_WORDS "p0102g05%s.png"
#define S5_REPLAY_GAME_BACKGROUND "p0102g05.png"

#define S5_DONKEY_WRONG_ANSWER "p0502g0200%02d.png"
#define S5_DONKEY_RIGHT_ANSWER "p0502g0200%02d.png"

#define S5_MASTER_QUESTION_PICTURE "01一般(出題時)/p0502g0100%02d.png"
#define S5_MASTER_WRONG_PICTURE "03答錯/p0502g0100%02d.png"

#define S5_QUESTION_WAVE_PICTURE "p0502g0700%02d.png"

#define S5_FINISH_BACKGROUND_PICTURE "p0402g03000%d.png"
#define S5_FINISH_WORD_PICTURE "p0402g03%s.png"

//音樂
#define S5_BACKGROUND_MUSIC "HB0026BM0501.mp3"
#define S5_VAGETABLE_SELECTED_SOUND "HB0026SE0503.mp3"
#define S5_VAGETABLE_FELL_INTO_WATER_SOUND "HB0026SE0501.mp3"

#define S5_QUESTION_OS "%sHB0026OS0507.mp3"
#define S5_ANSWER_SELECTED_SOUND "HB0026SE0103.mp3"

#define S5_ANSWER_DROPPED_SOUND "HB0026SE0309.mp3"
#define S5_QUESTION_WAVE_SOUND "HB0026SE0501.mp3"
#define S5_CORRECT_SOUND "HB0026SE0502.mp3"
#define S5_DONKEY_LAUGH_SOUND "HB0026SE0504.mp3"
#define S5_DONKEY_DISAPPOINT_SOUND "HB0026SE0505.mp3"

#define S5_FINISH_SOUND "HB0026SE0306.mp3"

#define S5_PLAY_AGAIN_SOUND "HB0026SE0102.mp3"

enum VagetableStatus
{
    VAGETABLE_ON_TAKING = 0,
    VAGETABLE_ON_THE_WATER,
    VAGETABLE_UNDER_THE_WATER,
};

class Scene5 : public cocos2d::CCLayer
{
public:
    Scene5();
    ~Scene5();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    virtual void draw();
    
    void tick(float dt);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Scene5);
    
protected:
    //共用
    FunctionMenu* functionMenu;
    ActorsLines* actorsLines;
    
    CCMenu* goToGameMenu;
    
    bool isGameOpenning;
    
    void gotoGame();
    
    void preSceneCallBackFunc();
    
    void nextSceneCallBackFunc();
    
    void choseSceneClickedCallFunc(cocos2d::CCObject *pSender, int chapterID);
    
    void functionStatusCallBackFunc(cocos2d::CCObject *pSender, int functionMenuStatus);
    
    void closeWordsWithClickedBackgroundCallBack();
    
    void openWordsCallBack();
    
    void whenLeaving();
    
    void setSpriteObjectAtContent(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* content);
    
    void actionSettingWithBeginNumber(CCActionInterval*& targetAction, const char* name, int count, int repeatCount, float delayPerUnit, int beginNumber);
    
    CCSpriteFrameCache* imageCache;
    
    //end
    
    //故事
    static const int vagetableCount = 8;
    
    CCLayer* storyLayer;
    CCLayer* gameLayer;
    
    CCSprite* background;
    CCSprite* master;
    CCSprite* donkey;
    
    TouchSpriteWithPassEvent* vagetablesSprite[vagetableCount];
    b2Body* vagetableBodiesInNormal[vagetableCount];
    
    b2Body* vagetableBodies[vagetableCount];
    
    b2World* world;
    GLESDebugDraw *m_debugDraw;
    
    MyContactListener *contactListener;
    
    void createStoryInterActiveObject(int index);
    
    b2MouseJoint* mouseJoint;
    
    CCPoint vagetablePosition[vagetableCount];
    
    void createStoryObjects();
    
    void vagetableTouchBegan(cocos2d::CCObject *pSender);
    void vagetableTouchEnded(cocos2d::CCObject *pSender);
    void vagetableTouchMoved(cocos2d::CCObject *pSender);
    
    void createVagetableNormalB2Body(int index);
    
    int selectIndexId;
    
    CCActionInterval* vagetableNormalAction[vagetableCount];
    
    void setAllAction();
    
    CCSequence* vagetableFallInWater[vagetableCount];
    
    void vagetableFellDown(CCObject* pSender);
    
    CCActionInterval* masterNormalAndRightAction;
    CCActionInterval* donkeyNormalAction;
    
    CCSprite* wave;
    
    CCActionInterval* waveAction;
    
    //遊戲
    
    //再玩一次
    CCLayer* playAgainLayer;
    CCMenu* playAgainMenu;
    CCSprite* playAgainWord;
    
    CCSequence* playAgainLayerMoveOut;
    CCSequence* playAgainLayerMoveIn;
    
    void onReplayClicked();
    
    void onPlayAgainLayerMovedIn();
    void onPlayAgainLayerMovedOut();
    //end
    
    CCLayer* answerLayer;
    
    static const int answerCount = 4;
    static const int wrongAnswerCount = 8;
    
    CCSequence* answerLayerMoveOut;
    CCSequence* answerLayerMoveIn;
    
    void showTheQuestion();
    
    void onAnswerLayerMovedIn();
    void onAnswerLayerMovedOut();
    
    void createGameObjects();
    
    CCPoint answerPosition[answerCount];
    CCSprite* stage[answerCount];
    
    void getRandomAndNoRepeat(int *randomArray, int xCount, int yCount);
    
    int rightAnswerArray[answerCount];
    
    CCActionInterval* questionFadeIn;
    CCActionInterval* questionFadeOut;
    CCActionInterval* questionWordFadeIn;
    CCActionInterval* questionWordFadeOut;
    
    CCSprite* questionBackground;
    CCSprite* questionWord;
    
    int questionIndex;
    
    MoveableSpr* answerSprite[answerCount];
    CCSprite* answerWord[answerCount];
    
    void createAnswerObject();
    
    void onAnswerDrop(CCObject* pSender);
    void onAnswerDrag(CCObject* pSender);
    
    bool checkPointIsInAnswerLayer(CCPoint point);
    
    CCPoint answerOriginalPosition;
    
    void setAllAnswerMoveableAndShow(bool moveable);
    
    CCSequence* waveRightAction;
    CCSequence* waveWrongAction;
    
    CCSequence* donkeyWrongAction;
    CCSequence* donkeyRightAction;
    
    CCActionInterval* masterRightAction;
    CCActionInterval* masterWrongAction;
    
    CCActionInterval* masterQuestionNormalAction;
    
    void onWrongActionDone();
    void onRightActionDone();
    
    void onDonkeyWrongActionDone();
    void onDonkeyRightActionDone();
    
    CCSprite* questionWaveSprite;
    
    CCPoint donkeyPosition;
    
    void playGameNormalAction();
    
    CCActionInterval* finishAction;
    
    CCSprite* finishBackground;
    CCSprite* finishWord;
    
    int questionOSId;
    
    const char* getLanguageName(bool isTwoType);
    
    void changeLanguage();
    
    void onLanguageChanged();
    
    void setOtherAnswerMoveable(bool moveable);
    
    CCMenu* answerCover;
    
    b2Body* groundBody;
};

#endif // __HELLOWORLD_Scene5_H__
