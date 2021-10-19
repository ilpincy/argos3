/**
 * @file <argos3/plugins/robots/builderbot/simulator/qtopengl_builderbot.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "qtopengl_builderbot.h"

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/convex_hull.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

#include <array>

#define TAG_WHITE std::array<GLfloat, 3> {1.0f, 1.0f, 1.0f}
#define TAG_BLACK std::array<GLfloat, 3> {0.0f, 0.0f, 0.0f}

namespace argos {

   /****************************************/
   /****************************************/

    CQTOpenGLBuilderBot::CQTOpenGLBuilderBot() :
      /* create the model */
      m_cBuilderBotModel("builderbot.obj"),
      m_cBuilderBotManipulatorModel("builderbot-manipulator.obj"),
      m_arrTagTexture {
         TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_BLACK, TAG_WHITE,
         TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE, TAG_WHITE,
      },
      /* get pointers to the LED materials */
      m_arrLEDs {
         &m_cBuilderBotModel.GetMaterial("led_0"),
         &m_cBuilderBotModel.GetMaterial("led_1"),
         &m_cBuilderBotModel.GetMaterial("led_2"),
         &m_cBuilderBotModel.GetMaterial("led_3"),
         &m_cBuilderBotModel.GetMaterial("led_4"),
         &m_cBuilderBotModel.GetMaterial("led_5"),
         &m_cBuilderBotModel.GetMaterial("led_6"),
         &m_cBuilderBotModel.GetMaterial("led_7"),
         &m_cBuilderBotModel.GetMaterial("led_8"),
         &m_cBuilderBotModel.GetMaterial("led_9"),
         &m_cBuilderBotModel.GetMaterial("led_10"),
         &m_cBuilderBotModel.GetMaterial("led_11"),
      } {
      /* generate the tag texture */
      GLuint unTagTex;
      glGenTextures(1, &unTagTex);
      glBindTexture(GL_TEXTURE_2D, unTagTex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 10, 10, 0, GL_RGB, GL_FLOAT, m_arrTagTexture.data());
      /* draw normalized tag into list */
      m_unTagList = glGenLists(1);
      glNewList(m_unTagList, GL_COMPILE);
      glEnable(GL_NORMALIZE);
      glDisable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, unTagTex);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.5f,  0.5f);
      glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f,  0.5f);
      glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
      glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.5f, -0.5f);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glEnable(GL_LIGHTING);
      glDisable(GL_NORMALIZE);
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLBuilderBot::~CQTOpenGLBuilderBot() {}

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBot::Draw(const CBuilderBotEntity& c_entity) {
      /* update LED materials */
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
      /* buffers for converting to euler angles */
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
      m_cBuilderBotModel.Draw();
      /* draw tags */
      for(const CTagEquippedEntity::SInstance& s_instance :
          c_entity.GetTagEquippedEntity().GetInstances()) {
         /* the texture of the tag contains the white border which isn't actually part
            of the tag. Stretching the texture by 25% fixes this problem */
         Real fScaling = s_instance.Tag.GetSideLength() * 1.25f;
         const CVector3& cTagPosition = s_instance.PositionOffset;
         const CQuaternion& cTagOrientation = s_instance.OrientationOffset;
         cTagOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
         glPushMatrix();
         glTranslatef(cTagPosition.GetX(),
                      cTagPosition.GetY(),
                      cTagPosition.GetZ());
         glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
         glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
         glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
         glScalef(fScaling, fScaling, 1.0f);
         glCallList(m_unTagList);
         glPopMatrix();
      }
      glPopMatrix();
      /* draw the manipulator */
      const CVector3& cManipulatorPosition = c_entity.GetLiftSystemEntity().GetAnchor().Position;
      const CQuaternion& cManipulatorOrientation = c_entity.GetLiftSystemEntity().GetAnchor().Orientation;
      cManipulatorOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      glPushMatrix();
      glTranslatef(cManipulatorPosition.GetX(), cManipulatorPosition.GetY(), cManipulatorPosition.GetZ());
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
      m_cBuilderBotManipulatorModel.Draw();
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLBuilderBotDebug::CQTOpenGLBuilderBotDebug() {
      m_unDisplayList = glGenLists(4);
      /* references to the display lists */
      m_unBoxList       = m_unDisplayList;
      m_unCylinderList  = m_unDisplayList + 1;
      m_unSphereList    = m_unDisplayList + 2;
      m_unUpperBaseList = m_unDisplayList + 3;
      /* make box list */
      glNewList(m_unBoxList, GL_COMPILE);
      MakeBox();
      glEndList();
      /* make cylinder list */
      glNewList(m_unCylinderList, GL_COMPILE);
      MakeCylinder();
      glEndList();
      /* make sphere list */
      glNewList(m_unSphereList, GL_COMPILE);
      MakeSphere();
      glEndList();
      /* make upper base list */
      glNewList(m_unUpperBaseList, GL_COMPILE);
      MakeUpperBase();
      glEndList();
      /* generate the items to draw list */
      m_arrItemsToDraw = {
         std::make_tuple("lower_base", m_unBoxList, m_cLowerBaseScaling),
         std::make_tuple("left_wheel", m_unCylinderList, m_cWheelScaling),
         std::make_tuple("right_wheel", m_unCylinderList, m_cWheelScaling),
         std::make_tuple("rear_pivot", m_unSphereList, m_cPivotScaling),
         std::make_tuple("upper_base", m_unUpperBaseList, m_cUpperBaseScaling),
         std::make_tuple("lift_column", m_unBoxList, m_cLiftColumnScaling),
         std::make_tuple("end_effector", m_unBoxList, m_cEndEffectorScaling),
         std::make_tuple("end_effector_support", m_unBoxList, m_cEndEffectorSupportScaling),
      };
   }

   /****************************************/
   /****************************************/

   CQTOpenGLBuilderBotDebug::~CQTOpenGLBuilderBotDebug() {
      glDeleteLists(m_unDisplayList, 4);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBotDebug::Draw(const CBuilderBotEntity& c_entity) {
      //glDisable(GL_CULL_FACE);
      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
      //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      //glEnable(GL_CULL_FACE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBotDebug::MakeBox() {
      glEnable(GL_NORMALIZE);
      /* This part covers the top and bottom faces (parallel to XY) */
      glBegin(GL_QUADS);
      /* Bottom face */
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f( 0.5f,  0.5f, 0.0f);
      glVertex3f( 0.5f, -0.5f, 0.0f);
      glVertex3f(-0.5f, -0.5f, 0.0f);
      glVertex3f(-0.5f,  0.5f, 0.0f);
      /* Top face */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-0.5f, -0.5f, 1.0f);
      glVertex3f( 0.5f, -0.5f, 1.0f);
      glVertex3f( 0.5f,  0.5f, 1.0f);
      glVertex3f(-0.5f,  0.5f, 1.0f);
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUADS);
      /* South face */
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f(-0.5f, -0.5f, 1.0f);
      glVertex3f(-0.5f, -0.5f, 0.0f);
      glVertex3f( 0.5f, -0.5f, 0.0f);
      glVertex3f( 0.5f, -0.5f, 1.0f);
      /* East face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f( 0.5f, -0.5f, 1.0f);
      glVertex3f( 0.5f, -0.5f, 0.0f);
      glVertex3f( 0.5f,  0.5f, 0.0f);
      glVertex3f( 0.5f,  0.5f, 1.0f);
      /* North face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f( 0.5f,  0.5f, 1.0f);
      glVertex3f( 0.5f,  0.5f, 0.0f);
      glVertex3f(-0.5f,  0.5f, 0.0f);
      glVertex3f(-0.5f,  0.5f, 1.0f);
      /* West face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(-0.5f,  0.5f, 1.0f);
      glVertex3f(-0.5f,  0.5f, 0.0f);
      glVertex3f(-0.5f, -0.5f, 0.0f);
      glVertex3f(-0.5f, -0.5f, 1.0f);
      glEnd();
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBotDebug::MakeUpperBase() {
      glEnable(GL_NORMALIZE);
      std::vector<CVector2> vecPoints = {
         {0.039, 0.065}, {-0.039, 0.065}, {-0.065, 0.039}, {-0.065,-0.039},
         {-0.039,-0.065}, {0.039,-0.065}, {0.065,-0.039}, {0.065, 0.039}, {0.039, 0.065}
      };
      /* top face */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(const CVector2& cPoint : vecPoints) {
         glVertex3f(cPoint.GetX(), cPoint.GetY(), 0.0569f);
      }
      glEnd();
      /* bottom face */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(const CVector2& cPoint : vecPoints) {
         glVertex3f(cPoint.GetX(), cPoint.GetY(), 0.0f);
      }
      glEnd();
      /* side faces */
      glBegin(GL_QUAD_STRIP);
      for(const CVector2& cPoint : vecPoints) {
         glNormal3f(cPoint.GetX(), cPoint.GetY(), 0.0f);
         glVertex3f(cPoint.GetX(), cPoint.GetY(), 0.0569f);
         glVertex3f(cPoint.GetX(), cPoint.GetY(), 0.0f);
      }
      glEnd();
      glDisable(GL_NORMALIZE); 
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBuilderBotDebug::MakeCylinder() {
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

   void CQTOpenGLBuilderBotDebug::MakeSphere() {
      CVector3 cNormal, cPoint;
      CRadians cSlice(CRadians::TWO_PI / m_unVertices);
      glEnable(GL_NORMALIZE);
      glBegin(GL_TRIANGLE_STRIP);
      for(CRadians cInclination; cInclination <= CRadians::PI; cInclination += cSlice) {
         for(CRadians cAzimuth; cAzimuth <= CRadians::TWO_PI; cAzimuth += cSlice) {
            cPoint.FromSphericalCoords(0.5f, cInclination, cAzimuth);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
            cPoint.FromSphericalCoords(0.5f, cInclination + cSlice, cAzimuth);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
            cPoint.FromSphericalCoords(0.5f, cInclination, cAzimuth + cSlice);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
            cPoint.FromSphericalCoords(0.5f, cInclination + cSlice, cAzimuth + cSlice);
            glNormal3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ() + 0.5f);
         }
      }
      glEnd();
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawBuilderBotNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBuilderBotEntity& c_entity) {
         if(c_entity.IsDebug()) {
            static CQTOpenGLBuilderBotDebug m_cDebugModel;
            m_cDebugModel.Draw(c_entity);
         }
         else {
            static CQTOpenGLBuilderBot m_cModel;
            m_cModel.Draw(c_entity);
         }
         c_visualization.DrawRays(c_entity.GetControllableEntity());
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawBuilderBotSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBuilderBotEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   /****************************************/
   /****************************************/

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawBuilderBotNormal, CBuilderBotEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawBuilderBotSelected, CBuilderBotEntity);

   /****************************************/
   /****************************************/

   const CVector3 CQTOpenGLBuilderBotDebug::m_cLowerBaseScaling                  {0.13f, 0.14f, 0.0393566f};
   const CVector3 CQTOpenGLBuilderBotDebug::m_cWheelScaling                      {0.04f, 0.04f, 0.015f};
   const CVector3 CQTOpenGLBuilderBotDebug::m_cPivotScaling                      {0.04f, 0.04f, 0.04f};
   const CVector3 CQTOpenGLBuilderBotDebug::m_cUpperBaseScaling                  {1.0f, 1.0f, 1.0f};
   const CVector3 CQTOpenGLBuilderBotDebug::m_cLiftColumnScaling                 {0.0585f, 0.0945f, 0.28775f};
   const CVector3 CQTOpenGLBuilderBotDebug::m_cEndEffectorScaling                {0.065875f, 0.072f, 0.01f};
   const CVector3 CQTOpenGLBuilderBotDebug::m_cEndEffectorSupportScaling         {0.01025f, 0.0575f, 0.15675f};  
   const std::array<GLfloat, 4> CQTOpenGLBuilderBotDebug::m_arrDefaultColor      {0.5f, 0.5f, 0.5f, 1.0f};
   const std::array<GLfloat, 4> CQTOpenGLBuilderBotDebug::m_arrDefaultSpecular   {0.0f, 0.0f, 0.0f, 1.0f};
   const std::array<GLfloat, 4> CQTOpenGLBuilderBotDebug::m_arrDefaultEmission   {0.0f, 0.0f, 0.0f, 1.0f};
   const std::array<GLfloat, 1> CQTOpenGLBuilderBotDebug::m_arrDefaultShininess  {0.0f};

   /****************************************/
   /****************************************/

}
