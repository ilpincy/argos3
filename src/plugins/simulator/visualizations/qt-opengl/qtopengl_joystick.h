/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_joystick.h>
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
