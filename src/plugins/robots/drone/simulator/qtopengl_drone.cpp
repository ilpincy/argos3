/**
 * @file <argos3/plugins/robots/drone/simulator/qtopengl_drone.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_drone.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/drone/simulator/drone_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>

#include <array>

namespace argos {

   /****************************************/
   /****************************************/

    CQTOpenGLDrone::CQTOpenGLDrone() :
      /* create the model */
      m_cDroneModel("drone.obj"),
      m_cPropellerModel("e305_propeller.obj"),
      /* get pointers to the LED materials */
      m_arrLEDs {
         &m_cDroneModel.GetMaterial("led_0"),
         &m_cDroneModel.GetMaterial("led_1"),
         &m_cDroneModel.GetMaterial("led_2"),
         &m_cDroneModel.GetMaterial("led_3")        
      } {}

   /****************************************/
   /****************************************/

   CQTOpenGLDrone::~CQTOpenGLDrone() {}

   /****************************************/
   /****************************************/

   void CQTOpenGLDrone::Draw(const CDroneEntity& c_entity) {
      /* Update LED materials */
      const CDirectionalLEDEquippedEntity& c_leds = c_entity.GetDirectionalLEDEquippedEntity();
      for(UInt32 un_material_idx = 0;
          un_material_idx < m_arrLEDs.size();
          un_material_idx++) {
         const CColor& cColor = c_leds.GetLED(un_material_idx).GetColor();
         std::array<GLfloat, 4> arrColor = {
            cColor.GetRed() / 255.0f,
            cColor.GetGreen() / 255.0f,
            cColor.GetBlue() / 255.0f,
            1.0f
         };
         m_arrLEDs[un_material_idx]->Emission = arrColor;
      }
      /* buffers for converting from quaternions to euler angles */
      CRadians cZAngle, cYAngle, cXAngle;
      /* draw the robot */
      const CVector3& cPosition = c_entity.GetEmbodiedEntity().GetOriginAnchor().Position;
      const CQuaternion& cOrientation = c_entity.GetEmbodiedEntity().GetOriginAnchor().Orientation;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cDroneModel.Draw();
      /* start of draw propellers */
      UInt32 unPropellerAngle = 
         (CSimulator::GetInstance().GetSpace().GetSimulationClock() * 10) % 360;
      /* (1, 1) */
      glPushMatrix();
      glTranslatef(m_cPropellerOffset.GetX(), m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, -1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* (-1, 1) */
      glPushMatrix();
      glTranslatef(-m_cPropellerOffset.GetX(), m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, 1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* (-1, -1) */
      glPushMatrix();
      glTranslatef(-m_cPropellerOffset.GetX(), -m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, -1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* (1, -1) */
      glPushMatrix();
      glTranslatef(m_cPropellerOffset.GetX(), -m_cPropellerOffset.GetY(), m_cPropellerOffset.GetZ());
      glRotatef(unPropellerAngle, 0.0f, 0.0f, 1.0f);
      m_cPropellerModel.Draw();
      glPopMatrix();
      /* end of draw propellers */
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLDroneDebug::CQTOpenGLDroneDebug() {
      m_unDisplayList = glGenLists(1);
      /* references to the display lists */
      m_unCylinderList = m_unDisplayList;
      /* make cylinder list */
      glNewList(m_unCylinderList, GL_COMPILE);
      MakeCylinder();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLDroneDebug::~CQTOpenGLDroneDebug() {
      glDeleteLists(m_unDisplayList, 1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLDroneDebug::Draw(const CDroneEntity& c_entity) {
      /* set materials */
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_arrDefaultSpecular.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, m_arrDefaultShininess.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_arrDefaultEmission.data());
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_arrDefaultColor.data());
      /* debug drawing code here */
      const SAnchor& sAnchor = c_entity.GetEmbodiedEntity().GetOriginAnchor();
      const CVector3& cPosition = sAnchor.Position;
      const CQuaternion& cOrientation = sAnchor.Orientation;
      CRadians cZAngle, cYAngle, cXAngle;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      glScalef(m_cExtents.GetX(), m_cExtents.GetY(), m_cExtents.GetZ());
      glCallList(m_unCylinderList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLDroneDebug::MakeCylinder() {
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

   class CQTOpenGLOperationDrawDroneNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CDroneEntity& c_entity) {
         if(c_entity.IsDebug()) {
            static CQTOpenGLDroneDebug m_cDebugModel;
            m_cDebugModel.Draw(c_entity);
         }
         else {
            static CQTOpenGLDrone m_cModel;
            m_cModel.Draw(c_entity);
         }
         c_visualization.DrawRays(c_entity.GetControllableEntity());
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawDroneSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CDroneEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   /****************************************/
   /****************************************/

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawDroneNormal, CDroneEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawDroneSelected, CDroneEntity);

   /****************************************/
   /****************************************/

   const std::array<GLfloat, 4> CQTOpenGLDroneDebug::m_arrDefaultColor      {0.5, 0.5, 0.5, 1.0};
   const std::array<GLfloat, 4> CQTOpenGLDroneDebug::m_arrDefaultSpecular   {0.0, 0.0, 0.0, 1.0};
   const std::array<GLfloat, 4> CQTOpenGLDroneDebug::m_arrDefaultEmission   {0.0, 0.0, 0.0, 1.0};
   const std::array<GLfloat, 1> CQTOpenGLDroneDebug::m_arrDefaultShininess  {0.0};

   const CVector3 CQTOpenGLDroneDebug::m_cExtents {0.7, 0.7, 0.2};
   const CVector3 CQTOpenGLDrone::m_cPropellerOffset {0.159, 0.159, 0.208};


   /****************************************/
   /****************************************/

}
