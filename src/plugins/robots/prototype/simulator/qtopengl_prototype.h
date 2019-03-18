/**
 * @file <argos3/plugins/robots/prototype/simulator/qtopengl_prototype.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu - <zhuweixu_harry@126.com>
 */

#ifndef QTOPENGL_PROTOTYPE_H
#define QTOPENGL_PROTOTYPE_H

namespace argos {
   class CPrototypeEntity;
   class CVector3;
}

#include <argos3/core/utility/math/convex_hull.h>

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLPrototype {

   public:

      CQTOpenGLPrototype();

      virtual ~CQTOpenGLPrototype();

      virtual void DrawEntity(CPrototypeEntity& c_entity);

      virtual void DrawDevices(CPrototypeEntity& c_entity);

   private:

      void MakeBox();
      void MakeCylinder();
      void MakeSphere();
      void MakeLED();
      void MakePoles();
      void MakeTag();
      void MakeTagTexture();

      void DrawConvexHull(const std::vector<CVector3>& vec_points,
                          const std::vector<CConvexHull::SFace>& vec_faces);

   private:

      GLuint m_unBaseList;
      GLuint m_unBoxList;
      GLuint m_unCylinderList;
      GLuint m_unSphereList;
      GLuint m_unLEDList;
      GLuint m_unPoleList;
      GLuint m_unTagList;
      GLuint m_unVertices;
      GLuint m_unTagTex;

      const GLfloat m_fTagTexture[48] = {
         1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
         /* shade the bottom corner in debug mode */
#ifndef NDEBUG
         0.5f, 0.5f, 0.5f
#else
         1.0f, 1.0f, 1.0f
#endif
      };
   };
}

#endif
