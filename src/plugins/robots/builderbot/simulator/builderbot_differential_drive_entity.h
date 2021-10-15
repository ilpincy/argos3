/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DIFFERENTIAL_DRIVE_ENTITY_H
#define BUILDERBOT_DIFFERENTIAL_DRIVE_ENTITY_H

namespace argos {
   class CBuilderBotDifferentialDriveEntity;
}

#include <argos3/core/simulator/entity/entity.h>

namespace argos {

   class CBuilderBotDifferentialDriveEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CBuilderBotDifferentialDriveEntity(CComposableEntity* pc_parent);

      CBuilderBotDifferentialDriveEntity(CComposableEntity* pc_parent,
                                         const std::string& str_id);

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDifferentialDriveEntity() {}

      virtual void Update() {}

      virtual void Reset();

      void SetTargetVelocityLeft(Real f_left) {
         m_fTargetVelocityLeft = f_left;
      }

      Real GetTargetVelocityLeft() const {
         return m_fTargetVelocityLeft;
      }

      void SetTargetVelocityRight(Real f_right) {
         m_fTargetVelocityRight = f_right;
      }

      Real GetTargetVelocityRight() const {
         return m_fTargetVelocityRight;
      }

      void SetVelocityLeft(Real f_left) {
         m_fVelocityLeft = f_left;
      }

      Real GetVelocityLeft() const {
         return m_fVelocityLeft;
      }

      void SetVelocityRight(Real f_right) {
         m_fVelocityRight = f_right;
      }

      Real GetVelocityRight() const {
         return m_fVelocityRight;
      }

      virtual std::string GetTypeDescription() const {
         return "differential_drive";
      }

   private:
      /* velocity targets */
      Real m_fTargetVelocityLeft;
      Real m_fTargetVelocityRight;

      /* velocity as reported by the physics engine */
      Real m_fVelocityLeft;
      Real m_fVelocityRight;
   };
}

#endif
