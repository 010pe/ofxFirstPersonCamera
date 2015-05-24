#include "ofxFirstPersonCamera.h"

ofxFirstPersonCamera::ofxFirstPersonCamera()
:keyUp         (GLFW_KEY_E)
,keyDown       (GLFW_KEY_C)
,keyLeft       (GLFW_KEY_A)
,keyRight      (GLFW_KEY_D)
,keyForward    (GLFW_KEY_W)
,keyBackward   (GLFW_KEY_S)
,keyRollLeft   (GLFW_KEY_Q)
,keyRollRight  (GLFW_KEY_R)
,keyRollReset  (GLFW_KEY_F)
,movespeed     (1.00f)
,rollspeed     (1.00f)
,sensitivity   (0.10f)
,upvector      (0,1,0)
,IsControlled  (false)
,IsMouseInited (false)
,Up            (false)
,Down          (false)
,Left          (false)
,Right         (false)
,Forward       (false)
,Backward      (false)
,RollLeft      (false)
,RollRight     (false)
,RollReset     (false)
,WinWidth      (0)
,WinHeight     (0)
,WinCenterX    (0)
,WinCenterY    (0)
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
  glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

# ifdef TARGET_LINUX
  XUndefineCursor(ofGetX11Display(), ofGetX11Window());
# endif

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
  if (!IsControlled) return;

  if (Up||Down||Left||Right||Forward||Backward) updateCamPosition();

  if (RollLeft||RollRight||RollReset) updateCamRoll();
}

void ofxFirstPersonCamera::mouseMoved(ofMouseEventArgs& mouse)
{
  if (!IsControlled) return;

  updateCamRotation(mouse);
}

void ofxFirstPersonCamera::mouseDragged(ofMouseEventArgs& mouse)
{
  if (!IsControlled) return;

  updateCamRotation(mouse);
}

void ofxFirstPersonCamera::updateCamRoll()
{
  float angle = rollspeed * (60.0f / ofGetFrameRate());

  if (RollLeft)  { roll(  angle ); upvector = getUpDir(); }
  if (RollRight) { roll( -angle ); upvector = getUpDir(); }
  if (RollReset) { roll( -getRoll() ); upvector = ofVec3f(0, 1, 0); }
}

void ofxFirstPersonCamera::updateCamPosition()
{
  float speed = movespeed * (60.0f / ofGetFrameRate());

  move(getLookAtDir() * speed * (Forward-Backward) +
         getSideDir() * speed * (Right-Left) +
           getUpDir() * speed * (Up-Down));
}

void ofxFirstPersonCamera::updateCamRotation(ofMouseEventArgs& mouse)
{
  if (!IsMouseInited) { // This fixes glitch at first mouse move
    mouse.x = WinCenterX;
    mouse.y = WinCenterY;
    IsMouseInited = true;
  }

  {
    // Window center and mouse position difference
    float xdiff = WinCenterX - mouse.x;
    float ydiff = WinCenterY - mouse.y;

    // Apply sensitivity
    xdiff *= sensitivity;
    ydiff *= sensitivity;

    // Rotate our camera
    rotate(xdiff, upvector);
    rotate(ydiff, getSideDir());
  }

  // Set cursor position to the center of the window
  glfwSetCursorPos(GLFWWindow, WinCenterX, WinCenterY);
}

bool ofxFirstPersonCamera::isControlled()
{
  return IsControlled;
}
