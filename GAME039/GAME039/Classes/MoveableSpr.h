//
//  MoveableSpr
//
//  可移動的 sprite 物件
//

#ifndef __Cinderella_X__MoveableSpr__
#define __Cinderella_X__MoveableSpr__

#include "cocos2d.h"

using namespace cocos2d;

class MoveableSpr : public CCSprite ,public CCTouchDelegate
{
public:
    // === Create Function ===
    static MoveableSpr* createBySpriteFrameName(const char *spName);
    
    static MoveableSpr* createBySpriteFramNameWithTouchPriority(const char *spName, int touchPriority);
    
    void myInit();
    virtual ~MoveableSpr();
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);    
    
    //-----------
    // 檢查點擊是否在sprite範圍內.
    bool containsTouchLocation(CCTouch *touch);
    
    // 設定觸碰判斷通過時時要執行的callback
    void setTarget(CCObject *rec, SEL_MenuHandler m_touchBeganCallBack, SEL_MenuHandler touchEndCallBack);
    
    void setMoveable(bool moveable);
    
    void setWithAlpha(bool alphaOnOff);
    
private:
    CCObject*       m_pListener;
    SEL_MenuHandler    m_touchBeganCallBack;
    SEL_MenuHandler m_touchEndCallBack;
    
    int touchPriority;
    
    bool moveable;
    
    bool alphaOnOff;
    
};

#endif /* defined(__Cinderella_X__MoveableSpr__) */
