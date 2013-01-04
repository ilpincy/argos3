/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_joystick.cpp>
 *
 * @brief This file contains the implementation of the QTOpenGLJoystick class.
 * It is heavily based on the code found at http://www.batcom-it.net/?p=59.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_joystick.h"
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLJoystick::CQTOpenGLJoystick(QObject *parent, int joystickEventTimeout, bool doAutoRepeat, int repeatDelay)
      : QObject(parent)
   {
      if ( SDL_Init(SDL_INIT_JOYSTICK) == 0 ) {
         int i;
         for (i = 0; i < SDL_NumJoysticks(); i++) {
            joystickNames.append(SDL_JoystickName(i));
            LOG << "[INFO] Found joystick #" << i << ": \"" << SDL_JoystickName(i) << "\"" << std::endl;
         }
         connect(&joystickTimer, SIGNAL(timeout()), this, SLOT(processEvents()));
      } else {
         THROW_ARGOSEXCEPTION("QTOpenGLJoystick: couldn't initialize SDL joystick support");
      }

      joystick = NULL;
      numAxes = numButtons = numHats = numTrackballs = 0;
      autoRepeat = doAutoRepeat;
      autoRepeatDelay = repeatDelay;
      eventTimeout = joystickEventTimeout;
   }

/****************************************/
/****************************************/

   CQTOpenGLJoystick::~CQTOpenGLJoystick()
   {
      if ( isOpen() )
         close();

      SDL_Quit();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLJoystick::open(int stick)
   {
      if ( isOpen() )
         close();

      joystick = SDL_JoystickOpen(stick);
      if ( joystick ) {
         numAxes = SDL_JoystickNumAxes(joystick);
         numButtons = SDL_JoystickNumButtons(joystick);
         numHats = SDL_JoystickNumHats(joystick);
         numTrackballs = SDL_JoystickNumBalls(joystick);
         joystickTimer.start(eventTimeout);
      } else {
         THROW_ARGOSEXCEPTION("QTOpenGLJoystick: couldn't open SDL joystick #%d" << stick);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLJoystick::close()
   {
      joystickTimer.stop();
      if ( joystick )
         SDL_JoystickClose(joystick);
      joystick = NULL;
      numAxes = numButtons = numHats = numTrackballs = 0;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLJoystick::processEvents()
   {
      if ( !isOpen() )
         return;

      SDL_JoystickUpdate();

      int i;
      for (i = 0; i < numAxes; i++) {
         Sint16 moved = SDL_JoystickGetAxis(joystick, i);
         if ( abs(moved) >= deadzones[i] ) {
            if ( (moved != axes[i]) ) {
               int deltaMoved = abs(axes[i] - moved);
               if ( deltaMoved >= sensitivities[i] )
                  emit axisValueChanged(i, moved);
               axes[i] = moved;
               axisRepeatTimers[i].restart();
            } else if (autoRepeat && moved != 0) {
               if ( axisRepeatTimers[i].elapsed() >= autoRepeatDelay ) {
                  emit axisValueChanged(i, moved);
                  axes[i] = moved;
               }
            } else
               axisRepeatTimers[i].restart();
         } else
            emit axisValueChanged(i, 0);
      }
      for (i = 0; i < numButtons; i++) {
         Uint8 changed = SDL_JoystickGetButton(joystick, i);
         if ( (changed != buttons[i]) ) {
            emit buttonValueChanged(i, (bool) changed);
            buttons[i] = changed;
            buttonRepeatTimers[i].restart();
         } else if (autoRepeat && changed != 0) {
            if ( buttonRepeatTimers[i].elapsed() >= autoRepeatDelay ) {
               emit buttonValueChanged(i, (bool) changed);
               buttons[i] = changed;
            }
         } else
            buttonRepeatTimers[i].restart();
      }
      for (i = 0; i < numHats; i++) {
         Uint8 changed = SDL_JoystickGetHat(joystick, i);
         if ( (changed != hats[i]) ) {
            emit hatValueChanged(i, changed);
            hats[i] = changed;
            hatRepeatTimers[i].restart();
         } else if (autoRepeat && changed != 0) {
            if ( hatRepeatTimers[i].elapsed() >= autoRepeatDelay ) {
               emit hatValueChanged(i, changed);
               hats[i] = changed;
            }
         } else
            hatRepeatTimers[i].restart();
      }

      for (i = 0; i < numTrackballs; i++) {
         int dx, dy;
         SDL_JoystickGetBall(joystick, i, &dx, &dy);
         if ( dx != 0 || dy != 0 )
            emit trackballValueChanged(i, dx, dy);
      }
   }

   /****************************************/
   /****************************************/

   int CQTOpenGLJoystick::getAxisValue(int axis)
   {
      if ( isOpen() ) {
         SDL_JoystickUpdate();
         return SDL_JoystickGetAxis(joystick, axis);
      } else
         return 0;
   }

   /****************************************/
   /****************************************/

}
