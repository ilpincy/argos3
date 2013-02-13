#ifndef QTOPENGL_LUA_MAIN_WINDOW_H
#define QTOPENGL_LUA_MAIN_WINDOW_H

namespace argos {
   class CQTOpenGLLuaMainWindow;
   class CQTOpenGLLuaEditor;
   class CQTOpenGLMainWindow;
   class CLuaController;
}

class QStatusBar;

#include <QMainWindow>

namespace argos {

   class CQTOpenGLLuaMainWindow : public QMainWindow {

      Q_OBJECT

   public:

      CQTOpenGLLuaMainWindow(QWidget* pc_parent);
      virtual ~CQTOpenGLLuaMainWindow();

   public slots:

      void New();
      void Open();
      bool Save();
      bool SaveAs();
      void Execute();
      void CodeModified();

   private:

      bool MaybeSave();
      void PopulateLuaControllers();
      void ReadSettings();
      void WriteSettings();
      void CreateCodeEditor();
      void CreateFileActions();
      void CreateLuaActions();
      void OpenFile(const QString& str_path = QString());
      bool SaveFile(const QString& str_path = QString());
      void SetCurrentFile(const QString& str_path);
      virtual void closeEvent(QCloseEvent* pc_event);

   private:

      CQTOpenGLMainWindow* m_pcMainWindow;
      QStatusBar* m_pcStatusbar;
      CQTOpenGLLuaEditor* m_pcCodeEditor;
      std::vector<CLuaController*> m_vecControllers;
      QString m_strFileName;

   };

}

#endif
