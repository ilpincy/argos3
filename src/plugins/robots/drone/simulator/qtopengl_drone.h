/**
 * @file <argos3/plugins/robots/drone/simulator/qtopengl_drone.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef QTOPENGL_DRONE_H
#define QTOPENGL_DRONE_H

namespace argos {
   class CQTOpenGLDrone;
   class CDroneEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>

namespace argos {

   class CQTOpenGLDrone {

   public:

      CQTOpenGLDrone();

      virtual ~CQTOpenGLDrone();

      virtual void Draw(const CDroneEntity& c_entity);

   private:
      /* OBJ models */
      CQTOpenGLObjModel m_cDroneModel;
      CQTOpenGLObjModel m_cPropellerModel;
      /* Pointer to the LED material within the OBJ model */
      std::array<CQTOpenGLObjModel::SMaterial*, 4> m_arrLEDs;
      const static CVector3 m_cPropellerOffset;
   };

   class CQTOpenGLDroneDebug {

   public:

      CQTOpenGLDroneDebug();

      virtual ~CQTOpenGLDroneDebug();

      virtual void Draw(const CDroneEntity& c_entity);

   private:

      void MakeCylinder();

      /* Display lists for the Drone */
      GLuint m_unDisplayList;
      GLuint m_unCylinderList;

      const static GLuint m_unVertices = 20;
      
      const static std::array<GLfloat, 4> m_arrDefaultColor;
      const static std::array<GLfloat, 4> m_arrDefaultSpecular;
      const static std::array<GLfloat, 4> m_arrDefaultEmission;
      const static std::array<GLfloat, 1> m_arrDefaultShininess;

      const static CVector3 m_cExtents;
   };

}

#endif
