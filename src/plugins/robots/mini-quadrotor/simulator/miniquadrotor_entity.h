/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/miniquadrotor_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MINIQUADROTOR_ENTITY_H
#define MINIQUADROTOR_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CEmbodiedEntity;
   class CRotorEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/rotor_equipped_entity.h>

namespace argos {

   class CMiniQuadrotorEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CMiniQuadrotorEntity();

      CMiniQuadrotorEntity(const std::string& str_id,
                     const std::string& str_controller_id,
                     const CVector3& c_position = CVector3(),
                     const CQuaternion& c_orientation = CQuaternion());
      
      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }
      
      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }
      
      inline CRotorEquippedEntity& GetRotorEquippedEntity() {
         return *m_pcRotorEquippedEntity;
      }

      virtual std::string GetTypeDescription() const {
         return "mini-bot";
      }

   private:

      CControllableEntity*  m_pcControllableEntity;
      CEmbodiedEntity*      m_pcEmbodiedEntity;
      CRotorEquippedEntity* m_pcRotorEquippedEntity;
   };

}

#endif
