/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <argos3/core/simulator/visualizations/qtopengl_joystick.h>
 *
 * @brief This file contains the definition of the QTOpenGLJoystick class.
 * It is heavily based on the code found at http://www.batcom-it.net/?p=59.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QT_OPENGL_JOYSTICK_H
#define QT_OPENGL_JOYSTICK_H

#include <QObject>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <SDL/SDL.h>

#define SDL_JOYSTICK_DEFAULT_EVENT_TIMEOUT         25
#define SDL_JOYSTICK_DEFAULT_AUTOREPEAT_DELAY      250

namespace argos {

   class CQTOpenGLJoystick : public QObject
   {
      Q_OBJECT

   public:
      QStringList joystickNames;
      SDL_Joystick *joystick;
      int numAxes;
      int numButtons;
      int numHats;
      int numTrackballs;
      int eventTimeout;
      int autoRepeatDelay;
      bool autoRepeat;
      QTimer joystickTimer;
      QMap<int, int> deadzones;
      QMap<int, int> sensitivities;

      CQTOpenGLJoystick(QObject *parent = 0,
                        int joystickEventTimeout = SDL_JOYSTICK_DEFAULT_EVENT_TIMEOUT,
                        bool doAutoRepeat = TRUE,
                        int autoRepeatDelay = SDL_JOYSTICK_DEFAULT_AUTOREPEAT_DELAY);
      ~CQTOpenGLJoystick();
      void open(int);
      void close();
      inline bool isOpen() const { return joystick != NULL; }
      int getAxisValue(int);
      inline bool connected() const { return !joystickNames.isEmpty(); }

   private:
      QMap<int, Sint16> axes;
      QMap<int, Uint8> buttons;
      QMap<int, Uint8> hats;
      QMap<int, QTime> axisRepeatTimers;
      QMap<int, QTime> buttonRepeatTimers;
      QMap<int, QTime> hatRepeatTimers;

   signals:
      void axisValueChanged(int axis, int value);
      void buttonValueChanged(int button, bool value);
      void hatValueChanged(int hat, int value);
      void trackballValueChanged(int trackball, int deltaX, int deltaY);

   public slots:
      void processEvents();
   };

}

#endif
