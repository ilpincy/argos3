/**
 * @file <argos3/plugins/robots/spiri/simulator/qtopengl_spiri.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_spiri.h"
#include "spiri_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   /* All measures are in meters */
   static const GLfloat SIDE        = 0.470f;
   static const GLfloat DIAGONAL    = SIDE * ::sqrt(2);
   static const GLfloat OUT_RADIUS  = DIAGONAL / 5.0f;
   static const GLfloat IN_RADIUS   = OUT_RADIUS * 0.95f;
   static const GLfloat HEIGHT      = 0.090f;

   static const GLfloat PROP_HEIGHT    = HEIGHT / 2.0f;
   static const GLfloat PROP_ELEVATION = HEIGHT - PROP_HEIGHT;
   static const GLfloat PROP_ARM       = DIAGONAL / 2.0f - OUT_RADIUS;
   
   static const GLfloat LEG_RADIUS = OUT_RADIUS * 0.05f;
   static const GLfloat LEG_HEIGHT = HEIGHT - PROP_HEIGHT;

   static const GLfloat BODY_SIDE      = OUT_RADIUS;
   static const GLfloat BODY_HEIGHT    = HEIGHT * 3.0f / 4.0f;
   static const GLfloat BODY_ELEVATION = HEIGHT - BODY_HEIGHT;

   /****************************************/
   /****************************************/

   CQTOpenGLSpiri::CQTOpenGLSpiri() :
      m_unVertices(40) {
      /* Reserve the needed display lists */
      m_unList = glGenLists(1);
      glNewList(m_unList, GL_COMPILE);
      MakeModel();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLSpiri::~CQTOpenGLSpiri() {
      glDeleteLists(m_unList, 1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::Draw(CSpiriEntity& c_entity) {
      glCallList(m_unList);
      /* Save attributes and current matrix */
      glPushAttrib(GL_LINE_BIT);
      /* Set line attributes */
      glEnable(GL_LINE_SMOOTH);
      glLineWidth(2.0);
      /* Set color */
      SetMainBodyMaterial();
      /* Draw ray */
      glBegin(GL_LINES);
      glVertex3f(0,0,0);
      glVertex3f(DIAGONAL,0,0);
      glEnd();
      /* Restore saved stuff */
      glPopAttrib();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::SetPlasticMaterial() {
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

   void CQTOpenGLSpiri::SetPropellerMaterial() {
      const GLfloat pfColor[]     = {   0.7f, 0.7f, 0.7f, 1.0f };
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

   void CQTOpenGLSpiri::SetMainBodyMaterial() {
      const GLfloat pfColor[]     = {   0.0f, 0.0f, 1.0f, 1.0f };
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

   void CQTOpenGLSpiri::MakeModel() {
      glPushMatrix();
      glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
      /* Main body */
      MakeMainBody();
      /* Front propeller */
      glTranslatef(PROP_ARM, 0.0f, 0.0f);
      MakePropeller();
      /* Left propeller */
      glTranslatef(-PROP_ARM, PROP_ARM, 0.0f);
      MakePropeller();
      /* Back propeller */
      glTranslatef(-PROP_ARM, -PROP_ARM, 0.0f);
      MakePropeller();
      /* Right propeller */
      glTranslatef(PROP_ARM, -PROP_ARM, 0.0f);
      MakePropeller();
      glTranslatef(0.0f, PROP_ARM, 0.0f);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::MakePropeller() {
      glPushMatrix();
      SetPlasticMaterial();
      /*
       * Leg
       */
      MakeCylinder(LEG_RADIUS, LEG_HEIGHT);
      /*
       * Propeller itself
       */
      /* Go above the leg */
      glTranslatef(0.0f, 0.0f, PROP_ELEVATION);
      /* Draw propeller side surfaces */
      MakeCylinderSurface(OUT_RADIUS, PROP_HEIGHT);
      glCullFace(GL_FRONT);
      MakeCylinderSurface(IN_RADIUS, PROP_HEIGHT);
      glCullFace(GL_BACK);
      /* Draw propeller bottom */
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      SetPlasticMaterial();
      MakeRing(OUT_RADIUS, IN_RADIUS);
      /* Draw rotating propeller */
      glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, 0.0f, PROP_HEIGHT / 2.0f);
      SetPropellerMaterial();
      MakeDisk(IN_RADIUS);
      /* Draw propeller top */
      glTranslatef(0.0f, 0.0f, PROP_HEIGHT / 2.0f);
      SetPlasticMaterial();
      MakeRing(OUT_RADIUS, IN_RADIUS);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::MakeMainBody() {
      glPushMatrix();
      SetMainBodyMaterial();
      glTranslatef(0.0f, 0.0f, BODY_ELEVATION);
      MakeBox(BODY_SIDE, BODY_SIDE, BODY_HEIGHT);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::MakeRing(GLfloat f_out_radius,
                                 GLfloat f_in_radius) {
      CVector2 cVertex(1.0f, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      glBegin(GL_QUAD_STRIP);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX() * f_in_radius, cVertex.GetY() * f_in_radius, 0.0f);
         glVertex3f(cVertex.GetX() * f_out_radius, cVertex.GetY() * f_out_radius, 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();      
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::MakeDisk(GLfloat f_radius) {
      CVector2 cVertex(f_radius, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::MakeCylinderSurface(GLfloat f_radius,
                                            GLfloat f_height) {
      CVector2 cVertex(f_radius, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      glEnable(GL_NORMALIZE);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), f_height);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::MakeCylinder(GLfloat f_radius,
                                     GLfloat f_height) {
      glPushMatrix();
      MakeDisk(f_radius);
      MakeCylinderSurface(f_radius, f_height);
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      MakeDisk(f_radius);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLSpiri::MakeBox(GLfloat f_length,
                                GLfloat f_width,
                                GLfloat f_height) {
      glEnable(GL_NORMALIZE);
      glPushMatrix();
      glScalef(f_length, f_width, f_height);
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
      /* The shape definitions is finished */
      glPopMatrix();
      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawSpiriNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CSpiriEntity& c_entity) {
         static CQTOpenGLSpiri m_cModel;
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawSpiriSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CSpiriEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawSpiriNormal, CSpiriEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawSpiriSelected, CSpiriEntity);

   /****************************************/
   /****************************************/

}
