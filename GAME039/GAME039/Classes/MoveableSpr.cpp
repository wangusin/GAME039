//
//  MoveableSpr
//

#include "MoveableSpr.h"

#define AnimationSpeed 1.0f/12.0f

using namespace cocos2d;

MoveableSpr* MoveableSpr::createBySpriteFrameName(const char *spName){
//    MoveableSpr *pobSprite = new MoveableSpr();
//    
//    if (pobSprite && pobSprite->initWithSpriteFrameName(spName))
//    {
//        pobSprite->myInit();
//        pobSprite->autorelease();
//        return pobSprite;
//    }
//    CC_SAFE_DELETE(pobSprite);
//    
//    return NULL;
    
    int touchPriority = -120;
    
    return createBySpriteFramNameWithTouchPriority(spName, touchPriority);
}

MoveableSpr* MoveableSpr::createBySpriteFramNameWithTouchPriority(const char *spName, int touchPriority){
    MoveableSpr *pobSprite = new MoveableSpr();
    
    if (pobSprite && pobSprite->initWithSpriteFrameName(spName))
    {
        pobSprite->touchPriority = touchPriority;
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;
}

void MoveableSpr::myInit(){
//    CCLOG("MoveableSpr init");
    m_pListener = NULL;
    m_touchBeganCallBack = NULL;
    m_touchEndCallBack = NULL;
    
    moveable = true;
    alphaOnOff = true;
    
}
MoveableSpr::~MoveableSpr(){
//    CCLOG("~MoveableSpr");
    m_pListener = NULL;
    m_touchBeganCallBack = NULL;
    m_touchEndCallBack = NULL;
    
}

void MoveableSpr::onEnter(){
    //因為CCMenu的priority是 -128
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, touchPriority, true);
    
    CCNode::onEnter();
}

void MoveableSpr::onExit(){
    CCLog("MoveableSpr tag = %d", this->getTag());
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

#pragma mark -
#pragma mark TouchDelegate
bool MoveableSpr::ccTouchBegan(CCTouch* touch, CCEvent* event){
    
    if (!moveable)
        return false;
    
    if (this->containsTouchLocation(touch)) {
        
//        CCLOG("Moveable Spr touch");
        if (m_pListener && m_touchBeganCallBack)
        {
            (m_pListener->*m_touchBeganCallBack)(this);
        }
        
        return true;
    }
    
    return false;
}
void MoveableSpr::ccTouchMoved(CCTouch* touch, CCEvent* event){
    
    CCPoint touchLocation = touch->getLocationInView();
    
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    this->setPosition(ccp(touchLocation.x, touchLocation.y));
    
}

void MoveableSpr::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if (m_pListener && m_touchEndCallBack)
    {
        (m_pListener->*m_touchEndCallBack)(this);
    }
}

#pragma mark -

bool MoveableSpr::containsTouchLocation(CCTouch *touch){
    
    CCPoint touchLocation = touch->getLocationInView();
    
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    CCRect thisRect = CCRectMake(getPositionX() - getContentSize().width*getAnchorPoint().x,
                                 getPositionY() - getContentSize().height*getAnchorPoint().y,
                                 getContentSize().width, getContentSize().height);
    
    bool isTouched = thisRect.containsPoint(touchLocation);
    
    if (isTouched) {
        
        if (!alphaOnOff)
            return true;
        
        //取得圖片該點的RGBA 做判斷.
        CCPoint nodeLocation = this->convertToNodeSpace(touchLocation);
        
        //-----
        u_int8_t data[4];
        CCRenderTexture *renderTexture = CCRenderTexture::create(getContentSize().width, getContentSize().height);
        
        renderTexture->begin();
        
        this->draw();
        
        glReadPixels(nodeLocation.x, nodeLocation.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        renderTexture->end();
        
        renderTexture->release();
        
        int alpha = data[3];
        
        if (alpha < 85) {
            // alpha 小於 85 當做透明.
            //不處理點擊觸碰.
            
            return false;
        }
    }
    
    return isTouched;
}

void MoveableSpr::setTarget(CCObject *rec, SEL_MenuHandler selector, SEL_MenuHandler touchEndCallBack){
    m_pListener = rec;
    m_touchBeganCallBack = selector;
    m_touchEndCallBack = touchEndCallBack;
}

void MoveableSpr::setMoveable(bool moveable)
{
    this->moveable = moveable;
}

void MoveableSpr::setWithAlpha(bool alphaOnOff)
{
    this->alphaOnOff = alphaOnOff;
}