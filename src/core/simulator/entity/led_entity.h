/**
 * @file <argos3/core/simulator/space/entities/led_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LED_ENTITY_H
#define LED_ENTITY_H

namespace argos {
   class CLedEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/space_hash.h>

namespace argos {

   class CLedEntity : public CEntity {

   public:

      CLedEntity(CEntity* pc_parent,
                 const CVector3& c_position,
                 const CColor& c_color) :
         CEntity(pc_parent),
         m_cPosition(c_position),
         m_cColor(c_color) {}
      virtual ~CLedEntity() {}

      virtual void Reset();

      const CVector3& GetPosition() const {
         return m_cPosition;
      }

      inline void SetPosition(const CVector3& c_position) {
         m_cPosition = c_position;
      }

      inline const CColor& GetColor() const {
         return m_cColor;
      }

      inline void SetColor(const CColor& c_color) {
         m_cColor = c_color;
      }

      virtual std::string GetTypeDescription() const {
         return "led_entity";
      }

      virtual void Update() {
         /** @todo implement this */
      }

   protected:

      CVector3 m_cPosition;
      CColor m_cColor;

   };

   typedef std::vector<CLedEntity*> TLedEntityList;
   typedef std::tr1::unordered_set<CLedEntity*> TLedEntitySet;

   /****************************************/
   /****************************************/

   class CLEDEntitySpaceHashUpdater : public CSpaceHashUpdater<CLedEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CLedEntity>& c_space_hash,
                              CLedEntity& c_element);

   private:

      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

}

#endif
