/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_turret_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_TURRET_ENTITY_H
#define FOOTBOT_TURRET_ENTITY_H

namespace argos {
   class CFootBotTurretEntity;
   struct SAnchor;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CFootBotTurretEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      enum EMode {
         MODE_OFF,
         MODE_PASSIVE,
         MODE_SPEED_CONTROL,
         MODE_POSITION_CONTROL
      };

   public:

      CFootBotTurretEntity(CComposableEntity* pc_parent);

      CFootBotTurretEntity(CComposableEntity* pc_parent,
                           const std::string& str_id,
                           SAnchor& s_anchor);

      virtual void Init(TConfigurationNode& t_tree);
      
      virtual void Reset();

      virtual void Update();

      inline UInt32 GetMode() const {
         return m_unMode;
      }

      inline void SetMode(UInt32 un_mode) {
         m_unMode = un_mode;
      }

      CRadians GetRotation() const;

      Real GetRotationSpeed() const;

      const CRadians& GetDesiredRotation() const;

      Real GetDesiredRotationSpeed() const;

      void SetDesiredRotation(const CRadians& c_rotation);

      void SetDesiredRotationSpeed(Real f_speed);

      virtual std::string GetTypeDescription() const {
         return "turret";
      }

   private:

      /* Anchor associated to this turret */
      SAnchor* m_psAnchor;
      /* Operational mode (off, passive, rotation control, speed control */
      UInt32 m_unMode;
      /* Desired rotation set by the actuator */
      CRadians m_cDesRot;
      /* Desired rotation speed set by the actuator */
      Real m_fDesRotSpeed;
      /* Current rotation speed */
      Real m_fCurRotSpeed;
      /* Rotation at previous time step */
      CRadians m_cOldRot;

   };
}

#endif
