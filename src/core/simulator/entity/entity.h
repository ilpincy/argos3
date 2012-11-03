/**
 * @file core/simulator/entity/entity.h
 *
 * @brief This file contains the definition of an entity.
 *
 * This file contains the class definition of an
 * entity, that is, the basic class that provides the interface for the
 * simulation of all the objects in the environment.
 *
 * @author Carlo Pinciroli - <cpinciroli@ulb.ac.be>
 */

#ifndef ENTITY_H
#define ENTITY_H

namespace argos {
   class CEntity;
   class CPhysicsEngine;
   class CSpace;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/configuration/base_configurable_resource.h>

#include <vector>
#include <map>
#include <string>

namespace argos {

   class CEntity : public CBaseConfigurableResource {

   public:

      typedef std::vector<CEntity*> TVector;
      typedef std::map<std::string, CEntity*> TMap;

   public:

      CEntity(CEntity* pc_parent) :
         m_pcParent(pc_parent) {}
      virtual ~CEntity() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset() {}
      virtual void Destroy() {}

      inline virtual const std::string& GetId() const {
         return m_strId;
      }

      inline virtual void SetId(const std::string& str_id) {
         m_strId = str_id;
      }

      inline bool HasParent() {
         return (m_pcParent != NULL);
      }

      inline CEntity& GetParent() {
         return *m_pcParent;
      }

      inline void SetParent(CEntity& c_parent) {
         m_pcParent = &c_parent;
      }

      virtual std::string GetTypeDescription() const = 0;

   private:

      std::string m_strId;
      CEntity* m_pcParent;

   };

}

#define REGISTER_ENTITY(CLASSNAME,                  \
                        LABEL,                      \
                        AUTHOR,                     \
                        VERSION,                    \
                        BRIEF_DESCRIPTION,          \
                        LONG_DESCRIPTION,           \
                        STATUS)                     \
   REGISTER_SYMBOL(CEntity,                         \
                   CLASSNAME,                       \
                   LABEL,                           \
                   AUTHOR,                          \
                   VERSION,                         \
                   BRIEF_DESCRIPTION,               \
                   LONG_DESCRIPTION,                \
                   STATUS)

#endif
