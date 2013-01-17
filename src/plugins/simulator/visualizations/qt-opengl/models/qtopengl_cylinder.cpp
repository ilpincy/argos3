/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/models/qtopengl_cylinder.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_cylinder.h"
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/simulator/entity/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real LED_RADIUS = 0.01f;

   /****************************************/
   /****************************************/

   CQTOpenGLCylinder::CQTOpenGLCylinder() :
      m_unVertices(20) {

      /* Reserve the needed display lists */
      m_unBaseList = glGenLists(1);
      m_unBodyList = m_unBaseList;
      m_unLEDList = m_unBaseList + 1;

      /* Make body list */
      glNewList(m_unBodyList, GL_COMPILE);
      MakeBody();
      glEndList();

      /* Make LED list */
      glNewList(m_unLEDList, GL_COMPILE);
      MakeLED();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLCylinder::~CQTOpenGLCylinder() {
      glDeleteLists(m_unBaseList, 2);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCylinder::DrawLEDs(CCylinderEntity& c_entity) {
      /* Draw the LEDs */
      GLfloat pfColor[]           = {   0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 100.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
      CLedEquippedEntity& cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
      for(UInt32 i = 0; i < cLEDEquippedEntity.GetAllLeds().size(); ++i) {
         glPushMatrix();
         /* Set the material */
         const CColor& cColor = cLEDEquippedEntity.GetLED(i).GetColor();
         pfColor[0] = cColor.GetRed();
         pfColor[1] = cColor.GetGreen();
         pfColor[2] = cColor.GetBlue();
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
         /* Perform rototranslation */
         const CVector3& cPosition = cLEDEquippedEntity.GetLED(i).GetPosition();
         glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
         /* Draw the LED */
         glCallList(m_unLEDList);
         glPopMatrix();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCylinder::Draw(CCylinderEntity& c_entity) {
      /* Draw the body */
      glPushMatrix();
      glScalef(c_entity.GetRadius(), c_entity.GetRadius(), c_entity.GetHeight());
      glCallList(m_unBodyList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCylinder::MakeBody() {
      /* Since this shape can be stretched,
         make sure the normal vectors are unit-long */
      glEnable(GL_NORMALIZE);

      /* Set the material */
      const GLfloat pfColor[]     = { 0.0f, 1.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 0.0f                   };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);

      /* Let's start the actual shape */
      /* Side surface */
      CVector2 cVertex(1.0f, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(),  0.5f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), -0.5f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Top disk */
      cVertex.Set(1.0f, 0.0f);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(),  0.5f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Bottom disk */
      cVertex.Set(1.0f, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(),  -0.5f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* The shape definition is finished */

      /* We don't need it anymore */
      glDisable(GL_NORMALIZE);

   }

   /****************************************/
   /****************************************/

   void CQTOpenGLCylinder::MakeLED() {
      CVector3 cNormal, cPoint;
      CRadians cSlice(CRadians::TWO_PI / m_unVertices);

      glBegin(GL_TRIANGLE_STRIP);
      for(CRadians cInclination; cInclination <= CRadians::PI; cInclination += cSlice) {
         for(CRadians cAzimuth; cAzimuth <= CRadians::TWO_PI; cAzimuth += cSlice) {

            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth + cSlice);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth + cSlice);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

         }
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawCylinder : public CQTOpenGLOperationDraw {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CCylinderEntity& c_entity) {
         c_visualization.DrawPositionalEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   private:
      CQTOpenGLCylinder m_cModel;
   };

   REGISTER_ENTITY_OPERATION(CQTOpenGLOperationDraw, CQTOpenGLWidget, CQTOpenGLOperationDrawCylinder, void, CCylinderEntity);

   /****************************************/
   /****************************************/

}
