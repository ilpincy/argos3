/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_USER_FUNCTIONS_H
#define QTOPENGL_USER_FUNCTIONS_H

namespace argos {
   class CQTOpenGLUserFunctions;
   class CFloorEntity;
}

class QPainter;

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CQTOpenGLUserFunctions {

   public:

      CQTOpenGLUserFunctions();

      virtual ~CQTOpenGLUserFunctions();

      /**
       * @brief Drawing hook executed after the floor is drawn.
       */
      inline virtual void Draw(CFloorEntity& c_entity) {}

      /**
       * @brief Drawing hook executed after all objects have been drawn.
       * Use this hook to draw your own stuff in the world.
       */
      inline virtual void DrawInWorld() {}

      /**
       * @brief Drawing hook to put graphics on top of the OpenGL window.
       * Extend this method to draw stuff on top of the 3D graphical window.
       * Use the methods in the Qt4 QPainter class to add all the
       * stuff you want, such as text, shapes and so on.
       * @param c_painter The QPainter object to draw the overlay.
       */
      inline virtual void DrawOverlay(QPainter& c_painter) {}

      /**
       * Returns the QTOpenGL widget.
       * @return The QTOpenGL widget.
       */
      inline CQTOpenGLWidget& GetOpenGLWidget() {
         return *m_pcQTOpenGLWidget;
      }

      /**
       * Sets the QTOpenGL widget for these user functions.
       * @param c_widget The QTOpenGL widget.
       */
      inline void SetOpenGLWidget(CQTOpenGLWidget& c_widget) {
         m_pcQTOpenGLWidget = &c_widget;
      }

      /**
       * @brief Draws a triangle, parallel to the XY plane.
       * By default the triangle is equilateral (edge length: 0.2m), red, with the height along the X axis, positioned in the origin.
       * The triangle reference system is positioned in (h/2,b/2,0), with the X axis along the height of the triangle.
       * @param v_center_offset triangle's center offset with respect to the origin.
       * @param c_color color of the triangle.
       * @param b_fill when true (default) the triangle is filled with color
       * @param c_orientation defines the 3D rotation of the triangle
       * @param f_base base of the the triangle
       * @param f_height height of the triangle
       */
      void DrawTriangle(const CVector3& c_center_offset = CVector3::ZERO,
                        const CColor& c_color = CColor::RED,
                        const bool b_fill = true,
                        const CQuaternion& c_orientation = CQuaternion(),
                        Real f_base = 0.2f,
                        Real f_height = 0.1732050808f);

      /**
       * @brief Draws a circle, parallel to the XY plane.
       * By default the circle is red, with a radius of 0.1m and positioned in the origin.
       * The circle reference system is positioned the center of the circle.
       * @param f_radius radius of the circle.
       * @param v_center_offset circle's center offset with respect to the origin.
       * @param c_color color of the circle.
       * @param b_fill when true (default) the circle is filled with color
       * @param c_orientation defines the 3D rotation of the circle
       * @param un_vertices number of vertices to be used for approximating the circle.
       */
      void DrawCircle(Real f_radius = 0.1f,
                      const CVector3& c_center_offset = CVector3::ZERO,
                      const CColor& c_color = CColor::RED,
                      const bool b_fill = true,
                      const CQuaternion& c_orientation = CQuaternion(),
                      GLuint un_vertices = 20);


      /**
       * @brief Draws a cylinder, with the height perpendicular to the XY plane.
       * By default the cylinder is red, with a radius of 0.1m, an height of 0.1m and positioned in the origin.
       * The cylinder reference system is positioned at half the way along the segment connecting the center of the 2 circular faces, with the z axis along the segment.
       * @param f_radius radius of the cylinder.
       * @param f_height height of the cylinder.
       * @param v_center_offset offset with respect to the origin where the circle will be drawn.
       * @param c_color color of the cylinder.
       * @param c_orientation defines the 3D rotation of the cylinder
       * @param un_vertices number of vertices to be used for approximating the circle.
       */
      void DrawCylinder(Real f_radius=0.1f,
                        Real f_height=0.1f,
                        const CVector3& c_center_offset = CVector3::ZERO,
                        const CColor& c_color = CColor::RED,
                        const CQuaternion& c_orientation = CQuaternion(),
                        GLuint un_vertices = 20);


      /**
       * @brief Draws a segment, with optional endpoint markers.
       * By default the segment starts in the origin and ends in (1,0,1), it is drawn in red without end points markers. The end point markers default color is red.
       * @param c_end_point vector specifying the end point of the segment.
       * @param c_start_point vector specifying the start point of the segment.
       * @param c_segment_color color of the segment.
       * @param b_draw_end_point when true the end point of the segment is drawn.
       * @param b_draw_start_point when true the start point of the segment is drawn.
       * @param c_end_point_color color of the end point if drawn.
       * @param c_start_point_color color of the end point if drawn.
       */
      void DrawSegment(const CVector3& c_end_point = CVector3(1.0f,0.0f,1.0f),
                       const CVector3& c_start_point = CVector3::ZERO,
                       const CColor& c_segment_color = CColor::RED,
                       const Real& f_line_width = 1.0f,
                       bool b_draw_end_point = false,
                       bool b_draw_start_point = false,
                       const CColor& c_end_point_color = CColor::RED,
                       const CColor& c_start_point_color = CColor::RED);

      /**
       * @brief Draws a polygon.
       * By default the polygon's color is red.
       * @param vec_points vector of vectors defining the vertices of the polygon
       * @param c_color color of the polygon.
       */
      void DrawPolygon(const std::vector<CVector3>& vec_points,
                       const CColor& c_color = CColor::RED);


      /**
       * @brief Draws a point
       * By default the point is positioned in the origin, drawn in red.
       * @param c_position vector specifying the position of the point
       * @param c_color color of the point.
       * @param f_point_diameter diameter of the point.
       */
      void DrawPoint(const CVector3& c_position = CVector3(1.0f,0.0f,1.0f),
                     const CColor& c_color = CColor::RED,
                     const Real f_point_diameter  = 5.0);

   private:

      /**
       * Pointer-to-thunk type definition.
       * @see Thunk
       */
      typedef void (CQTOpenGLUserFunctions::*TThunk)(CEntity&);

      /**
       * A templetized thunk.
       * This is a trampoline function that internally performs the
       * dispatch to a user-defined method.
       * @param USER_IMPL A user-defined subclass of CQTOpenGLUserFunctions.
       * @param ENTITY The entity type to pass as a parameter to the user-defined method.
       * @param c_entity The entity to pass as parameter.
       */
      template <typename USER_IMPL, typename ENTITY>
      void Thunk(CEntity& c_entity);

      /**
       * The base function holder.
       */
      class CFunctionHolder {};

      /**
       * The actual function holder.
       * This template class holds a pointer to a user-defined method.
       * @param USER_IMPL A user-defined subclass of CQTOpenGLUserFunctions.
       * @param ENTITY The entity type to pass as a parameter to the user-defined method.
       */
      template <typename USER_IMPL, typename ENTITY> class CFunctionHolderImpl : public CFunctionHolder {
      public:
         typedef void (USER_IMPL::*TFunction)(ENTITY&);
         TFunction Function;
         CFunctionHolderImpl(TFunction t_function) : Function(t_function) {}
      };

      /**
       * The vtable storing the thunks.
       */
      CVTable<CQTOpenGLUserFunctions, CEntity, TThunk> m_cThunks;

      /**
       * A vector of function holders.
       */
      std::vector<CFunctionHolder*> m_vecFunctionHolders;

   public:

      /**
       * Registers a user method.
       * @param USER_IMPL A user-defined subclass of CQTOpenGLUserFunctions.
       * @param ENTITY The entity type to pass as a parameter to the user-defined method.
       * @param pt_function The actual user-defined pointer-to-method.
       */
      template <typename USER_IMPL, typename ENTITY>
      void RegisterUserFunction(void(USER_IMPL::*pt_function)(ENTITY&));

      /**
       * Calls a user method for the given entity.
       * @param The method to pass as parameter.
       */
      void Call(CEntity& c_entity);

   private:

      /**
       * A pointer to the CQTOpenGLWidget.
       */
      CQTOpenGLWidget* m_pcQTOpenGLWidget;

   };

   /****************************************/
   /****************************************/

   template <typename USER_IMPL, typename ENTITY>
   void CQTOpenGLUserFunctions::Thunk(CEntity& c_entity) {
      /*
       * When this method is called, the static type of 'this'
       * is CQTOpenGLUserFunctions. Since we want to call
       * method in USER_IMPL (subclass of CQTOpenGLUserFunctions),
       * we need a cast. The cast is static because we trust
       * the user on not doing anything stupid.
       * The variable cImpl can be static because the cast is necessary
       * only the first time this function is called.
       */
      static USER_IMPL& cImpl = static_cast<USER_IMPL&>(*this);
      /* Cast the argument to the right type */
      ENTITY& cEntity = static_cast<ENTITY&>(c_entity);
      /* Cast the function holder to its effective type */
      CFunctionHolderImpl<USER_IMPL,ENTITY>& cFunctionHolder = static_cast<CFunctionHolderImpl<USER_IMPL,ENTITY>&>(*m_vecFunctionHolders[GetTag<ENTITY,CEntity>()]);
      /* Call the user-defined method */
      (cImpl.*(cFunctionHolder.Function))(cEntity);
   }

   template <typename USER_IMPL, typename ENTITY>
   void CQTOpenGLUserFunctions::RegisterUserFunction(void(USER_IMPL::*pt_function)(ENTITY&)) {
      /* Add the thunk to the VTable */
      m_cThunks.Add<ENTITY>(&CQTOpenGLUserFunctions::Thunk<USER_IMPL,ENTITY>);
      /* Add the function holder to the vector, padding gaps with NULL pointers */
      size_t unIdx = GetTag<ENTITY,CEntity>();
      if(m_vecFunctionHolders.size() <= unIdx) {
         m_vecFunctionHolders.resize(unIdx+1, NULL);
      }
      m_vecFunctionHolders[unIdx] = new CFunctionHolderImpl<USER_IMPL,ENTITY>(pt_function);
   }
   
   /****************************************/
   /****************************************/

}

/* Definitions useful for dynamic linking of user functions */
#define REGISTER_QTOPENGL_USER_FUNCTIONS(CLASSNAME, LABEL)  \
   REGISTER_SYMBOL(CQTOpenGLUserFunctions,                  \
                   CLASSNAME,                               \
                   LABEL,                                   \
                   "undefined",                             \
                   "undefined",                             \
                   "undefined",                             \
                   "undefined",                             \
                   "undefined")

#endif
