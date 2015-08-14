ofxFirstPersonCamera
====================


Lightweight ofCamera class that replicates camera controls of first person video games. It uses mouse to look around, hides cursor on activation and has reassignable keys which by default set to `WASD` for moving the camera, `E` and `C` to move camera up and down, `Q` and `R` to roll left and right and `F` to reset camera's up vector.

Tested on Linux and Windows, should work on Mac as well.


Dependencies
------------

#### 1. OF 0.9.0 and C++11

Compiling
---------

For [openFrameworks](https://github.com/openframeworks/openFrameworks):

[See wiki](https://github.com/ofnode/of/wiki/Compiling-ofApp-with-vanilla-openFrameworks)

For [CMake-based openFrameworks](https://github.com/ofnode/of):

Add this repo as a git submodule to your [ofApp](https://github.com/ofnode/ofApp) folder and use `ofxaddon` command in `CMakeLists.txt`.


How to use
----------

Include header file in `ofApp.h`, add an instance of `ofxFirstPersonCamera`:

```cpp
#pragma once

#include "ofMain.h"

#include "ofxFirstPersonCamera.h"

class ofApp : public ofBaseApp
{
  public:
    void mousePressed(ofMouseEventArgs &mouse);
    void draw();

    ofxFirstPersonCamera cam;
};
```

Enable first person camera control in `setup` call and set computed matrix to any ofCamera:

```cpp
#include "ofApp.h"

void ofApp::mousePressed(ofMouseEventArgs &mouse)
{
  cam.toggleControl();
}

void ofApp::draw()
{
  cam.begin();

    ofDrawGrid(10.0f, 10, true);

  cam.end();
}
```


Examples
--------

### [example-ofxFirstPersonCamera](https://github.com/ofnode/example-ofxFirstPersonCamera)
