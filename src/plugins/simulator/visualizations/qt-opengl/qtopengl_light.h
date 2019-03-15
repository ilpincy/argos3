/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/models/qtopengl_light.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_LIGHT_H
#define QTOPENGL_LIGHT_H

namespace argos {
   class CQTOpenGLLight;
   class CLightEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLLight {

   public:

      CQTOpenGLLight();

      virtual ~CQTOpenGLLight();

      virtual void Draw(CLightEntity& c_entity);

   private:

      GLuint m_unList;
      GLuint m_unVertexes;
      GLfloat m_fRadius;

   };

}

#endif
