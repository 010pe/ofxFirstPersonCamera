#include "ofxFirstPersonCamera.h"

ofxFirstPersonCamera::ofxFirstPersonCamera()
{
  IsRegistered = false;
  IsControlled = false;

  PAngle   = 0.00f;
  YAngle   = 0.00f;

  Up       = false;
  Down     = false;
  Left     = false;
  Right    = false;
  Forward  = false;
  Backward = false;
}

ofxFirstPersonCamera::~ofxFirstPersonCamera()
{
  disableControl();
}

void ofxFirstPersonCamera::enableControl()
{
  IsControlled = true;

  ofxHideMouse();

  glfwSetCursorPos(ofxGetGLFWWindow(), ofGetWidth()/2, ofGetHeight()/2);

  if (!IsRegistered) {
    IsRegistered = true;
    ofAddListener(ofEvents().update      , this, &ofxFirstPersonCamera::update);
    ofAddListener(ofEvents().keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
    ofAddListener(ofEvents().keyReleased , this, &ofxFirstPersonCamera::keyReleased);
    ofAddListener(ofEvents().mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
  }
}

void ofxFirstPersonCamera::disableControl()
{
  IsControlled = false;

  ofxShowMouse();

  Up       = false;
  Down     = false;
  Forward  = false;
  Left     = false;
  Backward = false;
  Right    = false;

  if (IsRegistered) {
    IsRegistered = false;
    ofRemoveListener(ofEvents().update      , this, &ofxFirstPersonCamera::update);
    ofRemoveListener(ofEvents().keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
    ofRemoveListener(ofEvents().keyReleased , this, &ofxFirstPersonCamera::keyReleased);
    ofRemoveListener(ofEvents().mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
  }
}

void ofxFirstPersonCamera::update(ofEventArgs& args)
{
  PAngle = getPitch();
  YAngle = getHeading();

  if (Up or Down or Forward or Left or Backward or Right) updateCamPosition();
}

void ofxFirstPersonCamera::keyPressed(ofKeyEventArgs& key)
{
  if      (key.keycode == upKey      ) Up       = true;
  else if (key.keycode == downKey    ) Down     = true;
  else if (key.keycode == leftKey    ) Left     = true;
  else if (key.keycode == rightKey   ) Right    = true;
  else if (key.keycode == forwardKey ) Forward  = true;
  else if (key.keycode == backwardKey) Backward = true;
}

void ofxFirstPersonCamera::keyReleased(ofKeyEventArgs& key)
{
  if      (key.keycode == upKey      ) Up       = false;
  else if (key.keycode == downKey    ) Down     = false;
  else if (key.keycode == leftKey    ) Left     = false;
  else if (key.keycode == rightKey   ) Right    = false;
  else if (key.keycode == forwardKey ) Forward  = false;
  else if (key.keycode == backwardKey) Backward = false;
}

void ofxFirstPersonCamera::mouseMoved(ofMouseEventArgs& mouse)
{
  updateCamRotation(mouse);
}

void ofxFirstPersonCamera::mouseDragged(ofMouseEventArgs& mouse)
{
  updateCamRotation(mouse);
}

void ofxFirstPersonCamera::updateCamPosition()
{
  {
    float P = PAngle * DEG_TO_RAD;
    float Y = YAngle * DEG_TO_RAD;

    ofVec3f LookVec = ofVec3f(-sin(Y)*cos(P), sin(P), -cos(Y)*cos(P));
    ofVec3f SideVec = ofVec3f(cos(Y), 0, -sin(Y));

    move(ofVec3f(0, 1, 0) * (Up-Down) * movespeed +
         LookVec * (Forward-Backward) * movespeed +
         SideVec * (Right-Left)       * movespeed);
  }
}

void ofxFirstPersonCamera::updateCamRotation(ofMouseEventArgs& mouse)
{
  {
    // Difference between mouse position and center
    float XDiff = mouse.x - (ofGetWidth () / 2.0f);
    float YDiff = mouse.y - (ofGetHeight() / 2.0f);

    XDiff *= sensitivity;
    YDiff *= sensitivity;

    PAngle -= YDiff;
    YAngle -= XDiff;
  }

  // Limit pitch angle to 180 degrees
  PAngle = ofClamp(PAngle, -90.0f, 90.0f);

  glfwSetCursorPos(ofxGetGLFWWindow(), ofGetWidth()/2, ofGetHeight()/2);

  // Room for improvements: make roll
  {
    ofMatrix4x4 View;
    View.rotate(PAngle, 1, 0, 0);
    View.rotate(YAngle, 0, 1, 0);
    View.translate(getPosition());
    setTransformMatrix(View);
  }
}

bool ofxFirstPersonCamera::isControlled()
{
  return IsControlled;
}
