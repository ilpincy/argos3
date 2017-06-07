/**
 * @file <argos3/plugins/robots/kilobot/simulator/qtopengl_kilobot.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#include "qtopengl_kilobot.h"
#include "kilobot_measures.h"
#include "kilobot_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   /* All measures are in meters */


   /****************************************/
   /****************************************/

   CQTOpenGLKilobot::CQTOpenGLKilobot() :
      m_unVertices(40) {
      /* Reserve the needed display lists */
      m_unLists = glGenLists(4);

      /* Assign indices for better referencing (later) */
      m_unBasicWheelList            = m_unLists;
      m_unWheelList                 = m_unLists + 1;
      m_unBaseList                  = m_unLists + 2;
      m_unLEDList                   = m_unLists + 3;

      // /* Create the materialless wheel display list */
      // glNewList(m_unBasicWheelList, GL_COMPILE);
      // MakeWheel();
      // glEndList();

      /* Create the wheel display list */
      glNewList(m_unWheelList, GL_COMPILE);
      RenderWheel();
      glEndList();

      /* Create the base module display list */
      glNewList(m_unBaseList, GL_COMPILE);
      RenderBase();
      glEndList();

      /* Create the LED display list */
      glNewList(m_unLEDList, GL_COMPILE);
      RenderLED();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLKilobot::~CQTOpenGLKilobot() {
      glDeleteLists(m_unLists, 4);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::Draw(CKilobotEntity& c_entity) {
      /* Place the pins */
      glPushMatrix();
      glTranslatef(KILOBOT_FRONT_PIN_DISTANCE, 0.0, 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(0.0f, KILOBOT_HALF_INTERPIN_DISTANCE, 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(0.0f, -KILOBOT_HALF_INTERPIN_DISTANCE, 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
      /* Place the base */
      glPushMatrix();
      glTranslatef(KILOBOT_ECCENTRICITY, 0.0f, 0.0f);
      glCallList(m_unBaseList);
      glPopMatrix();
      /* Place the beacon */
      CLEDEquippedEntity& cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
      const CColor& cLEDColor = cLEDEquippedEntity.GetLED(0).GetColor();
      SetLEDMaterial((GLfloat)cLEDColor.GetRed()/255,
                     (GLfloat)cLEDColor.GetGreen()/255,
                     (GLfloat)cLEDColor.GetBlue()/255);
      glCallList(m_unLEDList);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::SetWhitePlasticMaterial() {
      const GLfloat pfColor[]     = {   1.0f, 1.0f, 1.0f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.9f, 0.9f, 0.9f, 1.0f };
      const GLfloat pfShininess[] = { 100.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::SetBlackTireMaterial() {
      const GLfloat pfColor[]     = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 0.0f                   };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::SetCircuitBoardMaterial() {
      const GLfloat pfColor[]     = { 0.0f, 0.0f, 1.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.5f, 0.5f, 1.0f, 1.0f };
      const GLfloat pfShininess[] = { 10.0f                  };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::SetLEDMaterial(GLfloat f_red, GLfloat f_green, GLfloat f_blue) {
      const GLfloat pfColor[]     = {f_red, f_green, f_blue, 1.0f };
      const GLfloat pfSpecular[]  = { 0.0f,    0.0f,   0.0f, 1.0f };
      const GLfloat pfShininess[] = { 0.0f };
      const GLfloat pfEmission[]  = {f_red, f_green, f_blue, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::RenderWheel() {
      /* Set material */
      SetWhitePlasticMaterial();
      CVector2 cVertex(KILOBOT_PIN_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom side */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f );
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(KILOBOT_PIN_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_PIN_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(KILOBOT_PIN_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(),  KILOBOT_PIN_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }


   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::RenderBase() {
      /* Set material */
      SetCircuitBoardMaterial();
      /* Circuit board */
      CVector2 cVertex(KILOBOT_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_PIN_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(KILOBOT_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_PIN_HEIGHT);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(KILOBOT_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKilobot::RenderLED() {
      CVector2 cVertex(KILOBOT_LED_RADIUS, 0.0f);
      CRadians cAngle(-CRadians::TWO_PI / m_unVertices);
      /* Bottom part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Side surface */
      cAngle = -cAngle;
      CVector2 cNormal(1.0f, 0.0f);
      cVertex.Set(KILOBOT_LED_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_HEIGHT + KILOBOT_LED_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_HEIGHT);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Top part */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      cVertex.Set(KILOBOT_LED_RADIUS, 0.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), KILOBOT_HEIGHT + KILOBOT_LED_HEIGHT);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }


   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawKilobotNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CKilobotEntity& c_entity) {
         static CQTOpenGLKilobot m_cModel;
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawKilobotSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CKilobotEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawKilobotNormal, CKilobotEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawKilobotSelected, CKilobotEntity);

   /****************************************/
   /****************************************/

}
