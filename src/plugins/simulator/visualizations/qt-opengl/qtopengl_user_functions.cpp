/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Giovanni Pini - <gpini@ulb.ac.be>
 */

#include "qtopengl_user_functions.h"
#include <QPainter>

namespace argos {

   /****************************************/
   /****************************************/

   const GLfloat DEFAULT_SPECULAR[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
   const GLfloat DEFAULT_SHININESS[] = { 100.0f                   };
   const GLfloat DEFAULT_EMISSION[]  = {   0.0f, 0.0f, 0.0f, 1.0f };

   /****************************************/
   /****************************************/

   static void Rototranslate(const CVector3& c_position,
                             const CQuaternion& c_orientation) {
      /* Get Euler angles */
      CRadians cZAngle, cYAngle, cXAngle;
      c_orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* Translate */
      glTranslatef(c_position.GetX(), c_position.GetY(), c_position.GetZ());
      /* Rotate */
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
   }

   /****************************************/
   /****************************************/

   CQTOpenGLUserFunctions::CQTOpenGLUserFunctions() :
      m_vecFunctionHolders(1),
      m_pcQTOpenGLMainWindow(NULL) {
      m_cThunks.Add<CEntity>((TThunk)NULL);
   }

   /****************************************/
   /****************************************/

   CQTOpenGLUserFunctions::~CQTOpenGLUserFunctions() {
      while(!m_vecFunctionHolders.empty()) {
         delete m_vecFunctionHolders.back();
         m_vecFunctionHolders.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::KeyPressed(QKeyEvent* pc_event) {
      m_pcQTOpenGLMainWindow->GetOpenGLWidget().KeyPressed(pc_event);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::KeyReleased(QKeyEvent* pc_event) {
      m_pcQTOpenGLMainWindow->GetOpenGLWidget().KeyReleased(pc_event);
   }
   
   /****************************************/
   /****************************************/

   CEntity* CQTOpenGLUserFunctions::GetSelectedEntity() {
      return m_pcQTOpenGLMainWindow->GetOpenGLWidget().GetSelectedEntity();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::SelectEntity(CEntity& c_entity) {
      m_pcQTOpenGLMainWindow->GetOpenGLWidget().SelectEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DeselectEntity() {
      m_pcQTOpenGLMainWindow->GetOpenGLWidget().DeselectEntity();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLMainWindow& CQTOpenGLUserFunctions::GetMainWindow() {
      return *m_pcQTOpenGLMainWindow;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::SetMainWindow(CQTOpenGLMainWindow& c_main_win) {
      m_pcQTOpenGLMainWindow = &c_main_win;
   }

   /****************************************/
   /****************************************/

   CQTOpenGLWidget& CQTOpenGLUserFunctions::GetQTOpenGLWidget() {
      return m_pcQTOpenGLMainWindow->GetOpenGLWidget();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::SetColor(const CColor& c_color) {
      const GLfloat pfColor[]     = {
         c_color.GetRed()   / 255.0f,
         c_color.GetGreen() / 255.0f,
         c_color.GetBlue()  / 255.0f,
         1.0f
      };
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, DEFAULT_SPECULAR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, DEFAULT_SHININESS);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, DEFAULT_EMISSION);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawPoint(const CVector3& c_position,
                                          const CColor& c_color,
                                          const Real f_diameter) {
      /* Save attributes */
      glPushAttrib(GL_POINT_BIT);
      /* Set color */
      SetColor(c_color);
      /* Set point attributes */
      glPointSize(f_diameter);
      glEnable(GL_POINT_SMOOTH);
      /* Draw */
      glBegin(GL_POINTS);
      glVertex3f(c_position.GetX(), c_position.GetY(), c_position.GetZ());
      glEnd();
      /* Restore saved attributes */
      glPopAttrib();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawTriangle(const CVector3& c_position,
                                             const CQuaternion& c_orientation,
                                             Real f_base,
                                             Real f_height,
                                             const CColor& c_color,
                                             const bool b_fill) {
      /* Save attributes and current matrix */
      glPushAttrib(GL_POLYGON_BIT);
      glPushMatrix();
      /* Set color */
      SetColor(c_color);
      /* Disable face culling, to make the triangle visible from any angle */
	    glDisable(GL_CULL_FACE);
      /* Set polygon attributes */
	    glEnable(GL_POLYGON_SMOOTH);
      glPolygonMode(GL_FRONT_AND_BACK, b_fill ? GL_FILL : GL_LINE);
      /* Set position/orientation */
      Rototranslate(c_position, c_orientation);
      /*  Draw */
      glBegin(GL_TRIANGLES);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-f_base * 0.5f,     0.0f, 0.0f);
      glVertex3f( f_base * 0.5f,     0.0f, 0.0f);
      glVertex3f(          0.0f, f_height, 0.0f);
      glEnd();
      /* Restore saved stuff */
      glPopMatrix();
      glPopAttrib();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawPolygon(const CVector3& c_position,
                                            const CQuaternion& c_orientation,
                                            const std::vector<CVector2>& vec_points,
                                            const CColor& c_color,
                                            const bool b_fill) {
      if(vec_points.size() < 2) {
         LOGERR << "CQTOpenGLUserFunctions::DrawPolygon() needs at least 3 points." << std::endl;
         return;
      }
      /* Save attributes and current matrix */
      glPushAttrib(GL_POLYGON_BIT);
      /* Set color */
      SetColor(c_color);
      /* Disable face culling, to make the triangle visible from any angle */
	    glDisable(GL_CULL_FACE);
      /* Set polygon attributes */
	    glEnable(GL_POLYGON_SMOOTH);
      glPolygonMode(GL_FRONT_AND_BACK, b_fill ? GL_FILL : GL_LINE);
      /* Set position/orientation */
      Rototranslate(c_position, c_orientation);
      /*  Draw */
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(size_t i = 0; i < vec_points.size(); ++i) {
         glVertex3f(vec_points[i].GetX(), vec_points[i].GetY(), 0.0f);
      }
      glEnd();
      /* Reset translation */
      glTranslatef(-c_position.GetX(), -c_position.GetY(), -c_position.GetZ());
      /* Restore saved stuff */
      glPopAttrib();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawCircle(const CVector3& c_position,
                                           const CQuaternion& c_orientation,
                                           Real f_radius,
                                           const CColor& c_color,
                                           const bool b_fill,
                                           GLuint un_vertices) {
      /* Save attributes and current matrix */
      glPushAttrib(GL_POLYGON_BIT);
      glPushMatrix();
      /* Set color */
      SetColor(c_color);
      /* Disable face culling, to make the triangle visible from any angle */
	    glDisable(GL_CULL_FACE);
      /* Set polygon attributes */
	    glEnable(GL_POLYGON_SMOOTH);
      glPolygonMode(GL_FRONT_AND_BACK, b_fill ? GL_FILL : GL_LINE);
      /* Set position/orientation */
      Rototranslate(c_position, c_orientation);
      /*  Draw */
	    CVector2 cVertex(f_radius, 0.0f);
	    CRadians cAngle(CRadians::TWO_PI / un_vertices);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(size_t i = 0; i < un_vertices; ++i) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Restore saved stuff */
      glPopAttrib();
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawCylinder(const CVector3& c_position,
                                             const CQuaternion& c_orientation,
                                             Real f_radius,
                                             Real f_height,
                                             const CColor& c_color,
                                             GLuint un_vertices) {
      /* Save current matrix */
      glPushMatrix();
      /* Set color */
      SetColor(c_color);
      /* Set position/orientation */
      Rototranslate(c_position, c_orientation);
      /* Draw side surface */
      Real fHalfHeight = f_height * 0.5f;
      CVector2 cVertex(1.0f, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / un_vertices);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= un_vertices; i++) {
         glNormal3f(cVertex.GetX(), cVertex.GetY(), 0.0f);
         glVertex3f(cVertex.GetX() * f_radius, cVertex.GetY() * f_radius,  fHalfHeight);
         glVertex3f(cVertex.GetX() * f_radius, cVertex.GetY() * f_radius, -fHalfHeight);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Draw top disk */
      cVertex.Set(f_radius, 0.0f);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= un_vertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), fHalfHeight);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Draw bottom disk */
      cVertex.Set(f_radius, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= un_vertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), -fHalfHeight);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Restore saved matrix */
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawBox(const CVector3& c_position,
                                        const CQuaternion& c_orientation,
                                        const CVector3& c_size,
                                        const CColor& c_color) {
      /* Save current matrix */
      glPushMatrix();
      /* Set color */
      SetColor(c_color);
      /* Set position/orientation */
      Rototranslate(c_position, c_orientation);
      /* Draw top and bottom faces (parallel to XY) */
      CVector3 cHalfSize = c_size * 0.5f;
      glBegin(GL_QUADS);
      /* Bottom face */
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f( cHalfSize.GetX(),  cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(), -cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(), -cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(),  cHalfSize.GetY(), -cHalfSize.GetZ());
      /* Top face */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-cHalfSize.GetX(), -cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(), -cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(),  cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(),  cHalfSize.GetY(),  cHalfSize.GetZ());
      glEnd();
      /* Draw the other faces (South, East, North, West) */
      glBegin(GL_QUADS);
      /* North face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f( cHalfSize.GetX(), -cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(),  cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(),  cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(), -cHalfSize.GetY(),  cHalfSize.GetZ());
      /* South face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(-cHalfSize.GetX(), -cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(), -cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(),  cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(),  cHalfSize.GetY(), -cHalfSize.GetZ());
      /* East face */
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f(-cHalfSize.GetX(), -cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(), -cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(), -cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(), -cHalfSize.GetY(),  cHalfSize.GetZ());
      /* West face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(-cHalfSize.GetX(),  cHalfSize.GetY(), -cHalfSize.GetZ());
      glVertex3f(-cHalfSize.GetX(),  cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(),  cHalfSize.GetY(),  cHalfSize.GetZ());
      glVertex3f( cHalfSize.GetX(),  cHalfSize.GetY(), -cHalfSize.GetZ());
      glEnd();
      /* Restore saved matrix */
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawRay(const CRay3& c_ray,
                                        const CColor& c_color,
                                        Real f_width) {
      /* Save attributes and current matrix */
      glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT);
      /* Set line attributes */
      glEnable(GL_LINE_SMOOTH);
      glLineWidth(f_width);
      /* Unset lighting */
      glDisable(GL_LIGHTING);
      glDisable(GL_COLOR_MATERIAL);
      /* Set color */
      glColor3ub(c_color.GetRed(), c_color.GetGreen(), c_color.GetBlue());
      /* Draw ray */
      glBegin(GL_LINES);
      glVertex3f(c_ray.GetStart().GetX(), c_ray.GetStart().GetY(), c_ray.GetStart().GetZ());
      glVertex3f(c_ray.GetEnd().GetX(), c_ray.GetEnd().GetY(), c_ray.GetEnd().GetZ());
      glEnd();
      /* Restore saved stuff */
      glPopAttrib();
   }

   /****************************************/
   /****************************************/

   static void TransformPoint(Real* pf_vec_out,
                              const Real* pf_trans,
                              const Real* pf_vec_in) {
#define M(row,col) pf_trans[col * 4 + row]
      pf_vec_out[0] =
         M(0, 0) * pf_vec_in[0] +
         M(0, 1) * pf_vec_in[1] +
         M(0, 2) * pf_vec_in[2] +
         M(0, 3) * pf_vec_in[3];
      pf_vec_out[1] =
         M(1, 0) * pf_vec_in[0] +
         M(1, 1) * pf_vec_in[1] +
         M(1, 2) * pf_vec_in[2] +
         M(1, 3) * pf_vec_in[3];
      pf_vec_out[2] =
         M(2, 0) * pf_vec_in[0] +
         M(2, 1) * pf_vec_in[1] +
         M(2, 2) * pf_vec_in[2] +
         M(2, 3) * pf_vec_in[3];
      pf_vec_out[3] =
         M(3, 0) * pf_vec_in[0] +
         M(3, 1) * pf_vec_in[1] +
         M(3, 2) * pf_vec_in[2] +
         M(3, 3) * pf_vec_in[3];
#undef M
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawText(const CVector3& c_position,
                                         const std::string& str_text,
                                         const CColor& c_color,
                                         const QFont& c_font) {
      /* Get current modelview matrix */
      GLdouble pf_mv[16];
      glGetDoublev(GL_MODELVIEW_MATRIX, pf_mv);
      /* Get current projection matrix */
      GLdouble pf_proj[16];
      glGetDoublev(GL_PROJECTION_MATRIX, pf_proj);
      /* Get current viewport */
      GLint pn_vp[4];
      glGetIntegerv(GL_VIEWPORT, pn_vp);
      /* Buffers for projection calculations */
      GLdouble pf_v1[4], pf_v2[4];
      /* Transform original position with modelview matrix  */
      pf_v1[0] = c_position.GetX();
      pf_v1[1] = c_position.GetY();
      pf_v1[2] = c_position.GetZ();
      pf_v1[3] = 1.0;
      TransformPoint(pf_v2, pf_mv, pf_v1);
      /* Transform pf_v2 with projection matrix */
      TransformPoint(pf_v1, pf_proj, pf_v2);
      /* Failed projection? Get out */
      if (pf_v1[3] == 0.0) return;
      /* Rescale projection */
      pf_v1[0] /= pf_v1[3];
      pf_v1[1] /= pf_v1[3];
      /* Get screen position (before mirroring) */
      pf_v2[0] =
         (pn_vp[0] + (1. + pf_v1[0]) * pn_vp[2] / 2.) /
         GetMainWindow().devicePixelRatio();
      pf_v2[1] =
         (pn_vp[1] + (1. + pf_v1[1]) * pn_vp[3] / 2.) /
         GetMainWindow().devicePixelRatio();
      /* Mirror along Y because screen goes up to down */
      pf_v2[1] = GetMainWindow().GetOpenGLWidget().height() - pf_v2[1];
      /* Save OpenGL attributes */
      glPushAttrib(GL_ENABLE_BIT);
      /* Disable lighting to make text color unaffected by light */
      glDisable(GL_LIGHTING);
      glDisable(GL_COLOR_MATERIAL);
      /* Disable culling to make text visibile from any angle */
      glDisable(GL_CULL_FACE);
      /* Initialize Qt screen painter */
      QPainter cPainter(&GetMainWindow().GetOpenGLWidget());
      /* Set antialiasing */
      cPainter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
      /* Set pen color */
      cPainter.setPen(
         QColor(c_color.GetRed(),
                c_color.GetGreen(),
                c_color.GetBlue(),
                c_color.GetAlpha()));
      /* Set font */
      cPainter.setFont(c_font);
      /* Render text */
      cPainter.drawText(pf_v2[0], pf_v2[1], str_text.c_str());
      /* Dispose of Qt screen painter */
      cPainter.end();
      /* Restore saved attributes */
      glPopAttrib();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::Call(CEntity& c_entity) {
      TThunk t_thunk = m_cThunks[c_entity.GetTag()];
      if(t_thunk) (this->*t_thunk)(c_entity);
   }

   /****************************************/
   /****************************************/

}
