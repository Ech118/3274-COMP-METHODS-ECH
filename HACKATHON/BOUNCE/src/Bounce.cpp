// SoQt includes
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodes/SoCamera.h>

#include "Ball.h"
#include "Box.h"

#include <QApplication>
#include <QWidget>

#include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <random>


// Arg ball ref
void callback(void* data, SoSensor* sensor)
{
  // Cast
  auto* balls = (std::vector<std::unique_ptr<Ball>>*)data;
  for (auto& b : *balls) b->update(0.01f);

  for (size_t i = 0; i < balls->size(); i++)
        for (size_t j = i + 1; j < balls->size(); j++)
            (*balls)[i]->handleBallCollision(*(*balls)[j]);
}


// Callback function to handle keyboard events
void keyboardCallback(void* userData, SoEventCallback* eventCB) {
    const SoEvent* event = eventCB->getEvent();

    // Type cast
    auto* balls = (std::vector<std::unique_ptr<Ball>>*)userData;

    // Check if the event is a keyboard event
    if (event->isOfType(SoKeyboardEvent::getClassTypeId())) {
        const SoKeyboardEvent* keyEvent = (const SoKeyboardEvent*)event;

        // Check if the key was pressed
        if (SoKeyboardEvent::isKeyPressEvent(event, keyEvent->getKey())) {

            // Pick the kick direction
            SbVec3f kick(0, 0, 0);
            bool dissipate = false;
            switch (keyEvent->getKey()) {
                case SoKeyboardEvent::LEFT_ARROW:
                case SoKeyboardEvent::PAD_4:    // Pad checks are for mac support
                    kick.setValue(-5, 0, 0);
                    break;
                case SoKeyboardEvent::UP_ARROW:
                case SoKeyboardEvent::PAD_8:
                    kick.setValue(0, 0, -5);
                    break;
                case SoKeyboardEvent::RIGHT_ARROW:
                case SoKeyboardEvent::PAD_6:
                    kick.setValue(5, 0, 0);
                    break;
                case SoKeyboardEvent::DOWN_ARROW:
                case SoKeyboardEvent::PAD_2:
                    kick.setValue(0, 0, 5);
                    break;
                case SoKeyboardEvent::LEFT_SHIFT:
                case SoKeyboardEvent::RIGHT_SHIFT:
                    dissipate = true;
                    break;
                default:
                    return;     // Not one of our keys
            }

            // Kick every ball
            for (auto& b : *balls) 
            {
              if(dissipate)
              {
                b->scaleVelocity(0.5f);
              } 
              b->addVelocity(kick);
            } 
            if(dissipate) dissipate = false;

            // Tell Open Inventor that this event has been handled
            eventCB->setHandled();
        }
    }
}


int main(int argc, char **argv)
{
#ifndef __APPLE__
  setenv("QT_QPA_PLATFORM","xcb",0);
#endif
  
  int numBalls = 1;  
  float RADIUS = 1.0f;
  SbVec3f GRAVITY{0, -9.81f, 0};
  if (argc > 1) numBalls = std::stoi(argv[1]);
  if (numBalls < 1) numBalls = 1;
  if (argc > 2) RADIUS = std::stof(argv[2]);
  if (argc > 3) GRAVITY = SbVec3f(0, std::stof(argv[3]), 0);

  // Initialize the Qt system:
  QApplication app(argc, argv);
  
  // Make a main window:
  QWidget mainwin;
  mainwin.resize(800, 800);

  // Initialize SoQt 
  SoQt::init(&mainwin);
  
  // The root of a scene graph
  SoSeparator *root = new SoSeparator;
  root->ref();

  float HALF = 10.0f;
  // Container of size (2HALF)^3
  Box box(HALF);
  root->addChild(box.getNode());

  std::mt19937 rng(std::random_device{}());
  float lim = HALF - RADIUS;                        
  std::uniform_real_distribution<float> dist(-lim, lim);
  std::vector<std::unique_ptr<Ball>> balls;

  for (int i = 0; i < numBalls; i++) {
      SbVec3f p(dist(rng), dist(rng), dist(rng));
      balls.push_back(std::make_unique<Ball>(RADIUS, GRAVITY, p, &box));
  }
    
  for (auto& b : balls) root->addChild(b->getNode());

  // Create the event callback node and add it to the scene graph
  SoEventCallback* eventCallbackNode = new SoEventCallback;
  eventCallbackNode->addEventCallback(SoKeyboardEvent::getClassTypeId(), keyboardCallback, &balls);
  root->addChild(eventCallbackNode);

  // Initialize an examiner viewer:
  SoQtExaminerViewer * eviewer = new SoQtExaminerViewer(&mainwin);
#ifndef __APPLE__
  eviewer->setDoubleBuffer(false); // Needed for Wayland window manager.
#endif
  
  // send events to the scene graph instead of the camera controls
  eviewer->setSceneGraph(root);
  eviewer->setViewing(false);

  SoCamera* cam = eviewer->getCamera();
    cam->position.setValue(0, 0, 30);       // 30 units back along +z
    cam->pointAt(SbVec3f(0, 0, 0));        // box's center
  eviewer->show();

    // Schedule the update right here: 
    SoTimerSensor * timerSensor = new SoTimerSensor;
    timerSensor->setFunction(callback);
    timerSensor->setInterval(0.01);
    timerSensor->setData(&balls);
    timerSensor->schedule();
  
  // Pop up the main window.
  SoQt::show(&mainwin);

  // Loop until exit.
  SoQt::mainLoop();

  // Clean up resources.
  delete timerSensor;
  delete eviewer;
  root->unref();
  return 0;
}
