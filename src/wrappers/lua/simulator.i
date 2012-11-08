%module argos3_simulator
%{
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/space_hash.h>
#include <argos3/core/simulator/space/space_multi_thread_h_dispatch.h>
#include <argos3/core/simulator/space/space_hash_native.h>
#include <argos3/core/simulator/space/space_multi_thread_scatter_gather.h>
#include <argos3/core/simulator/space/space_no_threads.h>
#include <argos3/core/simulator/entity/medium_entity.h>
#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/entity/wifi_medium_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <argos3/core/simulator/argos_command_line_arg_parser.h>
#include <argos3/core/simulator/visualization/visualization.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/core/simulator/physics_engine/physics_engine_entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/actuator.h>
%}
