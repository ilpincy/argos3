
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_statetree_model.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef QTOPENGL_LUA_STATETREE_MODEL_H
#define QTOPENGL_LUA_STATETREE_MODEL_H

namespace argos {
   class CQTOpenGLLuaStateTreeModel;
   class CQTOpenGLLuaStateTreeVariableModel;
   class CQTOpenGLLuaStateTreeFunctionModel;
   class CQTOpenGLLuaStateTreeItem;   
}

extern "C" {
#include <lua.h>
}

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLLuaStateTreeModel : public QAbstractItemModel {

      Q_OBJECT

   public:

      CQTOpenGLLuaStateTreeModel(lua_State* pt_state,
                                 bool b_remove_empty_tables,
                                 QObject* pc_parent = 0);

      virtual ~CQTOpenGLLuaStateTreeModel();

      virtual QVariant data(const QModelIndex& c_index,
                            int n_role) const;

      virtual Qt::ItemFlags flags(const QModelIndex& c_index) const;

      virtual QModelIndex index(int n_row,
                                int n_column,
                                const QModelIndex& c_parent = QModelIndex()) const;
      
      virtual QModelIndex parent(const QModelIndex& c_index) const;

      virtual int rowCount(const QModelIndex& c_parent = QModelIndex()) const;

      void SetLuaState(lua_State* pt_state);

   public slots:

      void Refresh();
      void Refresh(int);

   protected:

      void ProcessLuaState(lua_State* pt_state,
                           CQTOpenGLLuaStateTreeItem* pc_item);

      virtual bool IsTypeVisitable(lua_State* pt_state) = 0;

   private:

      lua_State* m_ptState;
      CQTOpenGLLuaStateTreeItem* m_pcDataRoot;
      bool m_bRemoveEmptyTables;

   };

   /****************************************/
   /****************************************/

   class CQTOpenGLLuaStateTreeVariableModel : public CQTOpenGLLuaStateTreeModel {

      Q_OBJECT

   public:

      CQTOpenGLLuaStateTreeVariableModel(lua_State* pt_state,
                                         bool b_remove_empty_tables,
                                         QObject* pc_parent = 0);

      virtual ~CQTOpenGLLuaStateTreeVariableModel() {}

      virtual QVariant headerData(int n_section,
                                  Qt::Orientation e_orientation,
                                  int n_role = Qt::DisplayRole) const;

      virtual int columnCount(const QModelIndex& c_parent = QModelIndex()) const;

   protected:

      virtual bool IsTypeVisitable(lua_State* pt_state);

   };

   /****************************************/
   /****************************************/

   class CQTOpenGLLuaStateTreeFunctionModel : public CQTOpenGLLuaStateTreeModel {

      Q_OBJECT

   public:

      CQTOpenGLLuaStateTreeFunctionModel(lua_State* pt_state,
                                         bool b_remove_empty_tables,
                                         QObject* pc_parent = 0);

      virtual ~CQTOpenGLLuaStateTreeFunctionModel() {}

      virtual QVariant headerData(int n_section,
                                  Qt::Orientation e_orientation,
                                  int n_role = Qt::DisplayRole) const;

      virtual int columnCount(const QModelIndex& c_parent = QModelIndex()) const;

   protected:

      virtual bool IsTypeVisitable(lua_State* pt_state);

   };

   /****************************************/
   /****************************************/

}

#endif
