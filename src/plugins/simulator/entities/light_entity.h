/**
 * @file <argos3/plugins/simulator/entities/light_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LIGHT_ENTITY_H
#define LIGHT_ENTITY_H

namespace argos {
   class CLightEntity;
   class CLedEquippedEntity;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>

namespace argos {

   class CLightEntity : public CLEDEntity {

   public:

      ENABLE_VTABLE();

   public:

      CLightEntity();
      
      CLightEntity(const std::string& str_id,
                   const CVector3& c_position,
                   const CColor& c_color,
                   Real f_intensity);

      virtual void Init(TConfigurationNode& t_tree);

      inline Real GetIntensity() const {
         return m_fIntensity;
      }

      inline void SetIntensity(Real f_intensity) {
         m_fIntensity = f_intensity;
      }

      virtual std::string GetTypeDescription() const {
         return "light";
      }

   protected:

      Real m_fIntensity;
   };

}

#endif
