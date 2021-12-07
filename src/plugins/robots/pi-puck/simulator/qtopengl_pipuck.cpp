/**
 * @file <argos3/plugins/robots/pipuck/simulator/qtopengl_pipuck.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_pipuck.h"

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

#include <array>

#define BODY_LED_INDEX 8
#define FRONT_LED_INDEX 9

namespace argos {

   /****************************************/
   /****************************************/

    CQTOpenGLPiPuck::CQTOpenGLPiPuck() :
      /* create the model */
      m_cPiPuckModel("pipuck.obj"),
      m_cPiPuckWheelModel("pipuck-wheel.obj"),
      m_arrRingLEDs {
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-0"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-1"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-2"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-3"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-4"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-5"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-6"),
         &m_cPiPuckModel.GetMaterial("Red-Ring-LED-7"),
      },
      m_sBodyLED(m_cPiPuckModel.GetMaterial("Green-Body-LEDs")),
      m_sFrontLED(m_cPiPuckModel.GetMaterial("Front-facing-Red-LED")) {
   }

   /****************************************/
   /****************************************/

   CQTOpenGLPiPuck::~CQTOpenGLPiPuck() {}

   /****************************************/
   /****************************************/

   void CQTOpenGLPiPuck::Draw(const CPiPuckEntity& c_entity) {
      /* Update LED materials */
      const CDirectionalLEDEquippedEntity& c_leds = c_entity.GetDirectionalLEDEquippedEntity();
      for(UInt32 un_ring_led_index = 0;
          un_ring_led_index < 8;
          un_ring_led_index++) {
         if(c_leds.GetLED(un_ring_led_index).GetColor() == CColor::BLACK) {
            m_arrRingLEDs[un_ring_led_index]->Diffuse = m_arrRingLedOffAmbientDiffuse;
            m_arrRingLEDs[un_ring_led_index]->Ambient = m_arrRingLedOffAmbientDiffuse;
         }
         else {
            m_arrRingLEDs[un_ring_led_index]->Diffuse = m_arrRingLedOnAmbientDiffuse;
            m_arrRingLEDs[un_ring_led_index]->Ambient = m_arrRingLedOnAmbientDiffuse;
         }
      }
      if(c_leds.GetLED(BODY_LED_INDEX).GetColor() == CColor::BLACK) {
         m_sBodyLED.Diffuse = m_arrBodyLedOffAmbientDiffuse;
         m_sBodyLED.Ambient = m_arrBodyLedOffAmbientDiffuse;
      }
      else {
         m_sBodyLED.Diffuse = m_arrBodyLedOnAmbientDiffuse;
         m_sBodyLED.Ambient = m_arrBodyLedOnAmbientDiffuse;
      }
      if(c_leds.GetLED(FRONT_LED_INDEX).GetColor() == CColor::BLACK) {
         m_sFrontLED.Diffuse = m_arrFrontLedOffAmbientDiffuse;
         m_sFrontLED.Ambient = m_arrFrontLedOffAmbientDiffuse;
      }
      else {
         m_sFrontLED.Diffuse = m_arrFrontLedOnAmbientDiffuse;
         m_sFrontLED.Ambient = m_arrFrontLedOnAmbientDiffuse;
      }
      CRadians cZAngle, cYAngle, cXAngle;
      /* draw the body */
      const SAnchor& sBodyAnchor = c_entity.GetEmbodiedEntity().GetAnchor("body");
      sBodyAnchor.Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(sBodyAnchor.Position.GetX(),
                   sBodyAnchor.Position.GetY(),
                   sBodyAnchor.Position.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cPiPuckModel.Draw();
      glPopMatrix();
      /* draw the left wheel */
      const SAnchor& sLeftWheelAnchor = c_entity.GetEmbodiedEntity().GetAnchor("left_wheel");
      sLeftWheelAnchor.Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(sLeftWheelAnchor.Position.GetX(),
                   sLeftWheelAnchor.Position.GetY(),
                   sLeftWheelAnchor.Position.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cPiPuckWheelModel.Draw();
      glPopMatrix();
      /* draw the right wheel */
      const SAnchor& sRightWheelAnchor = c_entity.GetEmbodiedEntity().GetAnchor("right_wheel");
      sRightWheelAnchor.Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(sRightWheelAnchor.Position.GetX(),
                   sRightWheelAnchor.Position.GetY(),
                   sRightWheelAnchor.Position.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cPiPuckWheelModel.Draw();
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLPiPuckDebug::CQTOpenGLPiPuckDebug() {
      m_unDisplayList = glGenLists(4);
      /* references to the display lists */
      m_unCylinderList  = m_unDisplayList;
      /* make cylinder list */
      glNewList(m_unCylinderList, GL_COMPILE);
      MakeCylinder();
      glEndList();
      /* generate the items to draw list */
      m_arrItemsToDraw = {
         std::make_tuple("body", m_unCylinderList, m_cBodyScaling),
         std::make_tuple("left_wheel", m_unCylinderList, m_cWheelScaling),
         std::make_tuple("right_wheel", m_unCylinderList, m_cWheelScaling),
      };
   }

   /****************************************/
   /****************************************/

   CQTOpenGLPiPuckDebug::~CQTOpenGLPiPuckDebug() {
      glDeleteLists(m_unDisplayList, 4);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPiPuckDebug::Draw(const CPiPuckEntity& c_entity) {
      /* set materials */
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_arrDefaultSpecular.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, m_arrDefaultShininess.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_arrDefaultEmission.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_arrDefaultColor.data());
      /* draw bodies */
      for(const std::tuple<std::string, GLuint, CVector3>& c_item : m_arrItemsToDraw) {
         const SAnchor& sAnchor = 
            c_entity.GetEmbodiedEntity().GetAnchor(std::get<std::string>(c_item));
         const CVector3& cPosition = sAnchor.Position;
         const CQuaternion& cOrientation = sAnchor.Orientation;
         CRadians cZAngle, cYAngle, cXAngle;
         cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
         glPushMatrix();
         glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
         glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
         glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
         glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
         const CVector3& cScaling = std::get<CVector3>(c_item);
         glScalef(cScaling.GetX(), cScaling.GetY(), cScaling.GetZ());
         glCallList(std::get<GLuint>(c_item));
         glPopMatrix();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPiPuckDebug::MakeCylinder() {
      /* side faces */
      CVector2 cVertex(0.5f, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      glEnable(GL_NORMALIZE);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 1.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Top disk */
      cVertex.Set(0.5f, 0.0f);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 1.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Bottom disk */
      cVertex.Set(0.5f, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawPiPuckNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CPiPuckEntity& c_entity) {
         if(c_entity.IsDebug()) {
            static CQTOpenGLPiPuckDebug m_cDebugModel;
            m_cDebugModel.Draw(c_entity);
         }
         else {
            static CQTOpenGLPiPuck m_cModel;
            m_cModel.Draw(c_entity);
         }
         c_visualization.DrawRays(c_entity.GetControllableEntity());
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawPiPuckSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CPiPuckEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   /****************************************/
   /****************************************/

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawPiPuckNormal, CPiPuckEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawPiPuckSelected, CPiPuckEntity);

   /****************************************/
   /****************************************/

   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrRingLedOffAmbientDiffuse  {0.75f, 0.75f, 0.75f, 0.8f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrRingLedOnAmbientDiffuse   {1.00f, 0.00f, 0.00f, 0.8f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrBodyLedOffAmbientDiffuse  {0.75f, 0.75f, 0.75f, 0.7f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrBodyLedOnAmbientDiffuse   {0.27f, 0.83f, 0.27f, 0.7f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrFrontLedOffAmbientDiffuse {0.75f, 0.75f, 0.75f, 0.8f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuck::m_arrFrontLedOnAmbientDiffuse  {1.00f, 0.00f, 0.00f, 0.8f};

   /****************************************/
   /****************************************/

   const CVector3 CQTOpenGLPiPuckDebug::m_cBodyScaling                          {0.0724, 0.0724, 0.0472};
   const CVector3 CQTOpenGLPiPuckDebug::m_cWheelScaling                         {0.0425, 0.0425, 0.003};

   const std::array<GLfloat, 4> CQTOpenGLPiPuckDebug::m_arrDefaultColor         {0.5f, 0.5f, 0.5f, 1.0f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuckDebug::m_arrDefaultSpecular      {0.0f, 0.0f, 0.0f, 1.0f};
   const std::array<GLfloat, 4> CQTOpenGLPiPuckDebug::m_arrDefaultEmission      {0.0f, 0.0f, 0.0f, 1.0f};
   const std::array<GLfloat, 1> CQTOpenGLPiPuckDebug::m_arrDefaultShininess     {0.0f};

   /****************************************/
   /****************************************/

}
