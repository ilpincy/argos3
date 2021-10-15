/**
 * @file <argos3/plugins/robots/builderbot/simulator/qtopengl_builderbot.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef QTOPENGL_BUILDERBOT_H
#define QTOPENGL_BUILDERBOT_H

namespace argos {
   class CQTOpenGLBuilderBot;
   class CBuilderBotEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>

namespace argos {

   class CQTOpenGLBuilderBot {

   public:

      CQTOpenGLBuilderBot();

      virtual ~CQTOpenGLBuilderBot();

      virtual void Draw(const CBuilderBotEntity& c_entity);

   private:
      /* OBJ models */
      CQTOpenGLObjModel m_cBuilderBotModel;
      CQTOpenGLObjModel m_cBuilderBotManipulatorModel;
      /* The tag texture */
      const std::array<std::array<GLfloat, 3>, 100> m_arrTagTexture;
      /* Precompiled list for drawing tags */
      GLuint m_unTagList;
      /* Pointers to the LED material within the OBJ model */
      std::array<CQTOpenGLObjModel::SMaterial*, 12> m_arrLEDs;
   };


   class CQTOpenGLBuilderBotDebug {

   public:

      CQTOpenGLBuilderBotDebug();

      virtual ~CQTOpenGLBuilderBotDebug();

      virtual void Draw(const CBuilderBotEntity& c_entity);

   private:

      void MakeBox();

      void MakeCylinder();

      void MakeSphere();

      void MakeUpperBase();

      /* Display lists for the BuilderBot */
      GLuint m_unDisplayList;
      GLuint m_unBoxList;
      GLuint m_unCylinderList;
      GLuint m_unSphereList;
      GLuint m_unUpperBaseList;

      std::array<std::tuple<std::string, GLuint, CVector3>, 8> m_arrItemsToDraw;

      const static GLuint m_unVertices = 20;
      
      const static CVector3 m_cLowerBaseScaling;
      const static CVector3 m_cWheelScaling;
      const static CVector3 m_cPivotScaling;
      const static CVector3 m_cUpperBaseScaling;
      const static CVector3 m_cLiftColumnScaling;
      const static CVector3 m_cEndEffectorScaling;
      const static CVector3 m_cEndEffectorSupportScaling;
      
      const static std::array<GLfloat, 4> m_arrDefaultColor;
      const static std::array<GLfloat, 4> m_arrDefaultSpecular;
      const static std::array<GLfloat, 4> m_arrDefaultEmission;
      const static std::array<GLfloat, 1> m_arrDefaultShininess;
   };

}

#endif
