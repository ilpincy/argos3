/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/models/qtopengl_light.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_light.h"
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/entities/light_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLight::CQTOpenGLLight() :
      m_unVertexes(20),
      m_fRadius(0.1f) {
      /* Reserve a display list */
      m_unList = glGenLists(1);

      /* Start the display list */
      glNewList(m_unList, GL_COMPILE);

      /* Let's start the actual shape, a sphere */
      CVector3 cNormal, cPoint;
      CRadians cSlice(CRadians::TWO_PI / m_unVertexes);

      glBegin(GL_TRIANGLE_STRIP);
      for(CRadians cInclination; cInclination <= CRadians::PI; cInclination += cSlice) {
         for(CRadians cAzimuth; cAzimuth <= CRadians::TWO_PI; cAzimuth += cSlice) {

            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth);
            cPoint = m_fRadius * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth);
            cPoint = m_fRadius * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth + cSlice);
            cPoint = m_fRadius * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth + cSlice);
            cPoint = m_fRadius * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

         }
      }
      glEnd();

      /* End of the display list */
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLight::~CQTOpenGLLight() {
      glDeleteLists(m_unList, 1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLight::Draw(CLightEntity& c_entity) {
      /* Set the material */
      const CColor& cColor = c_entity.GetColor();
      const GLfloat pfColor[]     = { cColor.GetRed()   / 255.0f,
                                      cColor.GetGreen() / 255.0f,
                                      cColor.GetBlue()  / 255.0f,
                                      1.0f };
      const GLfloat pfSpecular[]  = { 0.0f,  0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 100.0f                  };
      const GLfloat pfEmission[]  = { 0.0f,  0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);

      glCallList(m_unList);
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawLightNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CLightEntity& c_entity) {
         static CQTOpenGLLight m_cModel;
         c_visualization.DrawEntity(c_entity);
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawLightSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CLightEntity& c_entity) {
         static CQTOpenGLLight m_cModel;
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         c_visualization.DrawEntity(c_entity);
         glScalef(1.1, 1.1, 1.1);
         m_cModel.Draw(c_entity);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawLightNormal, CLightEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawLightSelected, CLightEntity);

   /****************************************/
   /****************************************/

}
