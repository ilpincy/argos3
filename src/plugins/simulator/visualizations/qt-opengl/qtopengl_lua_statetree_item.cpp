/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_statetree_item.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "qtopengl_lua_statetree_item.h"
#include <cstdio>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaStateTreeItem::CQTOpenGLLuaStateTreeItem(CQTOpenGLLuaStateTreeItem* pc_parent) :
      m_pcParent(pc_parent) {}

   /****************************************/
   /****************************************/

   CQTOpenGLLuaStateTreeItem::CQTOpenGLLuaStateTreeItem(QList<QVariant>& list_data,
                                                        CQTOpenGLLuaStateTreeItem* pc_parent) :
      m_listData(list_data),
      m_pcParent(pc_parent) {}

   /****************************************/
   /****************************************/

   CQTOpenGLLuaStateTreeItem::~CQTOpenGLLuaStateTreeItem() {
      qDeleteAll(m_listChildren);
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaStateTreeItem* CQTOpenGLLuaStateTreeItem::GetParent() {
      return m_pcParent;
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaStateTreeItem* CQTOpenGLLuaStateTreeItem::GetChild(size_t un_idx) {
      return m_listChildren.value(un_idx);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaStateTreeItem::AddChild(CQTOpenGLLuaStateTreeItem* pc_child) {
      m_listChildren.append(pc_child);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaStateTreeItem::RemoveChild(CQTOpenGLLuaStateTreeItem* pc_child) {
      m_listChildren.removeOne(pc_child);
   }

   /****************************************/
   /****************************************/

   size_t CQTOpenGLLuaStateTreeItem::GetNumChildren() const {
      return m_listChildren.count();
   }

   /****************************************/
   /****************************************/

   bool ItemLessThan(const CQTOpenGLLuaStateTreeItem* pc_i1,
                     const CQTOpenGLLuaStateTreeItem* pc_i2) {
      if(pc_i1->GetData(0).type() == QVariant::Double &&
         pc_i2->GetData(0).type() == QVariant::Double) {
         return pc_i1->GetData(0).toDouble() < pc_i2->GetData(0).toDouble();
      }
      else {
         return pc_i1->GetData(0).toString().toLower() < pc_i2->GetData(0).toString().toLower();
      }
   }

   void CQTOpenGLLuaStateTreeItem::SortChildren() {
      qSort(m_listChildren.begin(), m_listChildren.end(), ItemLessThan);
      foreach(CQTOpenGLLuaStateTreeItem* pcItem, m_listChildren) {
         pcItem->SortChildren();
      }
   }

   /****************************************/
   /****************************************/

   QVariant CQTOpenGLLuaStateTreeItem::GetData(int n_col) const {
      return m_listData.value(n_col);
   }

   /****************************************/
   /****************************************/

   int CQTOpenGLLuaStateTreeItem::GetRow() {
      if(m_pcParent != NULL) {
         return m_pcParent->m_listChildren.indexOf(const_cast<CQTOpenGLLuaStateTreeItem*>(this));
      }
      else {
         return 0;
      }
   }

   /****************************************/
   /****************************************/

}
