#include "ofxFirstPersonCamera.h"

ofxFirstPersonCamera::ofxFirstPersonCamera()
{
  IsRegistered  = false;
  IsControlled  = false;
  IsMouseInited = false;

  Up        = false;
  Down      = false;
  Left      = false;
  Right     = false;
  Forward   = false;
  Backward  = false;
  RollLeft  = false;
  RollRight = false;
  RollReset = false;

  UpVector = ofVec3f(0, 1, 0);
}

ofxFirstPersonCamera::~ofxFirstPersonCamera()
{
  disableControl();
}

void ofxFirstPersonCamera::enableControl()
{
  ofxHideMouse();

  {
    int w; int h;
    glfwGetWindowSize(ofxGetGLFWWindow(), &w, &h);
    glfwSetCursorPos (ofxGetGLFWWindow(), w / 2.0f, h / 2.0f);
  }

  if (!IsRegistered) {
    ofAddListener(ofEvents().update      , this, &ofxFirstPersonCamera::update);
    ofAddListener(ofEvents().keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
    ofAddListener(ofEvents().keyReleased , this, &ofxFirstPersonCamera::keyReleased);
    ofAddListener(ofEvents().mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
    IsRegistered = true;
  }

  IsControlled = true;
}

void ofxFirstPersonCamera::disableControl()
{
  ofxShowMouse();

  Up        = false;
  Down      = false;
  Left      = false;
  Right     = false;
  Forward   = false;
  Backward  = false;
  RollLeft  = false;
  RollRight = false;
  RollReset = false;

  if (IsRegistered) {
    ofRemoveListener(ofEvents().update      , this, &ofxFirstPersonCamera::update);
    ofRemoveListener(ofEvents().keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
    ofRemoveListener(ofEvents().keyReleased , this, &ofxFirstPersonCamera::keyReleased);
    ofRemoveListener(ofEvents().mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
    IsRegistered = false;
  }

  IsControlled = false;
}

void ofxFirstPersonCamera::keyPressed(ofKeyEventArgs& key)
{
  if      (key.keycode == keyUp       ) Up        = true;
  else if (key.keycode == keyDown     ) Down      = true;
  else if (key.keycode == keyLeft     ) Left      = true;
  else if (key.keycode == keyRight    ) Right     = true;
  else if (key.keycode == keyForward  ) Forward   = true;
  else if (key.keycode == keyBackward ) Backward  = true;

  else if (key.keycode == keyRollLeft ) RollLeft  = true;
  else if (key.keycode == keyRollRight) RollRight = true;
  else if (key.keycode == keyRollReset) RollReset = true;
}

void ofxFirstPersonCamera::keyReleased(ofKeyEventArgs& key)
{
  if      (key.keycode == keyUp       ) Up        = false;
  else if (key.keycode == keyDown     ) Down      = false;
  else if (key.keycode == keyLeft     ) Left      = false;
  else if (key.keycode == keyRight    ) Right     = false;
  else if (key.keycode == keyForward  ) Forward   = false;
  else if (key.keycode == keyBackward ) Backward  = false;

  else if (key.keycode == keyRollLeft ) RollLeft  = false;
  else if (key.keycode == keyRollRight) RollRight = false;
  else if (key.keycode == keyRollReset) RollReset = false;
}

void ofxFirstPersonCamera::update(ofEventArgs& args)
{
  if (Up||Down||Left||Right||Forward||Backward) updateCamPosition();

  if (RollLeft||RollRight||RollReset) updateCamRoll();
}

void ofxFirstPersonCamera::mouseMoved(ofMouseEventArgs& mouse)
{
  updateCamRotation(mouse);
}

void ofxFirstPersonCamera::mouseDragged(ofMouseEventArgs& mouse)
{
  updateCamRotation(mouse);
}

void ofxFirstPersonCamera::updateCamRoll()
{
  if (RollLeft)  { roll(  rollspeed * (60.0f / ofGetFrameRate()) ); UpVector = getUpDir(); }
  if (RollRight) { roll( -rollspeed * (60.0f / ofGetFrameRate()) ); UpVector = getUpDir(); }
  if (RollReset) { roll( -getRoll() ); UpVector = ofVec3f(0, 1, 0); }
}

void ofxFirstPersonCamera::updateCamPosition()
{
  move(getLookAtDir() * ( movespeed * (60.0f / ofGetFrameRate()) ) * (Forward-Backward) +
         getSideDir() * ( movespeed * (60.0f / ofGetFrameRate()) ) * (Right-Left) +
           getUpDir() * ( movespeed * (60.0f / ofGetFrameRate()) ) * (Up-Down));
}

void ofxFirstPersonCamera::updateCamRotation(ofMouseEventArgs& mouse)
{
  int w; int h;
  glfwGetWindowSize(ofxGetGLFWWindow(), &w, &h);

  if (!IsMouseInited) {
    // Fix a first mouse move glitch
    mouse.x = (w / 2.0f);
    mouse.y = (h / 2.0f);
    IsMouseInited = true;
  }

  {
    // Window center and mouse position difference
    float xdiff = (w / 2.0f) - mouse.x;
    float ydiff = (h / 2.0f) - mouse.y;

    // Apply sensitivity
    xdiff *= sensitivity;
    ydiff *= sensitivity;

    // Rotate our camera
    rotate(xdiff, UpVector);
    rotate(ydiff, getSideDir());
  }

  // Set cursor position to the center of the window
  glfwSetCursorPos (ofxGetGLFWWindow(), w / 2.0f, h / 2.0f);
}

bool ofxFirstPersonCamera::isControlled()
{
  return IsControlled;
}
