
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
   class CEntity;
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
      void HandleEntitySelection(CEntity* pc_entity);
      void HandleEntityDeselection(CEntity* pc_entity);
      void VariableTreeChanged();
      void FunctionTreeChanged();

   private:

      bool MaybeSave();
      void PopulateLuaControllers();
      void ReadSettings();
      void WriteSettings();
      void CreateCodeEditor();
      void CreateLuaMessageTable();
      void CreateLuaStateDocks();
      void CreateFileActions();
      void CreateEditActions();
      void CreateCodeActions();
      void OpenFile(const QString& str_path = QString());
      bool SaveFile(const QString& str_path = QString());
      void SetCurrentFile(const QString& str_path);
      void UpdateRecentFiles();
      void SetMessage(int n_row,
                      const QString& str_robot_id,
                      const QString& str_message);

      QString StrippedFileName(const QString& str_path);

      virtual void closeEvent(QCloseEvent* pc_event);

   private:

      enum { MAX_RECENT_FILES = 5 };

      CQTOpenGLMainWindow* m_pcMainWindow;
      QStatusBar* m_pcStatusbar;
      CQTOpenGLLuaEditor* m_pcCodeEditor;
      CQTOpenGLLuaFindDialog* m_pcFindDialog;
      QDockWidget* m_pcLuaMsgDock;
      QTableWidget* m_pcLuaMessageTable;
      QDockWidget* m_pcLuaVariableDock;
      QDockWidget* m_pcLuaFunctionDock;
      QTreeView* m_pcLuaVariableTree;
      QTreeView* m_pcLuaFunctionTree;

      std::vector<CLuaController*> m_vecControllers;
      std::vector<CComposableEntity*> m_vecRobots;
      size_t m_unSelectedRobot;
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
