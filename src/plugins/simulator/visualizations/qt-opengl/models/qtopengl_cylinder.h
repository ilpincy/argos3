/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/models/qtopengl_cylinder.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_CYLINDER_H
#define QTOPENGL_CYLINDER_H

namespace argos {
   class CQTOpenGLCylinder;
   class CCylinderEntity;
}


#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLCylinder {

   public:

      CQTOpenGLCylinder();

      virtual ~CQTOpenGLCylinder();

      void DrawLEDs(CCylinderEntity& c_entity);
      virtual void Draw(CCylinderEntity& c_entity);

   private:

      void MakeBody();
      void MakeLED();

   private:

      GLuint m_unBaseList;
      GLuint m_unBodyList;
      GLuint m_unLEDList;
      GLuint m_unVertices;

   };

}

#endif
