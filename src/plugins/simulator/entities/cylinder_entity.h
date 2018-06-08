/**
 * @file <argos3/plugins/simulator/entities/cylinder_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Giovanni Pini - <gpini@ulb.ac.be>
 */

#ifndef CYLINDER_ENTITY_H
#define CYLINDER_ENTITY_H

namespace argos {
   class CCylinderEntity;
   class CEmbodiedEntity;
   class CLEDEquippedEntity;
}

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>

namespace argos {

   class CCylinderEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

      CCylinderEntity();

      CCylinderEntity(const std::string& str_id,
                      const CVector3& c_position,
                      const CQuaternion& c_orientation,
                      bool b_movable,
                      Real f_radius,
                      Real f_height,
                      Real f_mass = 1.0f);

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();

      /*
       * Enables the LEDs for this entity.
       * Adds the LED equipped entity to the given medium.
       * If you don't call this method, the LEDs added with
       * CCylinderEntity::AddLED() won't be updated correctly.
       * @param c_medium The medium to which the LEDs must be associated.
       * @see CCylinderEntity::AddLED()
       */
      void EnableLEDs(CLEDMedium& c_medium);

      /*
       * Disables the LEDs for this entity.
       */
      void DisableLEDs();

      /**
       * Adds an LED to this entity.
       * For the LEDs to be updated correctly, you must first call
       * CCylinderEntity::EnableLEDs().
       * @param c_offset The position of the LED wrt the origin anchor.
       * @param c_color The color of the LED.
       * @see CCylinderEntity::EnableLEDs()
       */
      void AddLED(const CVector3& c_offset,
                  const CColor& c_color = CColor::BLACK);

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CLEDEquippedEntity& GetLEDEquippedEntity() {
         return *m_pcLEDEquippedEntity;
      }

      inline Real GetRadius() const {
         return m_fRadius;
      }

      inline void SetRadius(Real c_radius) {
         m_fRadius = c_radius;
      }

      inline Real GetHeight() const {
         return m_fHeight;
      }

      inline void SetHeight(Real c_height) {
         m_fHeight = c_height;
      }

      inline Real GetMass() const {
         return m_fMass;
      }

      inline void SetMass(Real f_mass) {
         m_fMass = f_mass;
      }

      virtual std::string GetTypeDescription() const {
         return "cylinder";
      }

 private:

      CEmbodiedEntity*    m_pcEmbodiedEntity;
      CLEDEquippedEntity* m_pcLEDEquippedEntity;
      Real                m_fRadius;
      Real                m_fHeight;
      Real                m_fMass;
      CLEDMedium*         m_pcLEDMedium;

   };

}

#endif
