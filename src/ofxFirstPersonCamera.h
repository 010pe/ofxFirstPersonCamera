#pragma once

#include "ofMain.h"

class ofxFirstPersonCamera : public ofCamera
{
  public:

    ofxFirstPersonCamera();
   ~ofxFirstPersonCamera();

    bool isControlled();
    void toggleControl();
    void enableControl();
    void disableControl();

    int keyUp;
    int keyDown;
    int keyLeft;
    int keyRight;
    int keyForward;
    int keyBackward;
    int keyRollLeft;
    int keyRollRight;
    int keyRollReset;

    float movespeed;
    float rollspeed;
    float sensitivity;

    ofVec3f upvector;

  protected:

    void update(ofEventArgs&);

    void keyPressed(ofKeyEventArgs&);
    void keyReleased(ofKeyEventArgs&);
    void mouseMoved(ofMouseEventArgs&);
    void mouseDragged(ofMouseEventArgs&);

  private:

    void nodeRotate(ofMouseEventArgs&);

    struct Pressed {
      bool Up;
      bool Down;
      bool Left;
      bool Right;
      bool Forward;
      bool Backward;
      bool RollLeft;
      bool RollRight;
      bool RollReset;
    } Actions;

    bool IsControlled;
    bool IsMouseInited;

    GLFWwindow* GLFWWindow;
};
