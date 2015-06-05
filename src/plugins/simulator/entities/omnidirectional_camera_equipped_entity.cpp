#include "omnidirectional_camera_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   COmnidirectionalCameraEquippedEntity::COmnidirectionalCameraEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   COmnidirectionalCameraEquippedEntity::COmnidirectionalCameraEquippedEntity(CComposableEntity* pc_parent,
                                                                              const std::string& str_id,
                                                                              const CRadians& c_aperture,
                                                                              const CVector3& c_offset) :
      CEntity(pc_parent, str_id),
      m_cAperture(c_aperture),
      m_cOffset(c_offset) {      
      Disable();
   }

   /****************************************/
   /****************************************/

   void COmnidirectionalCameraEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CEntity::Init(t_tree);
         GetNodeAttribute(t_tree, "aperture", m_cAperture);
         GetNodeAttribute(t_tree, "offset", m_cOffset);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the omnidirectional camera equipped entity \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(COmnidirectionalCameraEquippedEntity);

   /****************************************/
   /****************************************/

}
