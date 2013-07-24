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

      /**
       * Initialized the medium.
       * By default, this method parses the medium id.
       * The order in which initialization takes place is:
       * <ul>
       * <li>CMedia::Init()
       * <li>CSpace::Init()
       * <li>CMedia::PostSpaceInitInit()
       * </ul>
       * @param t_tree the base of the XML configuration tree to parse
       * @throw CARGoSException if an error occurs
       * @see PostSpaceInitInit()
       * @see CSpace::Init()
       */
      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset() {}
      virtual void Destroy() {}

      /**
       * Executes extra initialization activities after the space has been initialized.
       * By default, this method does nothing.
       * The order in which initialization takes place is:
       * <ul>
       * <li>CMedia::Init()
       * <li>CSpace::Init()
       * <li>CMedia::PostSpaceInitInit()
       * </ul>
       * @throw CARGoSException if an error occurs
       * @see Init()
       * @see CSpace::Init()
       */
      virtual void PostSpaceInit() {}

      /**
       * Updates the state of this medium.
       */
      virtual void Update() = 0;

      /**
       * Returns the id of this medium.
       * @return The id of this medium.
       */
      inline const std::string& GetId() const {
         return m_strId;
      }

      /**
       * Sets the id of this medium.
       * @param str_id The id of this medium.
       */
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
