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

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   /**
    * The QTOpenGL user functions.
    * <p>
    * The QTOpenGL user functions allows you to draw objects on the graphical widget that
    * shows the 3D robot arena (CQTOpenGLWidget). You have three ways to draw stuff.
    * </p>
    * <p>
    * The first is to use DrawInWorld(). This method is called after everything else has been drawn
    * in the 3D world, and it allows you to draw custom objects anywhere in the world. When you use
    * this method, coordinates are relative to the origin of the 3D arena, and you must use OpenGL
    * primitives.
    * </p>
    * <p>
    * The second way to draw custom stuff is to use DrawOverlay(). This method is called after
    * DrawInWorld(). It allows you to draw stuff as overlay of the 3D arena widget. This method
    * requires you to use the methods in the Qt class QPainter.
    * </p>
    * <p>
    * The third way to draw custom stuff is to define an entity-specific method, as follows:
    * </p>
    * <pre>
    * class MyOpenGLUserFunctions : public CQTOpenGLUserFunctions {
    * public:
    *    MyOpenGLUserFunctions();
    *    void Draw(CFootBotEntity& c_entity);
    *    ...
    * };
    * </pre>
    * <p>
    * The signature of the method must be as in the example, i.e., the return value <em>must</em>
    * be <tt>void</tt>, and the function <em>must</em> have as a unique argument an entity (i.e., a
    * descendant of CEntity). The actual name of the function (e.g., <tt>Draw</tt>) can be anything you
    * like. To tell ARGoS to use this function, you need to register it in the class constructor as in
    * this example:
    * </p>
    * <pre>
    * MyOpenGLUserFunctions::MyOpenGLUserFunctions() {
    *    ...
    *    RegisterUserFunction<MyOpenGLUserFunctions,CFootBotEntity>(&MyOpenGLUserFunctions::Draw);
    *    ...
    * }
    * </pre>
    * <p>
    * After registration, ARGoS will call your method right after drawing an entity of the corresponding
    * type. The method allows you to draw additional information around the robot. You must use OpenGL
    * primitives. The coordinate system is relative to the robot reference point, which, for robots, is
    * usually its base.
    * </p>
    */
   class CQTOpenGLUserFunctions : public CBaseConfigurableResource {

   public:

      /**
       * Class constructor.
       */
      CQTOpenGLUserFunctions();

      /**
       * Class destructor.
       */
      virtual ~CQTOpenGLUserFunctions();

      virtual void Init(TConfigurationNode& t_tree) {}
      virtual void Reset() {}
      virtual void Destroy() {}

      /**
       * Called when a key press event occurs.
       * The focus must be on the QTOpenGL widget.
       * QTOpenGL reserves the following keys for camera movement:
       * - arrows
       * - Q W E A S D
       * If this function does not manage a keypress, it must end by
       * calling CQTOpenGLWidget::KeyPressed().
       * The default implementation calls CQTOpenGLWidget::KeyPressed().
       * @param pc_event The key press event.
       * @see QKeyEvent
       */
      virtual void KeyPressed(QKeyEvent* pc_event);

      /**
       * Called when a key release event occurs.
       * The focus must be on the QTOpenGL widget.
       * QTOpenGL reserves the following keys for camera movement:
       * - arrows
       * - Q W E A S D
       * If this function does not manage a key release, it must end by
       * calling CQTOpenGLWidget::KeyReleased().
       * The default implementation calls CQTOpenGLWidget::KeyReleased().
       * @param pc_event The key release event.
       * @see QKeyEvent
       */
      virtual void KeyReleased(QKeyEvent* pc_event);

      /**
       * Called when a mouse key is pressed.
       *
       * The focus must be on the QTOpenGL widget.
       *
       * QTOpenGL reserves the following events:
       * - mouse pressed with no key modifier
       * - mouse pressed while shift is pressed
       * - mouse pressed while ctrl is pressed
       *
       * Other combinations, e.g., mouse pressed while alt is pressed,
       * are available.
       */
      virtual void MouseKeyPressed(QMouseEvent* pc_event) {}

      /**
       * Called when a mouse key is released.
       *
       * The focus must be on the QTOpenGL widget.
       *
       * QTOpenGL reserves the following events:
       * - mouse released with no key modifier
       * - mouse released while shift is pressed
       * - mouse released while ctrl is pressed
       *
       * Other combinations, e.g., mouse released while alt is
       * pressed, are available.
       */
      virtual void MouseKeyReleased(QMouseEvent* pc_event) {}

      /**
       * Called when the mouse is moved.
       *
       * The focus must be on the QTOpenGL widget.
       *
       * QTOpenGL reserves the following events:
       * - mouse moved while mouse is grabbed. This happens when the camera is being moved.
       */
      virtual void MouseMoved(QMouseEvent* pc_event) {}

      /**
       * Called every time an entity is selected.
       * @param c_entity The selected entity.
       */
      virtual void EntitySelected(CEntity& c_entity) {}

      /**
       * Called every time an entity is deselected.
       * @param c_entity The deselected entity.
       */
      virtual void EntityDeselected(CEntity& c_entity) {}

      /**
       * Called every time an entity is moved.
       * @param c_entity The moved entity.
       * @param c_old_pos The old position of the entity.
       * @param c_new_pos The new position of the entity.
       */
      virtual void EntityMoved(CEntity& c_entity,
                               const CVector3& c_old_pos,
                               const CVector3& c_new_pos) {}

      /**
       * Called every time an entity is rotated.
       * @param c_entity The rotated entity.
       * @param c_old_orientation The old orientation of the entity.
       * @param c_new_orientation The new orientation of the entity.
       */
      virtual void EntityRotated(CEntity& c_entity,
                                 const CQuaternion& c_old_orientation,
                                 const CQuaternion& c_new_orientation) {}

      /**
       * Returns the currently selected entity, or NULL.
       */
      CEntity* GetSelectedEntity();

      /**
       * Selects the specified entity.
       * - If no entity was previously selected, the specified entity
       *   is selected and EntitySelected() is called.
       * - If an entity was previously selected, EntityDeselected() is
       *   called for that entity; then, the specified entity is
       *   selected and EntitySelected() is called.
       */
      virtual void SelectEntity(CEntity& c_entity);

      /**
       * Deselects the currently selected entity.
       * - If no entity was selected, nothing is done.
       * - If an entity was previously selected, EntityDeselected() is
       *   called for that entity.
       */
      virtual void DeselectEntity();

      /**
       * Drawing hook executed after the floor is drawn.
       */
      virtual void Draw(CFloorEntity& c_entity) {}

      /**
       * Drawing hook executed after all objects have been drawn.
       * Use this hook to draw your own stuff in the world. You must use
       * OpenGL primitives. Coordinates are expressed wrt the world's origin.
       */
      virtual void DrawInWorld() {}

      /**
       * Drawing hook to put graphics on top of the OpenGL window.
       * Extend this method to draw stuff on top of the 3D graphical window.
       * Use the methods in the Qt4 QPainter class to add all the
       * stuff you want, such as text, shapes and so on.
       * @param c_painter The QPainter object to draw the overlay.
       */
      virtual void DrawOverlay(QPainter& c_painter) {}

      /**
       * Returns the QTOpenGL main window.
       * @return The QTOpenGL main window.
       */
      CQTOpenGLMainWindow& GetMainWindow();

      /**
       * Sets the QTOpenGL main window for these user functions.
       * @param c_widget The QTOpenGL main window.
       */
      void SetMainWindow(CQTOpenGLMainWindow& c_main_win);

      /**
       * Returns the QTOpenGLWidget.
       * @return The QTOpenGLWidget.
       */
      CQTOpenGLWidget& GetQTOpenGLWidget();

      /**
       * Sets the current drawing color.
       * @param c_color The desired color.
       */
      void SetColor(const CColor& c_color);

      /**
       * Draws a point.
       * By default the point drawn in red.
       * @param c_position The point position.
       * @param c_color The point color.
       * @param f_diameter The point diameter.
       */
      void DrawPoint(const CVector3& c_position,
                     const CColor& c_color = CColor::RED,
                     Real f_diameter  = 5.0);

      /**
       * Draws an isosceles triangle.
       * By default the triangle is parallel to the XY plane, equilateral, with the height along the X axis.
       * The triangle reference system is positioned in at the center of the triangle base.
       * @param c_position The triangle position.
       * @param c_orientation The triangle orientation.
       * @param f_base The length of the triangle base.
       * @param f_height The length of the triangle height.
       * @param c_color The triangle color.
       * @param b_fill When <tt>true</tt>, the triangle is filled.
       */
      void DrawTriangle(const CVector3& c_position,
                        const CQuaternion& c_orientation,
                        Real f_base,
                        Real f_height,
                        const CColor& c_color = CColor::RED,
                        const bool b_fill = true);

      /**
       * Draws a 2D polygon.
       * By default the polygon is parallel to the XY plane and its fill color is red.
       * @param c_position The polygon position.
       * @param c_orientation The polygon orientation.
       * @param vec_points The polygon vertices.
       * @param c_color The polygon color.
       * @param b_fill When <tt>true</tt>, the polygon is filled.
       */
      void DrawPolygon(const CVector3& c_position,
                       const CQuaternion& c_orientation,
                       const std::vector<CVector2>& vec_points,
                       const CColor& c_color = CColor::RED,
                       const bool b_fill = true);

      /**
       * Draws a circle.
       * By default the circle is parallel to the XY plane and filled in red.
       * The circle reference system is positioned at the circle center.
       * @param c_position The circle position.
       * @param c_orientation The circle orientation.
       * @param f_radius The circle radius.
       * @param c_color The circle color.
       * @param b_fill When <tt>true</tt>, the triangle is filled.
       * @param un_vertices The number of vertices used to draw the circle.
       */
      void DrawCircle(const CVector3& c_position,
                      const CQuaternion& c_orientation,
                      Real f_radius,
                      const CColor& c_color = CColor::RED,
                      const bool b_fill = true,
                      GLuint un_vertices = 20);

      /**
       * Draws a cylinder, with the height perpendicular to the XY plane.
       * By default the cylinder is red and positioned in the origin.
       * The reference system is positioned at the center of mass.
       * @param c_position The cylinder position.
       * @param c_orientation The cylinder orientation of the cylinder.
       * @param f_radius The cylinder radius.
       * @param f_height The cylinder height.
       * @param c_color The cylinder color.
       * @param un_vertices The number of vertices used to draw the cylinder bases.
       */
      void DrawCylinder(const CVector3& c_position,
                        const CQuaternion& c_orientation,
                        Real f_radius,
                        Real f_height,
                        const CColor& c_color = CColor::RED,
                        GLuint un_vertices = 20);

      /**
       * Draws a box.
       * By default the box is red and positioned in the origin.
       * The reference system is positioned at the center of mass.
       * @param c_position The position of the box center.
       * @param c_orientation The orientation of the box.
       * @param c_size The size of the box.
       * @param c_color The box color.
       */
      void DrawBox(const CVector3& c_position,
                   const CQuaternion& c_orientation,
                   const CVector3& c_size,
                   const CColor& c_color = CColor::RED);

      /**
       * Draws a ray, with optional endpoint markers.
       * By default the ray is drawn in red without end points markers. The end point markers default color is red.
       * @param c_ray The ray coordinates.
       * @param c_color The ray color.
       * @param f_width The ray width.
       */
      void DrawRay(const CRay3& c_ray,
                   const CColor& c_color = CColor::RED,
                   Real f_width = 1.0f);

      /**
       * Draws a string of text.
       * By default the text is black and aligned left.
       * @param c_position The text position.
       * @param str_text The text to display
       * @param c_color The text color
       * @param c_font A Qt font configuration
       */
      void DrawText(const CVector3& c_position,
                    const std::string& str_text,
                    const CColor& c_color = CColor::BLACK,
                    const QFont& c_font = QFont());

   protected:

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
       * @see TThunk
       */
      template <typename USER_IMPL, typename ENTITY>
      void Thunk(CEntity& c_entity);

      /**
       * The base function holder.
       * @see CFunctionHolderImpl
       */
      class CFunctionHolder {};

      /**
       * The actual function holder.
       * This template class holds a pointer to a user-defined method.
       * @param USER_IMPL A user-defined subclass of CQTOpenGLUserFunctions.
       * @param ENTITY The entity type to pass as a parameter to the user-defined method.
       * @see CFunctionHolder
       */
      template <typename USER_IMPL, typename ENTITY> class CFunctionHolderImpl : public CFunctionHolder {
      public:
         typedef void (USER_IMPL::*TFunction)(ENTITY&);
         TFunction Function;
         CFunctionHolderImpl(TFunction t_function) : Function(t_function) {}
      };

      /**
       * The vtable storing the thunks.
       * @see TThunk
       * @see Thunk
       */
      CVTable<CQTOpenGLUserFunctions, CEntity, TThunk> m_cThunks;

      /**
       * A vector of function holders.
       * @see CFunctionHolder
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
      virtual void Call(CEntity& c_entity);

   private:

      /**
       * A pointer to the CQTOpenGLMainWindow.
       */
      CQTOpenGLMainWindow* m_pcQTOpenGLMainWindow;

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
