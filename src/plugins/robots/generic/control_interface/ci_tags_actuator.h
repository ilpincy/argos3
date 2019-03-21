/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_tags_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_TAGS_ACTUATOR_H
#define CCI_TAGS_ACTUATOR_H

namespace argos {
   class CCI_TagsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_TagsActuator : public CCI_Actuator {

   public:

      typedef std::vector<std::string> TSettings;

   public:

      CCI_TagsActuator() {}

      virtual ~CCI_TagsActuator() {}

      /**
       * @brief Returns the number of tags
       */
      size_t GetNumTags() const;

      /**
       * @brief Sets the payload of a single tag.
       *
       * @param un_tag_number Tag that must be set
       * @param str_payload Payload to set
       */
      virtual void SetSinglePayload(UInt32 un_tag_number,
                                    const std::string& str_payload);
      
      /**
       * @brief Sets all the tags with the same payload.
       *
       * @param str_payload Payload to set
       */
      virtual void SetAllPayloads(const std::string& str_payload);


#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      TSettings m_tSettings;

   };

}

#endif
