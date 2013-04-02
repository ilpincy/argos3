/**
 * @file <argos3/core/simulator/entity/led_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LED_ENTITY_H
#define LED_ENTITY_H

namespace argos {
   class CLEDEntity;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/space_hash.h>

namespace argos {

   class CLEDEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CLEDEntity*> TList;
      typedef CSet<CLEDEntity*> TSet;

   public:

      CLEDEntity(CComposableEntity* pc_parent);

      CLEDEntity(CComposableEntity* pc_parent,
                 const std::string& str_id,
                 const CVector3& c_position,
                 const CColor& c_color);

      virtual ~CLEDEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      inline const CColor& GetColor() const {
         return m_cColor;
      }

      inline const CColor& GetInitColor() const {
         return m_cInitColor;
      }

      inline void SetColor(const CColor& c_color) {
         m_cColor = c_color;
      }

      inline void SetInitColor(const CColor& c_color) {
         m_cInitColor = c_color;
      }

      virtual std::string GetTypeDescription() const {
         return "led";
      }

   protected:

      CColor m_cColor;
      CColor m_cInitColor;

   };

   /****************************************/
   /****************************************/

   class CLEDEntitySpaceHashUpdater : public CSpaceHashUpdater<CLEDEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CLEDEntity>& c_space_hash,
                              CLEDEntity& c_element);

   private:

      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

}

#endif
