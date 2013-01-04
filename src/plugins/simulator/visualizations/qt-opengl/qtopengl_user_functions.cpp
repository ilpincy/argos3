/**
 * @file <argos3/core/simulator/visualizations/qtopengl_user_functions.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Giovanni Pini - <gpini@ulb.ac.be>
 */

#include "qtopengl_user_functions.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawTriangle(const CVector3& c_center_offset,
                                             const CColor& c_color,
                                             const bool b_fill,
                                             const CQuaternion& c_orientation,
                                             Real f_base,
                                             Real f_height) {

	    glDisable(GL_LIGHTING);
	    glDisable(GL_CULL_FACE);

	    glColor3ub(c_color.GetRed(),
                 c_color.GetGreen(),
                 c_color.GetBlue());

	  if(b_fill) {
          glBegin(GL_POLYGON);
	  }
	  else {
		  glBegin(GL_LINE_LOOP);
	  }
      CVector3 cNormalDirection(0.0f, 0.0f, 1.0f);
      cNormalDirection.Rotate(c_orientation);
      glNormal3f(cNormalDirection.GetX(),
                 cNormalDirection.GetY(),
                 cNormalDirection.GetZ());

      CVector3 cVertex(f_height * 0.5f, 0.0f, 0.0f);
      cVertex.Rotate(c_orientation);
      glVertex3f(cVertex.GetX() + c_center_offset.GetX(),
                 cVertex.GetY() + c_center_offset.GetY(),
                 cVertex.GetZ() + c_center_offset.GetZ());

      cVertex.Set(-f_height * 0.5f, f_base * 0.5f, 0.0f);
      cVertex.Rotate(c_orientation);
      glVertex3f(cVertex.GetX() + c_center_offset.GetX(),
                 cVertex.GetY() + c_center_offset.GetY(),
                 cVertex.GetZ() + c_center_offset.GetZ());

      cVertex.Set(-f_height * 0.5f, -f_base * 0.5f, 0.0f);
      cVertex.Rotate(c_orientation);
      glVertex3f(cVertex.GetX() + c_center_offset.GetX(),
                 cVertex.GetY() + c_center_offset.GetY(),
                 cVertex.GetZ() + c_center_offset.GetZ());
      glEnd();

      glEnable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
   }

   /****************************************/
   /****************************************/


   void CQTOpenGLUserFunctions::DrawCircle(Real f_radius,
                                           const CVector3& c_center_offset,
                                           const CColor& c_color,
                                           const bool b_fill,
                                           const CQuaternion& c_orientation,
                                           GLuint un_vertices) {

	    glDisable(GL_LIGHTING);
	    glDisable(GL_CULL_FACE);

	    glColor3ub(c_color.GetRed(),
                 c_color.GetGreen(),
                 c_color.GetBlue());

	    CVector3 cVertex(f_radius, 0.0f, 0.0f);
	    CRadians cAngle(CRadians::TWO_PI / un_vertices);

	  if(b_fill) {
          glBegin(GL_POLYGON);
	  }
	  else {
		  glBegin(GL_LINE_LOOP);
	  }
      CVector3 cNormalDirection(0.0f, 0.0f, 1.0f);
      cNormalDirection.Rotate(c_orientation);
      glNormal3f(cNormalDirection.GetX(),
                 cNormalDirection.GetY(),
                 cNormalDirection.GetZ());

      /* Compute the quaternion defining the rotation of the vertices used to draw the circle. */
      CQuaternion cVertexRotation;
      CVector3 cVertexRotationAxis(0.0f, 0.0f, 1.0f);
      cVertexRotationAxis.Rotate(c_orientation);
      cVertexRotation.FromAngleAxis(cAngle, cVertexRotationAxis);

	    cVertex.Rotate(c_orientation);
      for(GLuint i = 0; i <= un_vertices; i++) {
         glVertex3f(cVertex.GetX() + c_center_offset.GetX(),
                    cVertex.GetY() + c_center_offset.GetY(),
                    cVertex.GetZ() + c_center_offset.GetZ());
         cVertex.Rotate(cVertexRotation);
      }
      glEnd();

      glEnable(GL_CULL_FACE);
      glEnable(GL_LIGHTING);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawCylinder(Real f_radius,
                                             Real f_height,
                                             const CVector3& c_center_offset,
                                             const CColor& c_color,
                                             const CQuaternion& c_orientation,
                                             GLuint un_vertices) {

    	/* Draw top circle*/
    	CVector3 cCirclePos(0.0f, 0.0f, f_height * 0.5f);
    	cCirclePos.Rotate(c_orientation);
    	cCirclePos += c_center_offset;
    	DrawCircle(f_radius,
                 cCirclePos,
                 c_color,
                 true,
                 c_orientation,
                 un_vertices);

    	/* Draw bottom circle*/
    	cCirclePos.Set(0.0f, 0.0f, -f_height * 0.5f);
    	cCirclePos.Rotate(c_orientation);
    	cCirclePos += c_center_offset;
    	DrawCircle(f_radius,
                 cCirclePos,
                 c_color,
                 true,
                 c_orientation,
                 un_vertices);

    	/* Side surface */
      CVector3 cVertex1(f_radius, 0.0f,  f_height * 0.5f);
      CVector3 cVertex2(f_radius, 0.0f, -f_height * 0.5f);
      CRadians cAngle(CRadians::TWO_PI / un_vertices);

      /* Compute the quaternion defining the rotation of the vertices used to draw the side surface. */
      CQuaternion cVertexRotation;
      CVector3 cVertexRotationAxis(0.0f, 0.0f, 1.0f);
      cVertexRotationAxis.Rotate(c_orientation);
      cVertexRotation.FromAngleAxis(cAngle, cVertexRotationAxis);

      glDisable(GL_LIGHTING);
      glColor3ub(c_color.GetRed(),
                 c_color.GetGreen(),
                 c_color.GetBlue());

      glBegin(GL_QUAD_STRIP);

      /* Compute the normal direction of the starting edge. */
      CVector3 cNormalDirection(cVertex1.GetX(), cVertex1.GetY(), 0.0f);
      cNormalDirection.Rotate(c_orientation);
      glNormal3f(cNormalDirection.GetX(),
                 cNormalDirection.GetY(),
                 cNormalDirection.GetZ());

      /* Rotate the endpoints of the first edge.*/
      cVertex1.Rotate(c_orientation);
      cVertex2.Rotate(c_orientation);

      for(GLuint i = 0; i <= un_vertices; i++) {
         glVertex3f(cVertex1.GetX() + c_center_offset.GetX(),
                    c_center_offset.GetY() + cVertex1.GetY(),
                    c_center_offset.GetZ() + cVertex1.GetZ() );
         glVertex3f(cVertex2.GetX() + c_center_offset.GetX(),
                    c_center_offset.GetY() + cVertex2.GetY(),
                    c_center_offset.GetZ() + cVertex2.GetZ() );
         /* Rotate the vertices and the normal direction, set the new normal. */
         cVertex1.Rotate(cVertexRotation);
         cVertex2.Rotate(cVertexRotation);
         cNormalDirection.Rotate(cVertexRotation);
         glNormal3f(cNormalDirection.GetX(),
                    cNormalDirection.GetY(),
                    cNormalDirection.GetZ());
      }

      glEnd();
      glEnable(GL_LIGHTING);

   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawSegment(const CVector3& c_end_point,
                                            const CVector3& c_start_point,
                                            const CColor& c_segment_color,
                                            const Real& f_line_width,
                                            bool b_draw_end_point,
                                            bool b_draw_start_point,
                                            const CColor& c_end_point_color,
                                            const CColor& c_start_point_color) {

      /* Draw the segment */

      glDisable(GL_LIGHTING);
      glColor3ub(c_segment_color.GetRed(),
                 c_segment_color.GetGreen(),
                 c_segment_color.GetBlue());

      glEnable (GL_LINE_SMOOTH);
      glLineWidth(f_line_width);

      glBegin(GL_LINES);
      glVertex3f(c_start_point.GetX(),
                 c_start_point.GetY(),
                 c_start_point.GetZ() );
      glVertex3f(c_end_point.GetX(),
                 c_end_point.GetY(),
                 c_end_point.GetZ() );
      glEnd();

      /* Draw the end and start points if necessary */

      if(b_draw_end_point) {
    	 DrawPoint(c_end_point, c_end_point_color, 5.0);
      }

      if(b_draw_start_point) {
    	 DrawPoint(c_start_point, c_start_point_color, 5.0);
      }

      glPointSize(1.0);
      glEnable(GL_LIGHTING);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawPoint(const CVector3& c_position,
                                          const CColor& c_color,
                                          const Real f_point_diameter) {
       glDisable(GL_LIGHTING);
       glColor3ub(c_color.GetRed(),
    		      c_color.GetGreen(),
                  c_color.GetBlue());
       glPointSize(f_point_diameter);
       glBegin(GL_POINTS);
  	   glVertex3f(c_position.GetX(), c_position.GetY(), c_position.GetZ());
  	   glEnd();
       glPointSize(1.0);
       glEnable(GL_LIGHTING);

   }

   /****************************************/
   /****************************************/

   void CQTOpenGLUserFunctions::DrawPoligon(const std::vector<CVector3>& vec_points,
                                            const CColor& c_color) {
      if (vec_points.size() > 2)
      {
         glDisable(GL_LIGHTING);
         glDisable(GL_CULL_FACE);

         glColor3ub(c_color.GetRed(),
                    c_color.GetGreen(),
                    c_color.GetBlue());

         glBegin(GL_POLYGON);
         for (UInt32 i = 0; i < vec_points.size(); ++i)
         {
            glVertex3f(vec_points[i].GetX(),
                       vec_points[i].GetY(),
                       vec_points[i].GetZ());
         }
         glEnd();

         glEnable(GL_CULL_FACE);
         glEnable(GL_LIGHTING);
      }
   }

   /****************************************/
   /****************************************/


}

