
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_variabletree_item.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "qtopengl_lua_variabletree_item.h"

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaVariableTreeItem::CQTOpenGLLuaVariableTreeItem(QList<QVariant>& list_data,
                                                              CQTOpenGLLuaVariableTreeItem* pc_parent) :
      m_listData(list_data),
      m_pcParent(pc_parent) {}

   /****************************************/
   /****************************************/

   CQTOpenGLLuaVariableTreeItem::~CQTOpenGLLuaVariableTreeItem() {
      qDeleteAll(m_listChildren);
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaVariableTreeItem* CQTOpenGLLuaVariableTreeItem::GetParent() {
      return m_pcParent;
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaVariableTreeItem* CQTOpenGLLuaVariableTreeItem::GetChild(size_t un_idx) {
      return m_listChildren.value(un_idx);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaVariableTreeItem::AddChild(CQTOpenGLLuaVariableTreeItem* pc_child) {
      m_listChildren.append(pc_child);
   }

   /****************************************/
   /****************************************/

   size_t CQTOpenGLLuaVariableTreeItem::GetNumChildren() const {
      return m_listChildren.count();
   }

   /****************************************/
   /****************************************/

   QVariant CQTOpenGLLuaVariableTreeItem::GetData(int n_col) const {
      return m_listData.value(n_col);
   }

   /****************************************/
   /****************************************/

   int CQTOpenGLLuaVariableTreeItem::GetRow() {
      if(m_pcParent != NULL) {
         return m_pcParent->m_listChildren.indexOf(const_cast<CQTOpenGLLuaVariableTreeItem*>(this));
      }
      else {
         return 0;
      }
   }

   /****************************************/
   /****************************************/

}
