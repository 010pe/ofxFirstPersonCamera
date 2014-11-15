#include "ofxFirstPersonCamera.h"

ofxFirstPersonCamera::ofxFirstPersonCamera()
{
  IsRegistered = false;
  IsControlled = false;

  Up       = false;
  Down     = false;
  Left     = false;
  Right    = false;
  Forward  = false;
  Backward = false;

  UpVector = ofVec3f(0, 1, 0);
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
  Left     = false;
  Right    = false;
  Forward  = false;
  Backward = false;

  if (IsRegistered) {
    IsRegistered = false;
    ofRemoveListener(ofEvents().update      , this, &ofxFirstPersonCamera::update);
    ofRemoveListener(ofEvents().keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
    ofRemoveListener(ofEvents().keyReleased , this, &ofxFirstPersonCamera::keyReleased);
    ofRemoveListener(ofEvents().mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
  }
}

void ofxFirstPersonCamera::keyPressed(ofKeyEventArgs& key)
{
  if      (key.keycode == keyUp       ) Up       = true;
  else if (key.keycode == keyDown     ) Down     = true;
  else if (key.keycode == keyLeft     ) Left     = true;
  else if (key.keycode == keyRight    ) Right    = true;
  else if (key.keycode == keyForward  ) Forward  = true;
  else if (key.keycode == keyBackward ) Backward = true;

  if      (key.keycode == keyRollLeft ) { roll( rollspeed); UpVector = getUpDir(); }
  else if (key.keycode == keyRollRight) { roll(-rollspeed); UpVector = getUpDir(); }
  else if (key.keycode == keyRollReset) { roll(-getRoll()); UpVector = ofVec3f(0, 1, 0); }
}

void ofxFirstPersonCamera::keyReleased(ofKeyEventArgs& key)
{
  if      (key.keycode == keyUp      ) Up       = false;
  else if (key.keycode == keyDown    ) Down     = false;
  else if (key.keycode == keyLeft    ) Left     = false;
  else if (key.keycode == keyRight   ) Right    = false;
  else if (key.keycode == keyForward ) Forward  = false;
  else if (key.keycode == keyBackward) Backward = false;
}

void ofxFirstPersonCamera::update(ofEventArgs& args)
{
  if (Up||Down||Left||Right||Forward||Backward) updateCamPosition();
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
  move(getLookAtDir() * movespeed * (Forward-Backward) +
         getSideDir() * movespeed * (Right-Left) +
           getUpDir() * movespeed * (Up-Down));
}

void ofxFirstPersonCamera::updateCamRotation(ofMouseEventArgs& mouse)
{
  {
    // Mouse position and window center difference
    float xdiff = (ofGetWidth()  / 2.0f) - mouse.x;
    float ydiff = (ofGetHeight() / 2.0f) - mouse.y;

    // Apply sensitivity
    xdiff *= sensitivity;
    ydiff *= sensitivity;

    // Rotate our camera
    rotate(xdiff, UpVector);
    rotate(ydiff, getSideDir());
  }

  // Set cursor position to the center of the window
  glfwSetCursorPos(ofxGetGLFWWindow(), ofGetWidth()/2, ofGetHeight()/2);
}

bool ofxFirstPersonCamera::isControlled()
{
  return IsControlled;
}
