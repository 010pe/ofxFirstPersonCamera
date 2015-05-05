ofxFirstPersonCamera
====================


Lightweight camera class that replicates camera controls of first person video games. It hides cursor on activation and has reassignable keys. Tested on Linux and Windows, should work on Mac as well.


Dependencies
------------

#### 1. OF 0.9.0 and C++11
#### 2. [ofxGLFWUsefulFunctions](https://github.com/ofnode/ofxGLFWUsefulFunctions)

Compiling
---------

For [openFrameworks](https://github.com/openframeworks/openFrameworks):

[See wiki](https://github.com/ofnode/of/wiki/Compiling-ofApp-with-vanilla-openFrameworks)

For [CMake-based openFrameworks](https://github.com/ofnode/of):

Add this repo as a git submodule to your [ofApp](https://github.com/ofnode/ofApp) folder and use `ofxaddon` command in `CMakeLists.txt`.


How to use
----------

Include header file in `ofApp.h`, add a camera instance as a public member of a class:

```cpp
#pragma once

#include <ofMain.h>

#include "ofxFirstPersonCamera.h"

class ofApp : public ofBaseApp
{
  public:
    void setup();
    void draw ();

    ofxFirstPersonCamera cam;
};
```

And enable camera control in `ofApp.cpp` on `setup` call:

```cpp
#include "ofApp.h"

void ofApp::setup()
{
  cam.enableControl();
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
