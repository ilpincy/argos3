/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file core/control_interface/ci_controller.h
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_CONTROLLER_H
#define CCI_CONTROLLER_H

namespace argos {
   class CCI_Controller;
}

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/control_interface/ci_robot.h>

namespace argos {

   class CCI_Controller : public CBaseConfigurableResource {

   public:

      virtual ~CCI_Controller() {
         if (m_pcRobot != NULL) delete m_pcRobot;
      }

      virtual void Init(TConfigurationNode& t_node) {}

      virtual void ControlStep() {}

      virtual void Reset() {}

      virtual void Destroy() {}

      inline CCI_Robot& GetRobot() {
         return *m_pcRobot;
      }

      inline void SetRobot(CCI_Robot& pc_robot) {
         m_pcRobot = &pc_robot;
      }

      inline const std::string& GetId() const {
         return m_strId;
      }

      inline void SetId(const std::string& str_id) {
         m_strId = str_id;
      }

      virtual bool IsControllerFinished() const {
         return false;
      }

   private:

      CCI_Robot* m_pcRobot;
      std::string m_strId;

   };

}

#define REGISTER_CONTROLLER(CLASSNAME, LABEL) \
   REGISTER_SYMBOL(CCI_Controller,            \
                   CLASSNAME,                 \
                   LABEL,                     \
                   "undefined",               \
                   "undefined",               \
                   "undefined",               \
                   "undefined",               \
                   "undefined")

#endif
