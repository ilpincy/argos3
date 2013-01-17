/**
 * @file <argos3/plugins/simulator/entities/cylinder_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Giovanni Pini - <gpini@ulb.ac.be>
 */

#ifndef CYLINDER_ENTITY_H
#define CYLINDER_ENTITY_H

namespace argos {
   class CCylinderEntity;
   class CEmbodiedEntity;
   class CLedEquippedEntity;
}

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/led_equipped_entity.h>

namespace argos {

   class CCylinderEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

      CCylinderEntity();
      virtual ~CCylinderEntity();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual CEntity& GetComponent(const std::string& str_component);
      virtual bool HasComponent(const std::string& str_component);

      virtual void UpdateComponents();

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CLedEquippedEntity& GetLEDEquippedEntity() {
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
         return "cylinder_entity";
      }

   private:

      CEmbodiedEntity*      m_pcEmbodiedEntity;
      CLedEquippedEntity*   m_pcLEDEquippedEntity;
      std::vector<CVector3> m_vecBaseLEDPositions;
      Real                  m_fRadius;
      Real                  m_fHeight;
      Real                  m_fMass;

   };

}

#endif
