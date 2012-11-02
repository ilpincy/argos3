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
 * @file argos3/core/control_interface/ci_controller.h
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
#include <argos3/core/utility/configuration/memento.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/control_interface/ci_robot.h>

namespace argos {

   class CCI_Controller : public CBaseConfigurableResource,
                          public CMemento {

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

      inline const std::string& GetControllerId() const {
         return m_strControllerId;
      }

      inline void SetControllerId(const std::string& str_controller_id) {
         m_strControllerId = str_controller_id;
      }

      virtual bool IsControllerFinished() const {
         return false;
      }

      virtual void SaveState(CByteArray& c_buffer) {}
      virtual void LoadState(CByteArray& c_buffer) {}

   private:

      CCI_Robot* m_pcRobot;
      std::string m_strControllerId;

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
