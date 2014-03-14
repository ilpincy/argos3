/**
 * @file <argos3/plugins/robots/eye-bot/simulator/qtopengl_eyebot.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_EYEBOT_H
#define QTOPENGL_EYEBOT_H

namespace argos {
   class CQTOpenGLEyeBot;
   class CEyeBotEntity;
}


#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLEyeBot {

   public:

      CQTOpenGLEyeBot();

      virtual ~CQTOpenGLEyeBot();

      virtual void Draw(CEyeBotEntity& c_entity);

   protected:

      /** Sets a colored LED material */
      void SetLEDMaterial(GLfloat f_red, GLfloat f_green, GLfloat f_blue);

      /** Sets a shiny gray material */
      void SetBodyMaterial();

      /** An eye-bot leg */
      void MakeLeg();

      /** A single LED of the side ring */
      void DrawSideLED();

      /** A single LED of the bottom ring */
      void DrawBottomLED();

      /** The eye-bot body */
      void DrawBody();

   private:

      /** Start of the display list index */
      GLuint m_unLists;

      /** Leg display list */
      GLuint m_unLegList;

      /** Body display list */
      GLuint m_unBodyList;

      /** Side LED display list */
      GLuint m_unSideLEDList;

      /** Bottom LED display list */
      GLuint m_unBottomLEDList;

      /** Number of vertices to display the round parts */
      GLuint m_unVertices;

      /* Angle gap between two leds */
      GLfloat m_fLEDAngleSlice;

   };

}

#endif
