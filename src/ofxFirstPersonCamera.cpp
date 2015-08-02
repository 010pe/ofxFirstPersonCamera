#include "ofxFirstPersonCamera.h"

ofxFirstPersonCamera::ofxFirstPersonCamera()
:upvector      (0,1,0)
,movespeed     (1.00f)
,rollspeed     (1.00f)
,sensitivity   (0.10f)
,keyUp         (GLFW_KEY_E)
,keyDown       (GLFW_KEY_C)
,keyLeft       (GLFW_KEY_A)
,keyRight      (GLFW_KEY_D)
,keyForward    (GLFW_KEY_W)
,keyBackward   (GLFW_KEY_S)
,keyRollLeft   (GLFW_KEY_Q)
,keyRollRight  (GLFW_KEY_R)
,keyRollReset  (GLFW_KEY_F)
,WinWidth      (0)
,WinHeight     (0)
,WinCenterX    (0)
,WinCenterY    (0)
,IsControlled  (false)
,IsMouseInited (false)
{
  ofAddListener(ofEvents().update      , this, &ofxFirstPersonCamera::update);
  ofAddListener(ofEvents().keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
  ofAddListener(ofEvents().keyReleased , this, &ofxFirstPersonCamera::keyReleased);
  ofAddListener(ofEvents().mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
  ofAddListener(ofEvents().mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
}

ofxFirstPersonCamera::~ofxFirstPersonCamera()
{
  ofRemoveListener(ofEvents().update      , this, &ofxFirstPersonCamera::update);
  ofRemoveListener(ofEvents().keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
  ofRemoveListener(ofEvents().keyReleased , this, &ofxFirstPersonCamera::keyReleased);
  ofRemoveListener(ofEvents().mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
  ofRemoveListener(ofEvents().mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
}

void ofxFirstPersonCamera::enableControl()
{
  if(IsControlled) return;

  GLFWWindow = static_cast<ofAppGLFWWindow*>(ofGetWindowPtr())->getGLFWWindow();

  glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

# ifdef TARGET_LINUX
  Display* X11Display = ofGetX11Display();
  Window X11Window = ofGetX11Window();
  const char Nil[] = {0};
  XColor Col;
  Pixmap Pix = XCreateBitmapFromData(X11Display, X11Window, Nil, 1, 1);
  Cursor Cur = XCreatePixmapCursor(X11Display, Pix, Pix, &Col, &Col, 0, 0);
  XDefineCursor(X11Display, X11Window, Cur);
  XFreeCursor(X11Display, Cur);
# endif

  glfwGetWindowSize(GLFWWindow, &WinWidth, &WinHeight);

  WinCenterX = WinWidth  / 2.0f;
  WinCenterY = WinHeight / 2.0f;

  glfwSetCursorPos(GLFWWindow, WinCenterX, WinCenterY);

  IsControlled = true;
}

void ofxFirstPersonCamera::disableControl()
{
  if(!IsControlled) return;

  glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

# ifdef TARGET_LINUX
  XUndefineCursor(ofGetX11Display(), ofGetX11Window());
# endif

  IsControlled = false;
}

void ofxFirstPersonCamera::keyPressed(ofKeyEventArgs& keys)
{
  Pressed go = Actions;
  int key = keys.keycode;

  if      (key == keyUp       ) go.Up        = true;
  else if (key == keyDown     ) go.Down      = true;
  else if (key == keyLeft     ) go.Left      = true;
  else if (key == keyRight    ) go.Right     = true;
  else if (key == keyForward  ) go.Forward   = true;
  else if (key == keyBackward ) go.Backward  = true;

  else if (key == keyRollLeft ) go.RollLeft  = true;
  else if (key == keyRollRight) go.RollRight = true;
  else if (key == keyRollReset) go.RollReset = true;

  Actions = go;
}

void ofxFirstPersonCamera::keyReleased(ofKeyEventArgs& keys)
{
  Pressed go = Actions;
  int key = keys.keycode;

  if      (key == keyUp       ) go.Up        = false;
  else if (key == keyDown     ) go.Down      = false;
  else if (key == keyLeft     ) go.Left      = false;
  else if (key == keyRight    ) go.Right     = false;
  else if (key == keyForward  ) go.Forward   = false;
  else if (key == keyBackward ) go.Backward  = false;

  else if (key == keyRollLeft ) go.RollLeft  = false;
  else if (key == keyRollRight) go.RollRight = false;
  else if (key == keyRollReset) go.RollReset = false;

  Actions = go;
}

void ofxFirstPersonCamera::update(ofEventArgs& args)
{
  bool controlled = IsControlled;
  if (!controlled) return;

  Pressed go = Actions;

  if (go.Up||go.Down||go.Left||go.Right||go.Forward||go.Backward) this->updateCamPosition();

  if (go.RollLeft||go.RollRight||go.RollReset) this->updateCamRoll();
}

void ofxFirstPersonCamera::mouseMoved(ofMouseEventArgs& mouse)
{
  bool controlled = IsControlled;
  if (!controlled) return;

  this->updateCamRotation(mouse);
}

void ofxFirstPersonCamera::mouseDragged(ofMouseEventArgs& mouse)
{
  bool controlled = IsControlled;
  if (!controlled) return;

  this->updateCamRotation(mouse);
}

void ofxFirstPersonCamera::updateCamRoll()
{
  Pressed go = Actions;

  int rolldir = go.RollLeft - go.RollRight;
  if (rolldir != 0) {
    float roll = rollspeed;
    float rate = ofGetFrameRate();
    float angle = roll * (60.0f / rate);
    this->roll(rolldir * angle);
    ofVec3f updir = this->getUpDir();
    upvector = updir;
  }

  if (go.RollReset) {
    float thisroll = this->getRoll();
    this->roll(-thisroll);
    upvector = ofVec3f(0, 1, 0);
  }
}

void ofxFirstPersonCamera::updateCamPosition()
{
  float move = movespeed;
  float rate = ofGetFrameRate();
  float speed = move * (60.0f / rate);

  Pressed go = Actions;

  ofVec3f lookdir = this->getLookAtDir();
  ofVec3f sidedir = this->getSideDir();
  ofVec3f updir = this->getUpDir();

  this->move(lookdir * speed * (go.Forward - go.Backward) +
             sidedir * speed * (go.Right - go.Left) +
               updir * speed * (go.Up - go.Down));
}

void ofxFirstPersonCamera::updateCamRotation(ofMouseEventArgs& mouse)
{
  bool mouseIsNotInited = !IsMouseInited;
  // Fixes first mouse move
  if (mouseIsNotInited) {
    mouse.x = WinCenterX;
    mouse.y = WinCenterY;
    IsMouseInited = true;
  }

  float mouse_x = mouse.x;
  float mouse_y = mouse.y;
  float wcent_x = WinCenterX;
  float wcent_y = WinCenterY;
  // Window center and mouse position difference
  float xdiff = wcent_x - mouse_x;
  float ydiff = wcent_y - mouse_y;

  float sensit = sensitivity;
  // Apply sensitivity
  xdiff *= sensit;
  ydiff *= sensit;

  ofVec3f upvec = upvector;
  ofVec3f sidev = this->getSideDir();
  // Rotate our camera
  this->rotate(xdiff, upvec);
  this->rotate(ydiff, sidev);

  // Set cursor position to the center of the window
  glfwSetCursorPos(GLFWWindow, wcent_x, wcent_y);
}

bool ofxFirstPersonCamera::isControlled()
{
  return IsControlled;
}
