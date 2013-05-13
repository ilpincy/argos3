/**
 * @file <argos3/core/simulator/medium/medium.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MEDIUM_H
#define MEDIUM_H

namespace argos {
   class CMedium;
}

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   class CMedium : public CBaseConfigurableResource {

   public:

      typedef std::vector<CMedium*> TVector;
      typedef std::map<std::string, CMedium*> TMap;

   public:

      CMedium() {}
      virtual ~CMedium() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset() {}
      virtual void Destroy() {}
      virtual void Update() = 0;

      inline const std::string& GetId() const {
         return m_strId;
      }

      void SetId(const std::string& str_id) {
         m_strId = str_id;
      }

   private:
               
      /** The medium's id. */
      std::string m_strId;

   };

}

#define REGISTER_MEDIUM(CLASSNAME,                  \
                        LABEL,                      \
                        AUTHOR,                     \
                        VERSION,                    \
                        BRIEF_DESCRIPTION,          \
                        LONG_DESCRIPTION,           \
                        STATUS)                     \
   REGISTER_SYMBOL(CMedium,                         \
                   CLASSNAME,                       \
                   LABEL,                           \
                   AUTHOR,                          \
                   VERSION,                         \
                   BRIEF_DESCRIPTION,               \
                   LONG_DESCRIPTION,                \
                   STATUS)

#endif
