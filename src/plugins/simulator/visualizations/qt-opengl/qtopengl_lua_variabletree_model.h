#ifndef QTOPENGL_LUA_VARIABLETREE_MODEL_H
#define QTOPENGL_LUA_VARIABLETREE_MODEL_H

namespace argos {
   class CQTOpenGLLuaVariableTreeModel;
   class CQTOpenGLLuaVariableTreeItem;   
}

extern "C" {
#include <lua.h>
}

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace argos {

   class CQTOpenGLLuaVariableTreeModel : public QAbstractItemModel {

      Q_OBJECT

   public:

      CQTOpenGLLuaVariableTreeModel(lua_State* pt_state,
                                    QObject* pc_parent = 0);

      virtual ~CQTOpenGLLuaVariableTreeModel();

      virtual QVariant data(const QModelIndex& c_index,
                            int n_role) const;

      virtual Qt::ItemFlags flags(const QModelIndex& c_index) const;

      virtual QVariant headerData(int n_section,
                                  Qt::Orientation e_orientation,
                                  int n_role = Qt::DisplayRole) const;

      virtual QModelIndex index(int n_row,
                                int n_column,
                                const QModelIndex& c_parent = QModelIndex()) const;
      
      virtual QModelIndex parent(const QModelIndex& c_index) const;

      virtual int rowCount(const QModelIndex& c_parent = QModelIndex()) const;

      virtual int columnCount(const QModelIndex& c_parent = QModelIndex()) const;

   private:

      void SetupData();

      void ProcessLuaState(lua_State* pt_state,
                           CQTOpenGLLuaVariableTreeItem* pc_item);

   private:

      lua_State* m_ptState;
      CQTOpenGLLuaVariableTreeItem* m_pcDataRoot;

   };

}

#endif
