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

    int upKey       = GLFW_KEY_E;
    int downKey     = GLFW_KEY_C;
    int leftKey     = GLFW_KEY_A;
    int rightKey    = GLFW_KEY_D;
    int forwardKey  = GLFW_KEY_W;
    int backwardKey = GLFW_KEY_S;

    float movespeed   = 1.00f;
    float sensitivity = 0.10f;

  protected:

    void update (ofEventArgs&);

    void keyPressed   (ofKeyEventArgs&);
    void keyReleased  (ofKeyEventArgs&);
    void mouseMoved   (ofMouseEventArgs&);
    void mouseDragged (ofMouseEventArgs&);

    void updateCamPosition ();
    void updateCamRotation (ofMouseEventArgs&);

  private:

    bool IsRegistered;
    bool IsControlled;

    float PAngle;
    float YAngle;

    bool Up;
    bool Down;
    bool Left;
    bool Right;
    bool Forward;
    bool Backward;
};
