#pragma once

#include <ofMain.h>

#include "ofxGLFWUsefulFunctions.h"

class ofxFirstPersonCamera : public ofCamera
{
  public:

    ofxFirstPersonCamera();
   ~ofxFirstPersonCamera();

    bool isControlled   ();
    void enableControl  ();
    void disableControl ();

    int keyUp        = GLFW_KEY_E;
    int keyDown      = GLFW_KEY_C;
    int keyLeft      = GLFW_KEY_A;
    int keyRight     = GLFW_KEY_D;
    int keyForward   = GLFW_KEY_W;
    int keyBackward  = GLFW_KEY_S;
    int keyRollLeft  = GLFW_KEY_Q;
    int keyRollRight = GLFW_KEY_R;
    int keyRollReset = GLFW_KEY_F;

    float movespeed   = 1.00f;
    float rollspeed   = 1.00f;
    float sensitivity = 0.10f;

    ofVec3f upvector = ofVec3f(0, 1, 0);

  protected:

    void update (ofEventArgs&);

    void keyPressed   (ofKeyEventArgs&);
    void keyReleased  (ofKeyEventArgs&);
    void mouseMoved   (ofMouseEventArgs&);
    void mouseDragged (ofMouseEventArgs&);

    void updateCamRoll     ();
    void updateCamPosition ();
    void updateCamRotation (ofMouseEventArgs&);

  private:

    bool IsRegistered;
    bool IsControlled;
    bool IsMouseInited;

    bool Up;
    bool Down;
    bool Left;
    bool Right;
    bool Forward;
    bool Backward;
    bool RollLeft;
    bool RollRight;
    bool RollReset;
};
