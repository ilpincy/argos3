/**
 * @file <argos3/plugins/robots/prototype/simulator/qtopengl_prototype.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu - <zhuweixu_harry@126.com>
 */

#include "qtopengl_prototype.h"
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_link_equipped_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/magnet_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   const GLfloat BODY_COLOR[]           = { 0.5f, 0.5f, 0.5f, 1.0f };
   const GLfloat SPECULAR[]             = { 0.0f, 0.0f, 0.0f, 1.0f };
   const GLfloat SHININESS[]            = { 0.0f                   };
   const GLfloat EMISSION[]             = { 0.0f, 0.0f, 0.0f, 1.0f };
   static const Real LED_RADIUS         = 0.0025f;
#ifndef NDEBUG
   static const Real FIELD_SCALE_FACTOR = 0.0005f;
#endif

   /****************************************/
   /****************************************/

   CQTOpenGLPrototype::CQTOpenGLPrototype() :
      m_unVertices(20) {
      /* Reserve the needed display lists */
      m_unBaseList = glGenLists(6);
      /* References to the display lists */
      m_unBoxList      = m_unBaseList;
      m_unCylinderList = m_unBaseList + 1;
      m_unSphereList   = m_unBaseList + 2;
      m_unLEDList      = m_unBaseList + 3;
      m_unPoleList     = m_unBaseList + 4;
      m_unTagList      = m_unBaseList + 5;
      /* Make box list */
      glNewList(m_unBoxList, GL_COMPILE);
      MakeBox();
      glEndList();
      /* Make cylinder list */
      glNewList(m_unCylinderList, GL_COMPILE);
      MakeCylinder();
      glEndList();
      /* Make sphere list */
      glNewList(m_unSphereList, GL_COMPILE);
      MakeSphere();
      glEndList();
      /* Make LED list */
      glNewList(m_unLEDList, GL_COMPILE);
      MakeLED();
      glEndList();
      /* Make Poles list */
      glNewList(m_unPoleList, GL_COMPILE);
      MakePoles();
      glEndList();
      /* Make Tag list */
      MakeTagTexture();
      glNewList(m_unTagList, GL_COMPILE);
      MakeTag();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLPrototype::~CQTOpenGLPrototype() {
      glDeleteLists(m_unBaseList, 6);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPrototype::MakeLED() {
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

   void CQTOpenGLPrototype::DrawEntity(CPrototypeEntity& c_entity) {
      /* Draw the links */
      for(CPrototypeLinkEntity* pcLink : c_entity.GetLinkEquippedEntity().GetLinks()){
         /* Configure the body material */
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BODY_COLOR);
         /* Get the position of the link */
         const CVector3& cPosition = pcLink->GetAnchor().Position;
         /* Get the orientation of the link */
         const CQuaternion& cOrientation = pcLink->GetAnchor().Orientation;
         CRadians cZAngle, cYAngle, cXAngle;
         cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
         glPushMatrix();
         glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
         glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
         glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
         glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
         /* Draw the link */
         switch(pcLink->GetGeometry()) {
         case CPrototypeLinkEntity::EGeometry::BOX:
            glScalef(pcLink->GetExtents().GetX(),
                     pcLink->GetExtents().GetY(),
                     pcLink->GetExtents().GetZ());
            glCallList(m_unBoxList);
            break;
         case CPrototypeLinkEntity::EGeometry::CYLINDER:
            glScalef(pcLink->GetExtents().GetX(),
                     pcLink->GetExtents().GetY(),
                     pcLink->GetExtents().GetZ());
            glCallList(m_unCylinderList);
            break;
         case CPrototypeLinkEntity::EGeometry::SPHERE:
            glScalef(pcLink->GetExtents().GetX(),
                     pcLink->GetExtents().GetY(),
                     pcLink->GetExtents().GetZ());
            glCallList(m_unSphereList);
            break;
         case CPrototypeLinkEntity::EGeometry::CONVEX_HULL:
            DrawConvexHull(pcLink->GetConvexHullPoints(),
                           pcLink->GetConvexHullFaces());
            break;
         }
         glPopMatrix();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPrototype::DrawDevices(CPrototypeEntity& c_entity) {
      /* Draw the directional LEDs */
      if(c_entity.HasDirectionalLEDEquippedEntity()) {
         GLfloat pfColor[]           = {   0.0f, 0.0f, 0.0f, 1.0f };
         const GLfloat pfSpecular[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
         const GLfloat pfShininess[] = { 100.0f                   };
         const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
         glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
         const CDirectionalLEDEquippedEntity& cDirectionalLEDEquippedEntity =
            c_entity.GetDirectionalLEDEquippedEntity();
         for(const CDirectionalLEDEquippedEntity::SInstance& s_instance :
             cDirectionalLEDEquippedEntity.GetInstances()) {
            glPushMatrix();
            /* Set the material */
            const CColor& cColor = s_instance.LED.GetColor();
            pfColor[0] = cColor.GetRed()   / 255.0f;
            pfColor[1] = cColor.GetGreen() / 255.0f;
            pfColor[2] = cColor.GetBlue()  / 255.0f;
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
            /* Perform rototranslation */
            const CVector3& cPosition = s_instance.LED.GetPosition();
            glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
            /* Draw the LED */
            glCallList(m_unLEDList);
            glPopMatrix();
         }
      }
      /* Draw the LEDs */
      if(c_entity.HasLEDEquippedEntity()) {
         GLfloat pfColor[]           = {   0.0f, 0.0f, 0.0f, 1.0f };
         const GLfloat pfSpecular[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
         const GLfloat pfShininess[] = { 100.0f                   };
         const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
         glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
         for(const CLEDEquippedEntity::SActuator* ps_actuator :
             c_entity.GetLEDEquippedEntity().GetLEDs()) {
            glPushMatrix();
            /* Set the material */
            const CColor& cColor = ps_actuator->LED.GetColor();
            pfColor[0] = cColor.GetRed()   / 255.0f;
            pfColor[1] = cColor.GetGreen() / 255.0f;
            pfColor[2] = cColor.GetBlue()  / 255.0f;
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
            /* Perform rototranslation */
            const CVector3& cPosition = ps_actuator->LED.GetPosition();
            glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
            /* Draw the LED */
            glCallList(m_unLEDList);
            glPopMatrix();
         }
      }
      /* Draw the tags */
      if(c_entity.HasTagEquippedEntity()) {
         const CTagEquippedEntity& cTagEquippedEntity =
            c_entity.GetTagEquippedEntity();
         CRadians cZ, cY, cX;
         for(const CTagEquippedEntity::SInstance& s_instance :
             cTagEquippedEntity.GetInstances()) {
            const CVector3& cTagPosition = s_instance.Tag.GetPosition();
            const CQuaternion& cTagOrientation = s_instance.Tag.GetOrientation();
            cTagOrientation.ToEulerAngles(cZ, cY, cX);
            Real fScaling = s_instance.Tag.GetSideLength();
            glPushMatrix();
            glTranslatef(cTagPosition.GetX(),
                         cTagPosition.GetY(),
                         cTagPosition.GetZ());
            glRotatef(ToDegrees(cX).GetValue(), 1.0f, 0.0f, 0.0f);
            glRotatef(ToDegrees(cY).GetValue(), 0.0f, 1.0f, 0.0f);
            glRotatef(ToDegrees(cZ).GetValue(), 0.0f, 0.0f, 1.0f);
            glScalef(fScaling, fScaling, 1.0f);
            glCallList(m_unTagList);
            glPopMatrix();
         }
      }
#ifndef NDEBUG
      /* Draw the magnetic poles */
      if(c_entity.HasMagnetEquippedEntity()) {
         CMagnetEquippedEntity& cMagnetEquippedEntity =
            c_entity.GetMagnetEquippedEntity();
         CVector3 cFieldOrigin;
         CQuaternion cFieldOrientation;
         CRadians cFieldOrientationZ, cFieldOrientationY, cFieldOrientationX;
         for(CMagnetEquippedEntity::SInstance& s_instance :
             cMagnetEquippedEntity.GetInstances()) {
            cFieldOrigin = s_instance.Offset;
            cFieldOrigin.Rotate(s_instance.Anchor.Orientation);
            cFieldOrigin += s_instance.Anchor.Position;
            const CVector3& cField = s_instance.Magnet.GetField();
            cFieldOrientation = s_instance.Anchor.Orientation *
               CQuaternion(CVector3::Z, cField);
            cFieldOrientation.ToEulerAngles(cFieldOrientationZ,
                                            cFieldOrientationY,
                                            cFieldOrientationX);
            glPushMatrix();
            glTranslatef(cFieldOrigin.GetX(),
                         cFieldOrigin.GetY(),
                         cFieldOrigin.GetZ());
            glRotatef(ToDegrees(cFieldOrientationX).GetValue(), 1.0f, 0.0f, 0.0f);
            glRotatef(ToDegrees(cFieldOrientationY).GetValue(), 0.0f, 1.0f, 0.0f);
            glRotatef(ToDegrees(cFieldOrientationZ).GetValue(), 0.0f, 0.0f, 1.0f);
            glScalef(1.0f, 1.0f, cField.Length() * FIELD_SCALE_FACTOR);
            glCallList(m_unPoleList);
            glPopMatrix();
         }
      }
#endif
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPrototype::MakeBox() {
      glEnable(GL_NORMALIZE);
      /* Set the material */
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EMISSION);
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

   void CQTOpenGLPrototype::MakeCylinder() {
      glEnable(GL_NORMALIZE);
      /* Set the material */
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EMISSION);
      /* Side surface */
      CVector2 cVertex(0.5f, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
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

   void CQTOpenGLPrototype::MakeSphere() {
      glEnable(GL_NORMALIZE);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EMISSION);
      CVector3 cNormal, cPoint;
      CRadians cSlice(CRadians::TWO_PI / m_unVertices);
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

   void CQTOpenGLPrototype::DrawConvexHull(const std::vector<CVector3>& vec_points,
                                           const std::vector<CConvexHull::SFace>& vec_faces) {
      glEnable(GL_NORMALIZE);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EMISSION);
      glBegin(GL_TRIANGLES);
      for(const CConvexHull::SFace& s_face : vec_faces) {
         const CVector3& cVertex1 = vec_points[s_face.VertexIndices[0]];
         const CVector3& cVertex2 = vec_points[s_face.VertexIndices[1]];
         const CVector3& cVertex3 = vec_points[s_face.VertexIndices[2]];
         CVector3 cNormal(cVertex2 - cVertex1);
         cNormal.CrossProduct(cVertex3 - cVertex1);
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex1.GetX(), cVertex1.GetY(), cVertex1.GetZ());
         glVertex3f(cVertex2.GetX(), cVertex2.GetY(), cVertex2.GetZ());
         glVertex3f(cVertex3.GetX(), cVertex3.GetY(), cVertex3.GetZ());
      }
      glEnd();
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPrototype::MakeTagTexture() {
      glGenTextures(1, &m_unTagTex);
      glBindTexture(GL_TEXTURE_2D, m_unTagTex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, m_fTagTexture);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPrototype::MakeTag() {
      glEnable(GL_NORMALIZE);
      glDisable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, m_unTagTex);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.5f,  0.5f);
      glTexCoord2f(0.03f, 1.0f); glVertex2f(-0.5f,  0.5f);
      glTexCoord2f(0.03f, 0.03f); glVertex2f(-0.5f, -0.5f);
      glTexCoord2f(1.0f, 0.03f); glVertex2f( 0.5f, -0.5f);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glEnable(GL_LIGHTING);
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLPrototype::MakePoles() {
      glEnable(GL_NORMALIZE);
      glDisable(GL_LIGHTING);
      glLineWidth(4.0f);
      glBegin(GL_LINES);
      /* south pole */
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.5f);
      /* north pole */
      glColor3f(0.0, 0.0, 1.0);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, -0.5f);
      glEnd();
      glLineWidth(1.0f);
      glEnable(GL_LIGHTING);
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawPrototypeNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CPrototypeEntity& c_entity) {
         static CQTOpenGLPrototype m_cModel;
         if(c_entity.HasControllableEntity()) {
            c_visualization.DrawRays(c_entity.GetControllableEntity());
         }
         m_cModel.DrawEntity(c_entity);
         m_cModel.DrawDevices(c_entity);
      }
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawPrototypeSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CPrototypeEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLWidget, CQTOpenGLOperationDrawPrototypeNormal, void, CPrototypeEntity);

   REGISTER_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLWidget, CQTOpenGLOperationDrawPrototypeSelected, void, CPrototypeEntity);

   /****************************************/
   /****************************************/

}
