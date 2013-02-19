#include "qtopengl_lua_variabletree_model.h"
#include "qtopengl_lua_variabletree_item.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaVariableTreeModel::CQTOpenGLLuaVariableTreeModel(lua_State* pt_state,
                                                                QObject* pc_parent) :
      QAbstractItemModel(pc_parent),
      m_ptState(pt_state) {
      QList<QVariant> cHeader;
      cHeader << tr("Variable") << tr("Value");
      m_pcDataRoot = new CQTOpenGLLuaVariableTreeItem(cHeader);
      SetupData();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaVariableTreeModel::~CQTOpenGLLuaVariableTreeModel() {
      delete m_pcDataRoot;
   }

   /****************************************/
   /****************************************/

   QVariant CQTOpenGLLuaVariableTreeModel::data(const QModelIndex& c_index,
                                                int n_role) const {
      if(!c_index.isValid()) {
         return QVariant();
      }
      if(n_role != Qt::DisplayRole) {
         return QVariant();
      }
      CQTOpenGLLuaVariableTreeItem* pcItem = static_cast<CQTOpenGLLuaVariableTreeItem*>(c_index.internalPointer());
      return pcItem->GetData(c_index.column());
   }

   /****************************************/
   /****************************************/

   Qt::ItemFlags CQTOpenGLLuaVariableTreeModel::flags(const QModelIndex& c_index) const {
      if (!c_index.isValid()) {
         return 0;
      }
      else {
         return Qt::ItemIsEnabled;
      }
   }

   /****************************************/
   /****************************************/

   QVariant CQTOpenGLLuaVariableTreeModel::headerData(int n_section,
                                                      Qt::Orientation e_orientation,
                                                      int n_role) const {
      if(e_orientation == Qt::Horizontal &&
         n_role == Qt::DisplayRole) {
         return m_pcDataRoot->GetData(n_section);
      }
      return QVariant();
   }

   /****************************************/
   /****************************************/

   QModelIndex CQTOpenGLLuaVariableTreeModel::index(int n_row,
                                                    int n_column,
                                                    const QModelIndex& c_parent) const {
      if(!hasIndex(n_row, n_column, c_parent)) {
         return QModelIndex();
      }
      CQTOpenGLLuaVariableTreeItem* pcParentItem;
      if(!c_parent.isValid()) {
         pcParentItem = m_pcDataRoot;
      }
      else {
         pcParentItem = static_cast<CQTOpenGLLuaVariableTreeItem*>(c_parent.internalPointer());
      }
      CQTOpenGLLuaVariableTreeItem* pcChildItem = pcParentItem->GetChild(n_row);
      if(pcChildItem) {
         return createIndex(n_row, n_column, pcChildItem);
      }
      else {
         return QModelIndex();
      }
   }

   /****************************************/
   /****************************************/

   QModelIndex CQTOpenGLLuaVariableTreeModel::parent(const QModelIndex& c_index) const {
      if (!c_index.isValid()) {
         return QModelIndex();
      }
      CQTOpenGLLuaVariableTreeItem* pcChildItem = static_cast<CQTOpenGLLuaVariableTreeItem*>(c_index.internalPointer());
      CQTOpenGLLuaVariableTreeItem* pcParentItem = pcChildItem->GetParent();
      if (pcParentItem == m_pcDataRoot) {
         return QModelIndex();
      }
      else {
         return createIndex(pcParentItem->GetRow(), 0, pcParentItem);
      }
   }

   /****************************************/
   /****************************************/

   int CQTOpenGLLuaVariableTreeModel::rowCount(const QModelIndex& c_parent) const {
      CQTOpenGLLuaVariableTreeItem* pcParentItem;
      if(c_parent.column() > 0) {
         return 0;
      }
      if(!c_parent.isValid()) {
         pcParentItem = m_pcDataRoot;
      }
      else {
         pcParentItem = static_cast<CQTOpenGLLuaVariableTreeItem*>(c_parent.internalPointer());
      }
      return pcParentItem->GetNumChildren();
   }

   /****************************************/
   /****************************************/

   int CQTOpenGLLuaVariableTreeModel::columnCount(const QModelIndex& c_parent) const {
      return 2;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaVariableTreeModel::SetupData() {
      lua_getglobal(m_ptState, "_G");
      ProcessLuaState(m_ptState, m_pcDataRoot);
      lua_pop(m_ptState, 1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaVariableTreeModel::ProcessLuaState(lua_State* pt_state,
                                                       CQTOpenGLLuaVariableTreeItem* pc_item) {
      QList<QVariant> cData;
      switch(lua_type(pt_state, -2)) {
         case LUA_TBOOLEAN:
            cData << lua_toboolean(pt_state, -2);
            break;
         case LUA_TNUMBER:
            cData << lua_tonumber(pt_state, -2);
            break;
         case LUA_TSTRING:
            cData << lua_tostring(pt_state, -2);
            break;
         default: break;
      }
      if(lua_istable(pt_state, -1)) {
         CQTOpenGLLuaVariableTreeItem* pcChild = new CQTOpenGLLuaVariableTreeItem(cData, pc_item);
         pc_item->AddChild(pcChild);
         lua_pushnil(pt_state);
         while(lua_next(pt_state, -2)) {
            if(lua_type(pt_state, -1) != LUA_TFUNCTION &&
               (lua_type(pt_state, -2) != LUA_TSTRING ||
                (std::string(lua_tostring(pt_state, -2)) != "_G" &&
                 std::string(lua_tostring(pt_state, -2)) != "_VERSION" &&
                 std::string(lua_tostring(pt_state, -2)) != "package" &&
                 std::string(lua_tostring(pt_state, -2)) != "string" &&
                 std::string(lua_tostring(pt_state, -2)) != "os" &&
                 std::string(lua_tostring(pt_state, -2)) != "io" &&
                 std::string(lua_tostring(pt_state, -2)) != "math" &&
                 std::string(lua_tostring(pt_state, -2)) != "debug" &&
                 std::string(lua_tostring(pt_state, -2)) != "coroutine" &&
                 std::string(lua_tostring(pt_state, -2)) != "table"))) {
               ProcessLuaState(pt_state, pcChild);
            }
            lua_pop(pt_state, 1);
         }
      }
      else {
         switch(lua_type(pt_state, -1)) {
            case LUA_TBOOLEAN:
               cData << lua_toboolean(pt_state, -1);
               pc_item->AddChild(new CQTOpenGLLuaVariableTreeItem(cData, pc_item));
               break;
            case LUA_TNUMBER:
               cData << lua_tonumber(pt_state, -1);
               pc_item->AddChild(new CQTOpenGLLuaVariableTreeItem(cData, pc_item));
               break;
            case LUA_TSTRING:
               cData << lua_tostring(pt_state, -1);
               pc_item->AddChild(new CQTOpenGLLuaVariableTreeItem(cData, pc_item));
               break;
            default: break;
         }
      }
   }

   /****************************************/
   /****************************************/

}
