/**
 * @file <argos3/plugins/robots/e-puck/simulator/qtopengl_epuck.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_EPUCK_H
#define QTOPENGL_EPUCK_H

namespace argos {
   class CQTOpenGLEPuck;
   class CEPuckEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLEPuck {

   public:

      CQTOpenGLEPuck();

      virtual ~CQTOpenGLEPuck();

      virtual void Draw(CEPuckEntity& c_entity);

   protected:

      /** Sets a green plastic material */
      void SetGreenPlasticMaterial();
      /** Sets a red plastic material */
      void SetRedPlasticMaterial();
      /** Sets a circuit board material */
      void SetCircuitBoardMaterial();
      /** Sets a colored LED material */
      void SetLEDMaterial(GLfloat f_red,
                          GLfloat f_green,
                          GLfloat f_blue);

      /** Renders a wheel */
      void RenderWheel();
      /** Renders the chassis */
      void RenderChassis();
      /** Renders the body */
      void RenderBody();
      /** A single LED of the ring */
      void RenderLED();

   private:

      /** Start of the display list index */
      GLuint m_unLists;

      /** E-puck wheel */
      GLuint m_unWheelList;

      /** Chassis display list */
      GLuint m_unChassisList;

      /** Body display list */
      GLuint m_unBodyList;

      /** LED display list */
      GLuint m_unLEDList;

      /** Number of vertices to display the round parts
          (wheels, chassis, etc.) */
      GLuint m_unVertices;

      /* Angle gap between two leds */
      GLfloat m_fLEDAngleSlice;

   };

}

#endif
