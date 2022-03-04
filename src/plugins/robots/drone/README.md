# ARGoS Plug-in for the IRIDIA drone

The IRIDIA drone is supported in the [ARGoS simulator](https://argos-sim.info/). The plugin in the [ARGoS repository](https://github.com/ilpincy/argos3/tree/master/src/plugins/robots/drone) defines the control interface and provides an implementation of the drone in simulation. This implementation provides a visualization for the `qt-opengl` plugin and a model for the `pointmass3d` physics engine plugin.

The control interface defined in the ARGoS repository is also used on the real robot. The plugin for the real robot is available in the [argos3-drone repository](https://github.com/iridia-ulb/argos3-drone). This plugin can compiled as is, however, it is advisable to instead follow the instructions in the [meta-drone repository](https://github.com/iridia-ulb/meta-drone), which provides a step-by-step guide on how to set up a Docker image that builds a complete, bootable, USB stick for the drone that includes support for ARGoS.

## Usage
A drone can be added to the ARGoS simulator by adding a `drone` tag under the `arena` section of the experiment configuration file as follows:
```xml
<drone id="drone">
  <body position="0,0,0" orientation="0,0,0"/>
  <controller config="my_controller"/>
</drone>
```

### Controllers
In the example above, we referenced a controller called `my_controller`, which must be declared in the experiment configuration file under the `controllers` section, for example:
```xml
<lua_controller id="my_controller">
  <actuators/>
  <sensors />
  <params />
</lua_controller>
```

This controller defines a controller written in Lua, whose script can be passed using the `script` attribute in the `params` tag. The controller cannot do much at the moment since it does not declare any sensors or actuators.

### Physics Engines
To instantiate a drone in ARGoS, an instance of the Point-mass 3D physics engine must be declared. It should be noted that this engine does not support collisions with other objects in the simulation.

### Media
Media in ARGoS are the indices in which different entities can be looked up based on their current location. These indices predominately enable the sensors of a robot to look up entities of interest. For example, by placing tags or LEDs in their respective indices, they can be detected by a camera on another robot.

The drone's simulation model in ARGoS contains LEDs and a crude approximation of wifi whereas messages are simply broadcasted to all nearby robots. To enable the drone's LEDs to be seen by another robot, the attribute `led_medium` must be set to a `directional_led` medium declared in the `media` section of the experiment configuration file. Likewise, to enable the sending and receiving of the messages, the attribute `wifi_medium` must be set to a `simple_radio` medium. The following XML provides an example of the required configuration:
```xml
<arena size="1, 1, 1" positional_index="grid" positional_grid_size="1,1,1">
  <drone id="drone" led_medium="leds" wifi_medium="wifi">
    <body position="0,0,0" orientation="0,0,0"/>
    <controller config="my_controller"/>
  </drone>
</arena>

<media>
  <directional_led id="leds" index="grid" grid_size="5,5,5" />
  <simple_radio id="wifi" index="grid" grid_size="5,5,5" />
</media>
```

Note that the LEDs can still appear in the visualization and be controlled without `led_medium` being specified. By contrast, not specifying `wifi_medium` will result in the simple radio (used for approximating wifi) not being added to the robot.

## Sensors
The following section lists the sensors that are specific to the drone. There are also a few generic sensors (e.g., `simple_radios` for wifi) that can be added. These sensors are in part discoverable by running `argos3 -q sensors` in a terminal. The examples below make use of the Lua controller in ARGoS which allows writing control software in the form of a Lua script. The advantage of writing controllers in Lua is that there is no need for (cross-)compilation or linking. That being said, all sensors and actuators do provide a C++ interface for which the code itself is the documentation.

### `drone_cameras_system`
This sensor will detect tags underneath the drone's four cameras. The following configuration adds this sensor to the drone.
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <drone_camera_system implementation="default"
      tag_medium="tags"
      show_tag_rays="false"
      show_frustum="false" />
  </sensors>
  <params />
</lua_controller>
```

The attributes `show_tag_rays` and `show_frustum` default to false, however, the attribute `tag_medium` must be supplied and be an index in which the camera should try and find tags. By default the cameras are disabled and need to be enabled as per the following Lua code:
```lua
function init()
  for id, camera in pairs(robot.cameras_system) do
    camera.enable()
  end
end
```

The detected tags can be iterated over and printed to the ARGoS log as follows:
```lua
function step()
  for i, camera in pairs(robot.cameras_system) do
    log('camera ' .. camera.id)
    for j, tag in ipairs(robot.front_camera.tags) do
      log('  tag ' .. tag.id)
      log('    position = ' .. tag.position)
      log('    orientation = ' .. tag.orientation)
      log('    corners')
      for k, corner in ipairs(tag.corners) do
        log('      ' .. corner)
      end
    end
  end
end
```

The type of `tag.position` is a `vector3` and its units are in meters. The type of `tag.orientation` is a `quaternion`. The type of the tag corners is `vector2` and the units are in pixels.

### `drone_flight_system`
This sensor represents the input from a Pixhawk device. The sensor can be added to the `sensors` section of a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <drone_flight_system implementation="default" />
  </sensors>
  <params />
</lua_controller>
```

This configuration shows the sensor being added to a Lua controller. Several parameters can be read back from the Pixhawk device (either simulated or real) as shown in the following Lua code:
```lua
function step()
  log('position = ' .. robot.flight_system.position)
  log('orientation = ' .. robot.flight_system.orientation)
  log('target_orientation = ' .. robot.flight_system.target_orientation)
  log('velocity = ' .. robot.flight_system.velocity)
  log('angular_velocity = ' .. robot.flight_system.angular_velocity)
  log('target_position = ' .. robot.flight_system.target_position)
  log('height = ' .. robot.flight_system.height)
  log('battery_voltage = ' .. robot.flight_system.battery_voltage)
  log('temperature = ' .. robot.flight_system.temperature)
end
```

### `drone_rangefinders`
The rangefinders sensor is used to detect nearby obstacles around the drone. The following configuration demonstrates how to use this sensor in a controller.
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <drone_rangefinders implementation="default" />
  </sensors>
  <params />
</lua_controller>
```

The following code demonstrates how to use the readings from the sensor in Lua by printing the current readings to the ARGoS log. The following code also shows how to get the position and orientation of each sensor relative to an anchor (a local coordinate system) on the robot.
```lua
function step()
  for i, sensor in ipairs(robot.rangefinders)
    log('rangefinder ' .. i .. ':')
    log('  proximity = ' .. sensor.proximity)
    log('  illuminance = ' .. sensor.illuminance)
    log('  transform:')
    log('    anchor = ' .. sensor.transform.anchor)
    log('    position = ' .. sensor.transform.position)
    log('    orientation = ' .. sensor.transform.orientation)
  end
end
```

### `drone_system`
The system sensor is used to report the current time and temperature of the CPU. It can be added to a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <drone_system implementation="default" />
  </sensors>
  <params />
</lua_controller>
```

These values are available from a Lua controller and can be printed to the ARGoS log.
```lua
function step()
  log('time = ' .. robot.system.time)
  log('temperature = ' .. robot.system.temperature)
end
```

## Actuators
The following section lists the actuators that are specific to the drone. There are also a few generic actuators (e.g., `simple_radios` for wifi) that can be added. These actuators are in part discoverable by running `argos3 -q actuators` in a terminal. 

### `drone_flight_system`
The drone flight system enables configuring/writing data to a Pixhawk device:
```xml
<lua_controller id="my_controller">
  <actuators>
    <drone_flight_system implementation="default" />
  </actuators>
  <sensors />
  <params />
</lua_controller>
```

This actuator exports the following functions to Lua:

| Function            | Description                                                                                                 |
|---------------------|-------------------------------------------------------------------------------------------------------------|
| `ready`             | Returns a boolean when the Pixhawk is active                                                                |
| `set_armed`         | Takes a boolean to configure whether or not the drone is armed                                              |
| `set_offboard_mode` | Takes a boolean to configure whether or not the drone is in off-board mode                                  |
| `set_target_pose`   | Takes a vector3 and a number that describes the position and orientation to which the drone should fly to with respect to where it took off from |

```lua
function step()
  -- take off and hover at one meter above the ground
  if robot.flight_system.ready() then
    robot.flight_system.set_offboard_mode(true)
    robot.flight_system.set_armed(true)
    robot.flight_system.set_target_pose(vector3(0,0,1)), 0)
  end
end
```

### `drone_leds`
The LED actuator sets the colors of LEDs on the drone. It can be added to a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators>
    <drone_leds implementation="default" />
  </actuators>
  <sensors />
  <params />
</lua_controller>
```

In the Lua, the following functions are exported by the actuator:

| Function        | Arguments                                                    |
|-----------------|--------------------------------------------------------------|
| `set_led_index` | The index (integer between 1..4) and the color (either a string or three RGB colors) |
| `set_leds`      | the color (either a string or three RGB colors) |

For example, the following code turns off all LEDs on the drone except for LED group with index 1 which is set to red.
```lua
function step()
  robot.leds.set_leds('black')
  robot.leds.set_led_index(1, 'red')
end
```

