/**
 * @file <argos3/plugins/robots/pi-puck/simulator/qtopengl_pipuck.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef QTOPENGL_PIPUCK_H
#define QTOPENGL_PIPUCK_H

namespace argos {
   class CQTOpenGLPiPuck;
   class CPiPuckEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>

#include <array>

namespace argos {

   class CQTOpenGLPiPuck {

   public:

      CQTOpenGLPiPuck();

      virtual ~CQTOpenGLPiPuck();

      virtual void Draw(const CPiPuckEntity& c_entity);

   private:
      /* OBJ models */
      CQTOpenGLObjModel m_cPiPuckModel;
      CQTOpenGLObjModel m_cPiPuckWheelModel;
      /* The tag texture */
      static const std::array<std::array<GLfloat, 3>, 100> m_arrTagTexture;
      /* LED material overrides */
      static const std::array<GLfloat, 4> m_arrRingLedOffAmbientDiffuse;
      static const std::array<GLfloat, 4> m_arrRingLedOnAmbientDiffuse;
      static const std::array<GLfloat, 4> m_arrBodyLedOffAmbientDiffuse;
      static const std::array<GLfloat, 4> m_arrBodyLedOnAmbientDiffuse;
      static const std::array<GLfloat, 4> m_arrFrontLedOffAmbientDiffuse;
      static const std::array<GLfloat, 4> m_arrFrontLedOnAmbientDiffuse;
      /* Precompiled list for drawing tags */
      GLuint m_unTagList;
      /* Pointer to the LED material within the OBJ model */
      std::array<CQTOpenGLObjModel::SMaterial*, 8> m_arrRingLEDs;
      CQTOpenGLObjModel::SMaterial& m_sBodyLED;
      CQTOpenGLObjModel::SMaterial& m_sFrontLED;
   };


   class CQTOpenGLPiPuckDebug {

   public:

      CQTOpenGLPiPuckDebug();

      virtual ~CQTOpenGLPiPuckDebug();

      virtual void Draw(const CPiPuckEntity& c_entity);

   private:
      void MakeCylinder();

      /* Display lists for the PiPuck */
      GLuint m_unDisplayList;
      GLuint m_unCylinderList;

      std::array<std::tuple<std::string, GLuint, CVector3>, 3> m_arrItemsToDraw;

      const static GLuint m_unVertices = 20;
      
      const static CVector3 m_cBodyScaling;
      const static CVector3 m_cWheelScaling;
      
      const static std::array<GLfloat, 4> m_arrDefaultColor;
      const static std::array<GLfloat, 4> m_arrDefaultSpecular;
      const static std::array<GLfloat, 4> m_arrDefaultEmission;
      const static std::array<GLfloat, 1> m_arrDefaultShininess;
   };

}

#endif
