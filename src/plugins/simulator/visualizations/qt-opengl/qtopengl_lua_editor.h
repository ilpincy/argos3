
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_editor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef QTOPENGL_LUA_EDITOR_H
#define QTOPENGL_LUA_EDITOR_H

namespace argos {
   class CQTOpenGLLuaEditor;
}

#include <QPlainTextEdit>

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLLuaEditor : public QPlainTextEdit {

      Q_OBJECT

   public:

      CQTOpenGLLuaEditor(QWidget* pc_parent);
      virtual ~CQTOpenGLLuaEditor() {}
      
      void LineNumberAreaPaintEvent(QPaintEvent* pc_event);
      int LineNumberAreaWidth();

   protected:

      void resizeEvent(QResizeEvent* pc_event);

   private slots:

      void UpdateLineNumberAreaWidth(int);
      void HighlightCurrentLine();
      void UpdateLineNumberArea(const QRect& c_rect, int n_dy);

   private:

      /********************/
      /********************/

      class CLineNumberArea : public QWidget {

      public:
         CLineNumberArea(CQTOpenGLLuaEditor* pc_editor) :
            QWidget(pc_editor) {
            m_pcEditor = pc_editor;
         }

         QSize sizeHint() const {
            return QSize(m_pcEditor->LineNumberAreaWidth(), 0);
         }

      protected:

         void paintEvent(QPaintEvent* pc_event) {
            m_pcEditor->LineNumberAreaPaintEvent(pc_event);
         }

      private:

         CQTOpenGLLuaEditor* m_pcEditor;

      };

      /********************/
      /********************/

      CLineNumberArea* m_pcLineNumberArea;

   };

}

#endif
