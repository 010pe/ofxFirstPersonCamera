ofxFirstPersonCamera
====================


Lightweight camera class that replicates camera controls of first person video games. It hides cursor on activation and has reassignable keys. Tested on Linux and Windows, should work on Mac as well.


Dependencies
------------

#### 1. OF 0.9.0 and C++11
#### 3. [ofxGLFWUsefulFunctions](https://github.com/procedural/ofxGLFWUsefulFunctions)

Compiling
---------

For [openFrameworks][1]:

Download and extract folder to your `openFrameworks/addons` directory. Go to `openFrameworks/apps/projectGenerator/projectGeneratorSimple` folder and build it if it's not compiled already. Then go to relative `bin` folder and run [projectGeneratorSimple][2]. Select this addon in the list of non-core addons. When you finish generating project folder, you can find it in `openFrameworks/apps/myApps` directory, `mySketch` is default name for it.

For [CMake-based openFrameworks][3]:

Add this repo as a git submodule to your [app][4] folder and use `ofxaddon` command in `CMakeLists.txt`.


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
    void setup  ();
    void update ();
    void draw   ();

    ofxFirstPersonCamera cam;
};
```

And enable camera control in `ofApp.cpp` on `setup` call:

```cpp
#include "ofApp.h"

void ofApp::setup()
{
}

void ofApp::update()
{
  if (!cam.isControlled())
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

### [example-ofxFirstPersonCamera](https://github.com/procedural/example-ofxFirstPersonCamera)


  [1]: https://github.com/openframeworks/openFrameworks
  [2]: https://github.com/ofZach/projectGeneratorSimple
  [3]: https://github.com/procedural/of
  [4]: https://github.com/procedural/ofApp

