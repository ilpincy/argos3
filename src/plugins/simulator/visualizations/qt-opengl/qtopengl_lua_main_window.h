
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_main_window.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef QTOPENGL_LUA_MAIN_WINDOW_H
#define QTOPENGL_LUA_MAIN_WINDOW_H

namespace argos {
   class CQTOpenGLLuaMainWindow;
   class CQTOpenGLLuaEditor;
   class CQTOpenGLLuaFindDialog;
   class CQTOpenGLMainWindow;
   class CLuaController;
   class CComposableEntity;
}

class QAction;
class QStatusBar;
class QTableWidget;
class QTreeView;

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
      void OpenRecentFile();
      bool Save();
      bool SaveAs();
      void Execute();
      void Find();
      void CodeModified();
      void CheckLuaStatus(int n_step);
      void HandleMsgTableSelection();
      void HandleEntitySelection(size_t un_index);
      void HandleEntityDeselection(size_t);

   private:

      bool MaybeSave();
      void PopulateLuaControllers();
      void ReadSettings();
      void WriteSettings();
      void CreateCodeEditor();
      void CreateLuaMessageTable();
      void CreateLuaStateDock();
      void CreateFileActions();
      void CreateEditActions();
      void CreateCodeActions();
      void OpenFile(const QString& str_path = QString());
      bool SaveFile(const QString& str_path = QString());
      void SetCurrentFile(const QString& str_path);
      void UpdateRecentFiles();

      QString StrippedFileName(const QString& str_path);

      virtual void closeEvent(QCloseEvent* pc_event);

   private:

      enum { MAX_RECENT_FILES = 5 };

      CQTOpenGLMainWindow* m_pcMainWindow;
      QStatusBar* m_pcStatusbar;
      CQTOpenGLLuaEditor* m_pcCodeEditor;
      CQTOpenGLLuaFindDialog* m_pcFindDialog;
      QTableWidget* m_pcLuaMessageTable;
      QDockWidget* m_pcLuaStateDock;
      QTreeView* m_pcLuaStateTree;

      std::vector<CLuaController*> m_vecControllers;
      std::vector<CComposableEntity*> m_vecRobots;
      QString m_strFileName;

      QAction* m_pcFileNewAction;
      QAction* m_pcFileOpenAction;
      QAction* m_pcFileOpenRecentAction[MAX_RECENT_FILES];
      QAction* m_pcFileSaveAction;
      QAction* m_pcFileSaveAsAction;
      QAction* m_pcFileSeparateRecentAction;
      QAction* m_pcEditUndoAction;
      QAction* m_pcEditRedoAction;
      QAction* m_pcEditCopyAction;
      QAction* m_pcEditCutAction;
      QAction* m_pcEditPasteAction;
      QAction* m_pcEditFindAction;
      QAction* m_pcCodeExecuteAction;

   };

}

#endif
