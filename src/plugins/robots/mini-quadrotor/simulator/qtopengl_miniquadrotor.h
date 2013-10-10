/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/qtopengl_miniquadrotor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_MINI_QUADROTOR_H
#define QTOPENGL_MINI_QUADROTOR_H

namespace argos {
   class CQTOpenGLMiniQuadrotor;
   class CMiniQuadrotorEntity;
}


#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLMiniQuadrotor {

   public:

      CQTOpenGLMiniQuadrotor();

      virtual ~CQTOpenGLMiniQuadrotor();

      virtual void Draw(CMiniQuadrotorEntity& c_entity);

   protected:

      /** Sets the body material */
      void SetBodyMaterial();

      /** Sets the board material */
      void SetBoardMaterial();

      /** Sets the blade material */
      void SetBladeMaterial();

      /** The robot body */
      void MakeBody();

      /** The robot board */
      void MakeBoard();

      /** On of the two trasversal arms to which propellers are attached */
      void MakeArm();

      /** One of the four propellers */
      void MakePropeller();

   private:

      /** Number of vertices to display the round parts */
      GLuint m_unVertices;

      /** Start of the display list index */
      GLuint m_unLists;

   };

}

#endif
