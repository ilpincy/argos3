/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/models/qtopengl_box.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_BOX_H
#define QTOPENGL_BOX_H

namespace argos {
   class CQTOpenGLBox;
   class CBoxEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLBox {

   public:

      CQTOpenGLBox();

      virtual ~CQTOpenGLBox();

      virtual void DrawLEDs(CBoxEntity& c_entity);
      virtual void Draw(const CBoxEntity& c_entity);

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
