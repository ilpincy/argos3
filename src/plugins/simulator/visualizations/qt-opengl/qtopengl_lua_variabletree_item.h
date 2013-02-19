/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_variabletree_item.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef QTOPENGL_LUA_VARIABLETREE_ITEM_H
#define QTOPENGL_LUA_VARIABLETREE_ITEM_H

namespace argos {
   class CQTOpenGLLuaVariableTreeItem;
}

#include <QList>
#include <QVariant>

namespace argos {

   class CQTOpenGLLuaVariableTreeItem {

   public:

      CQTOpenGLLuaVariableTreeItem(QList<QVariant>& list_data,
                                   CQTOpenGLLuaVariableTreeItem* pc_parent = 0);
      ~CQTOpenGLLuaVariableTreeItem();

      CQTOpenGLLuaVariableTreeItem* GetParent();

      CQTOpenGLLuaVariableTreeItem* GetChild(size_t un_idx);

      void AddChild(CQTOpenGLLuaVariableTreeItem* pc_child);

      size_t GetNumChildren() const;

      QVariant GetData(int n_col) const;

      int GetRow();

   private:

      QList<QVariant> m_listData;
      CQTOpenGLLuaVariableTreeItem* m_pcParent;
      QList<CQTOpenGLLuaVariableTreeItem*> m_listChildren;

   };

}

#endif
