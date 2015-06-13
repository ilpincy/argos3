/**
 * @file <argos3/plugins/robots/spiri/simulator/qtopengl_spiri.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_SPIRI_H
#define QTOPENGL_SPIRI_H

namespace argos {
   class CQTOpenGLSpiri;
   class CSpiriEntity;
}


#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLSpiri {

   public:

      CQTOpenGLSpiri();

      virtual ~CQTOpenGLSpiri();

      virtual void Draw(CSpiriEntity& c_entity);

   private:

      void SetPlasticMaterial();

      void SetPropellerMaterial();

      void SetMainBodyMaterial();

      void MakeModel();

      void MakePropeller();

      void MakeMainBody();

      void MakeRing(GLfloat f_out_radius,
                    GLfloat f_in_radius);

      void MakeDisk(GLfloat f_radius);

      void MakeCylinderSurface(GLfloat f_radius,
                               GLfloat f_height);

      void MakeCylinder(GLfloat f_radius,
                        GLfloat f_height);

      void MakeBox(GLfloat f_length,
                   GLfloat f_width,
                   GLfloat f_height);

   private:

      /* Display list */
      GLuint m_unList;

      /* Number of vertices to display the round parts */
      GLuint m_unVertices;

   };

}

#endif
