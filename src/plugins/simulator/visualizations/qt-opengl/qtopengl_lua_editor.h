#ifndef LUA_EDITOR
#define LUA_EDITOR

namespace argos {
   class CQTOpenGLLuaEditor;
   class CQTOpenGLMainWindow;
   class CLuaController;
}

class QStatusBar;
class QPlainTextEdit;

#include <QMainWindow>

namespace argos {

   class CQTOpenGLLuaEditor : public QMainWindow {

      Q_OBJECT

   public:

      CQTOpenGLLuaEditor(CQTOpenGLMainWindow* pc_main_window);
      virtual ~CQTOpenGLLuaEditor();

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
      void CreateCodeEditor();
      void CreateFileActions();
      void CreateLuaActions();
      void OpenFile(const QString& str_path = QString());
      bool SaveFile(const QString& str_path = QString());
      void SetCurrentFile(const QString& str_path);

   private:

      CQTOpenGLMainWindow* m_pcMainWindow;
      QStatusBar* m_pcStatusbar;
      QPlainTextEdit* m_pcCodeEditor;
      std::vector<CLuaController*> m_vecControllers;
      QString m_strFileName;

   };

}

#endif
