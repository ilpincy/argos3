/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_statetree_item.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef QTOPENGL_LUA_STATETREE_ITEM_H
#define QTOPENGL_LUA_STATETREE_ITEM_H

namespace argos {
   class CQTOpenGLLuaStateTreeItem;
}

#include <QList>
#include <QVariant>

namespace argos {

   class CQTOpenGLLuaStateTreeItem {

   public:

      CQTOpenGLLuaStateTreeItem(CQTOpenGLLuaStateTreeItem* pc_parent = 0);
      CQTOpenGLLuaStateTreeItem(QList<QVariant>& list_data,
                                CQTOpenGLLuaStateTreeItem* pc_parent = 0);
      ~CQTOpenGLLuaStateTreeItem();

      CQTOpenGLLuaStateTreeItem* GetParent();

      CQTOpenGLLuaStateTreeItem* GetChild(size_t un_idx);

      void AddChild(CQTOpenGLLuaStateTreeItem* pc_child);

      void RemoveChild(CQTOpenGLLuaStateTreeItem* pc_child);

      size_t GetNumChildren() const;

      void SortChildren();

      QVariant GetData(int n_col) const;

      int GetRow();

   private:

      QList<QVariant> m_listData;
      CQTOpenGLLuaStateTreeItem* m_pcParent;
      QList<CQTOpenGLLuaStateTreeItem*> m_listChildren;

   };

}

#endif
