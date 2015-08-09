#include "ofxFirstPersonCamera.h"

ofxFirstPersonCamera::ofxFirstPersonCamera()
:IsMouseInited (false)
,upvector      (0,1,0)
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
{
  auto &events = ofEvents();
  ofAddListener(events.update      , this, &ofxFirstPersonCamera::update);
  ofAddListener(events.keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
  ofAddListener(events.keyReleased , this, &ofxFirstPersonCamera::keyReleased);
  ofAddListener(events.mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
  ofAddListener(events.mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
}

ofxFirstPersonCamera::~ofxFirstPersonCamera()
{
  auto &events = ofEvents();
  ofRemoveListener(events.update      , this, &ofxFirstPersonCamera::update);
  ofRemoveListener(events.keyPressed  , this, &ofxFirstPersonCamera::keyPressed);
  ofRemoveListener(events.keyReleased , this, &ofxFirstPersonCamera::keyReleased);
  ofRemoveListener(events.mouseMoved  , this, &ofxFirstPersonCamera::mouseMoved);
  ofRemoveListener(events.mouseDragged, this, &ofxFirstPersonCamera::mouseDragged);
}

void ofxFirstPersonCamera::enableControl()
{
  IsControlled = true;

  GLFWWindow = static_cast<ofAppGLFWWindow*>(ofGetWindowPtr())->getGLFWWindow();

  glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

# ifdef TARGET_LINUX
  Display* x11Display = ofGetX11Display();
  Window x11Window = ofGetX11Window();
  const char data[] = {0};
  Pixmap pixels = XCreateBitmapFromData(x11Display, x11Window, data, 1, 1);
  XColor color;
  Cursor cursor = XCreatePixmapCursor(x11Display, pixels, pixels, &color, &color, 0, 0);
  XDefineCursor(x11Display, x11Window, cursor);
  XFreeCursor(x11Display, cursor);
# endif

  int win_w;
  int win_h;
  auto win = GLFWWindow;
  glfwGetWindowSize(win, &win_w, &win_h);
  int win_center_x = win_w / 2.0f;
  int win_center_y = win_h / 2.0f;
  glfwSetCursorPos(win, win_center_x, win_center_y);
}

void ofxFirstPersonCamera::disableControl()
{
  IsControlled = false;

  glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

# ifdef TARGET_LINUX
  XUndefineCursor(ofGetX11Display(), ofGetX11Window());
# endif
}

void ofxFirstPersonCamera::update(ofEventArgs& args)
{
  if(!IsControlled) return;

  { // Roll
    Pressed act = Actions;
    bool unroll = act.RollReset;
    int rolldir = act.RollLeft - act.RollRight;

    if (rolldir) {
      float roll = rollspeed;
      float rate = ofGetFrameRate();
      float angle = roll * (60.0f / rate);
      this->roll(rolldir * angle);
      upvector = this->getUpDir();
    }

    if (unroll) {
      this->roll(-this->getRoll());
      upvector = ofVec3f(0, 1, 0);
    }
  }
  { // Position
    Pressed act = Actions;

    float look = act.Forward - act.Backward;
    float side = act.Right - act.Left;
    float up   = act.Up - act.Down;

    if (look != 0 || side != 0 || up != 0)
    {
      ofVec3f lookdir = this->getLookAtDir();
      ofVec3f sidedir = this->getSideDir();
      ofVec3f updir   = this->getUpDir();
      float move  = movespeed;
      float rate  = ofGetFrameRate();
      float speed = move * (60.0f / rate);
      this->move(lookdir * speed * look +
                 sidedir * speed * side +
                   updir * speed * up);
    }
  }
}

void ofxFirstPersonCamera::nodeRotate(ofMouseEventArgs& mouse)
{
  if(!IsControlled) return;

  int win_w;
  int win_h;
  auto win = GLFWWindow;
  glfwGetWindowSize(win, &win_w, &win_h);
  int win_center_x = win_w / 2.0f;
  int win_center_y = win_h / 2.0f;

  float mouse_x = mouse.x;
  float mouse_y = mouse.y;
  if (!IsMouseInited) {
  // Fixes first mouse move
    mouse_x = win_center_x;
    mouse_y = win_center_y;
    IsMouseInited = true;
  }

  float sensit = sensitivity;
  float xdiff = (win_center_x - mouse_x) * sensit;
  float ydiff = (win_center_y - mouse_y) * sensit;

  ofVec3f upvec = upvector;
  ofVec3f sidev = this->getSideDir();
  this->rotate(xdiff, upvec);
  this->rotate(ydiff, sidev);

  glfwSetCursorPos(win, win_center_x, win_center_y);
}

void ofxFirstPersonCamera::mouseMoved(ofMouseEventArgs& mouse)
{
  nodeRotate(mouse);
}

void ofxFirstPersonCamera::mouseDragged(ofMouseEventArgs& mouse)
{
  nodeRotate(mouse);
}

void ofxFirstPersonCamera::keyPressed(ofKeyEventArgs& keys)
{
  Pressed act = Actions;
  int key = keys.keycode;

  if      (key == keyUp       ) act.Up        = true;
  else if (key == keyDown     ) act.Down      = true;
  else if (key == keyLeft     ) act.Left      = true;
  else if (key == keyRight    ) act.Right     = true;
  else if (key == keyForward  ) act.Forward   = true;
  else if (key == keyBackward ) act.Backward  = true;

  else if (key == keyRollLeft ) act.RollLeft  = true;
  else if (key == keyRollRight) act.RollRight = true;
  else if (key == keyRollReset) act.RollReset = true;

  Actions = act;
}

void ofxFirstPersonCamera::keyReleased(ofKeyEventArgs& keys)
{
  Pressed act = Actions;
  int key = keys.keycode;

  if      (key == keyUp       ) act.Up        = false;
  else if (key == keyDown     ) act.Down      = false;
  else if (key == keyLeft     ) act.Left      = false;
  else if (key == keyRight    ) act.Right     = false;
  else if (key == keyForward  ) act.Forward   = false;
  else if (key == keyBackward ) act.Backward  = false;

  else if (key == keyRollLeft ) act.RollLeft  = false;
  else if (key == keyRollRight) act.RollRight = false;
  else if (key == keyRollReset) act.RollReset = false;

  Actions = act;
}
