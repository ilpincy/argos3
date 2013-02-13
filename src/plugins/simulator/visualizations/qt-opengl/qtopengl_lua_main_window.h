#ifndef QTOPENGL_LUA_MAIN_WINDOW_H
#define QTOPENGL_LUA_MAIN_WINDOW_H

namespace argos {
   class CQTOpenGLLuaMainWindow;
   class CQTOpenGLLuaEditor;
   class CQTOpenGLMainWindow;
   class CLuaController;
}

class QAction;
class QStatusBar;

#include <QMainWindow>

namespace argos {

   class CQTOpenGLLuaMainWindow : public QMainWindow {

      Q_OBJECT

   public:

      CQTOpenGLLuaMainWindow(CQTOpenGLMainWindow* pc_parent);
      virtual ~CQTOpenGLLuaMainWindow();

   public slots:

      void New();
      void Open();
      bool Save();
      bool SaveAs();
      void Execute();
      void CodeModified();
      void CheckLuaStatus(int n_step);

   private:

      bool MaybeSave();
      void PopulateLuaControllers();
      void ReadSettings();
      void WriteSettings();
      void CreateCodeEditor();
      void CreateFileActions();
      void CreateEditActions();
      void CreateCodeActions();
      void OpenFile(const QString& str_path = QString());
      bool SaveFile(const QString& str_path = QString());
      void SetCurrentFile(const QString& str_path);

   private:

      CQTOpenGLMainWindow* m_pcMainWindow;
      QStatusBar* m_pcStatusbar;
      CQTOpenGLLuaEditor* m_pcCodeEditor;
      std::vector<CLuaController*> m_vecControllers;
      QString m_strFileName;

      QAction* m_pcFileNewAction;
      QAction* m_pcFileOpenAction;
      QAction* m_pcFileSaveAction;
      QAction* m_pcFileSaveAsAction;
      QAction* m_pcEditUndoAction;
      QAction* m_pcEditRedoAction;
      QAction* m_pcEditCopyAction;
      QAction* m_pcEditCutAction;
      QAction* m_pcEditPasteAction;
      QAction* m_pcEditFindAction;
      QAction* m_pcEditSubstituteAction;
      QAction* m_pcCodeExecuteAction;

   };

}

#endif
