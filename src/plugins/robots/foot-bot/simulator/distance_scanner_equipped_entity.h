/**
 * @file <argos3/plugins/robots/foot-bot/simulator/distance_scanner_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <cpinciroli@ulb.ac.be>
 */

#ifndef DISTANCE_SCANNER_EQUIPPED_ENTITY_H
#define DISTANCE_SCANNER_EQUIPPED_ENTITY_H

namespace argos {
   class CDistanceScannerEquippedEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/ray.h>

namespace argos {

   class CDistanceScannerEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      enum EMode {
         MODE_OFF,
         MODE_POSITION_CONTROL,
         MODE_SPEED_CONTROL
      };

   public:

      CDistanceScannerEquippedEntity(CComposableEntity* pc_parent);

      CDistanceScannerEquippedEntity(CComposableEntity* pc_parent,
                                     const std::string& str_id);

      virtual void Reset();

      virtual void Update();

      inline UInt32 GetMode() const {
         return m_unMode;
      }

      inline void SetMode(UInt32 un_mode) {
         m_unMode = un_mode;
      }

      inline const CRadians& GetRotation() const {
         return m_cRotation;
      }

      inline void SetRotation(const CRadians& c_rotation);

      inline Real GetRotationSpeed() const {
         return m_fRotationSpeed;
      }

      inline void SetRotationSpeed(Real f_speed) {
         m_fRotationSpeed = f_speed;
      }

      virtual std::string GetTypeDescription() const {
         return "distance_scanner_equipped_entity";
      }

   private:

      UInt32 m_unMode;
      CRadians m_cRotation;
      Real m_fRotationSpeed;

   };
}

#endif
