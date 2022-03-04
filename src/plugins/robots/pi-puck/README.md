# ARGoS Plug-in for the Pi-Puck mobile robot

The Pi-Puck mobile robot is supported in the [ARGoS simulator](https://argos-sim.info/). The plugin in the [ARGoS repository](https://github.com/ilpincy/argos3/tree/master/src/plugins/robots/pi-puck) defines the control interface and provides an implementation of the Pi-Puck in simulation. This implementation provides a visualization for the `qt-opengl` plugin and models for the `dynamics2d` and `dynamics3d` physics engine plugins.

The control interface defined in the ARGoS repository is also used on the real robot. The plugin for the real robot is available in the [argos3-pipuck repository](https://github.com/iridia-ulb/argos3-pipuck). This plugin can compiled as is, however, it is advisable to instead follow the instructions in the [meta-pipuck repository](https://github.com/iridia-ulb/meta-pipuck), which provides a step-by-step guide on how to set up a Docker image that builds a complete, bootable, SD card image for the Pi-Puck that includes support for ARGoS.

## Usage
A Pi-Puck can be added to the ARGoS simulator by adding a `pipuck` tag under the `arena` section of the experiment configuration file as follows:
```xml
<pipuck id="pipuck">
  <body position="0,0,0" orientation="0,0,0"/>
  <controller config="my_controller"/>
</pipuck>
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
To create a Pi-Puck in ARGoS, it must be hosted by either the `dynamics2d` or `dynamics3d` physics engine plugin. The `dynamics2d` model is faster since it essentially models a Pi-Puck as a circle in the XY plane. The `dynamics3d` model is somewhat slower but more realistic. This model actually simulates the wheels of the Pi-Puck as cylindrical bodies that drive the Pi-Puck forwards as a result of the friction between the wheels and the floor of the arena. You can use either physics engine, however, entities in one physics engine will not collide with entities from another. The only exception to this rule are static objects such as the walls of the arena which can be hosted by multiple engines at the same time.

### Media
Media in ARGoS are the indices in which different entities can be looked up based on their current location. These indices predominately enable the sensors of a robot to look up entities of interest. For example, by placing tags or LEDs in their respective indices, they can be detected by a camera on another robot.

The Pi-Puck's simulation model in ARGoS contains LEDs and a crude approximation of wifi whereas messages are simply broadcasted to all nearby robots. To enable the Pi-Puck's LEDs to be seen by another robot, the attribute `led_medium` must be set to a `directional_led` medium declared in the `media` section of the experiment configuration file. Likewise, to enable the sending and receiving of the messages, the attribute `wifi_medium` must be set to a `simple_radio` medium. The following XML provides an example of the required configuration:
```xml
<arena size="1, 1, 1" positional_index="grid" positional_grid_size="1,1,1">
  <pipuck id="pipuck" led_medium="leds" wifi_medium="wifi">
    <body position="0,0,0" orientation="0,0,0"/>
    <controller config="my_controller"/>
  </pipuck>
</arena>

<media>
  <directional_led id="leds" index="grid" grid_size="5,5,5" />
  <simple_radio id="wifi" index="grid" grid_size="5,5,5" />
</media>
```

Note that the LEDs can still appear in the visualization and be controlled without `led_medium` being specified. By contrast, not specifying `wifi_medium` will result in the simple radio (used for approximating wifi) not being added to the robot.

## Sensors
The following section lists the sensors that are specific to the Pi-Puck. There are also a few generic sensors (e.g., `simple_radios` for wifi) that can be added. These sensors are in part discoverable by running `argos3 -q sensors` in a terminal. The examples below make use of the Lua controller in ARGoS which allows writing control software in the form of a Lua script. The advantage of writing controllers in Lua is that there is no need for (cross-)compilation or linking. That being said, all sensors and actuators do provide a C++ interface for which the code itself is the documentation.

### `pipuck_differential_drive`
This sensor is designed to read back the current velocity from the wheels. It's accuracy is inherently tied to the implementation details of the physic model. The sensor can be added to the `sensors` section of a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <pipuck_differential_drive implementation="default" />
  </sensors>
  <params />
</lua_controller>
```

This configuration shows the sensor being added to a Lua controller. With this configuration, the current forwards wheel velocities in centimeters per second can be read from a controller as follows:
```lua
function step()
  log('left speed = ' .. robot.differential_drive.encoders.left)
  log('right speed = ' .. robot.differential_drive.encoders.right)
end
```

This code will write the speeds of the wheels to the ARGoS logger.

### `pipuck_front_camera`
This sensor will detect tags and LEDs in front of the Pi-Puck's front camera. The following configuration adds this sensor to the Pi-Puck.
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <pipuck_front_camera implementation="default"
      rotation="0"
      resolution="640,480"
      principal_point="320,240"
      focal_length="1000,1000"
      tag_medium="tags"
      directional_led_medium="leds"
      show_tag_rays="false"
      show_led_rays="false"
      show_frustum="false" />
  </sensors>
  <params />
</lua_controller>
```

There are many attributes that can be used to configure the front camera sensor although most of them have defaults. The only two attributes which have to be supplied for the sensor to work are the `tag_medium` and the `directional_led_medium`. These attributes specify the indices in which the camera should try and find entities. The remaining attributes are optional and default to the values specified above (note that the principal point defaults to half the resolution).

| Attribute                | Description                                                                                                                      |
|--------------------------|----------------------------------------------------------------------------------------------------------------------------------|
| `rotation`               | Rotation of the Pi-Puck's front camera. Zero degrees of rotation has the camera rotated by -90 degrees as per the PO3030 variant |
| `resolution`             | Resolution that the camera is running at. This only influences the pixel coordinates of the detected tags                        |
| `principal_point`        | An intrinsic parameter of the camera sensor                                                                                      |
| `focal_length`           | An intrinsic parameter of the camera sensor                                                                                      |
| `tag_medium`             | The index in which tags can be found                                                                                             |
| `directional_led_medium` | The index in which LEDs can be found                                                                                             |
| `show_tag_rays`          | Draws the rays from the camera to the corners of a detected tag                                                                  |
| `show_led_rays`          | Draws the rays from the camera to an LED                                                                                         |
| `show_frustum`           | Draw the bounding frustum in which tags and LEDs can be detected                                                                 |

By default the camera is disabled and needs to be enabled as per the following Lua code:
```lua
function init()
  robot.front_camera.enable()
end
```

The detected tags can be iterated over and printed to the ARGoS log as follows:
```lua
function step()
  for i, tag in ipairs(robot.front_camera.tags) do
    log('tag ' .. tag.id)
    log('  position = ' .. tag.position)
    log('  orientation = ' .. tag.orientation)
    log('  corners')
    for j, corner in ipairs(tag.corners) do
      log('    ' .. corner)
    end
  end
end
```

The type of `tag.position` is a `vector3` and its units are in meters. The type of `tag.orientation` is a `quaternion`. The type of the tag corners is `vector2` and the units are in pixels.

To detect an LED, you need to specify the position where you expect the LED to be. This design choice reflects how the camera sensor for the real robot works. The function `detect_led` will return an integer in the range from 1..4 which represents which quadrant of the UV color space in which the color of LED matches. In simulation, these colors can be one of four fixed values `magenta`, `orange`, `green`, or `blue` as rough approximations for the four quadrants of the UV color space. Other colors will not be detected in simulation. The following code demonstrates detecting an LED that is 2 centimeters to the left of a detected tag (ignoring rotation).
```lua
function step()
  tag = robot.front_camera.tags[1]
  if tag ~= nil then
    led = robot.front_camera.detect_led(tag.position - vector3(0, 0.02, 0))
    log('led color is in quadrant ' .. led)
  end
end
```

### `pipuck_ground`
This is a sensor that is capable of reading the brightness of the floor at three locations near the front of the robot.
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <pipuck_ground implementation="default" />
  </sensors>
  <params />
</lua_controller>
```

The following code demonstrates how to use the readings from the sensor in Lua by printing the current readings to the ARGoS log. The following code also shows how to get the position and orientation of each sensor relative to an anchor (a local coordinate system) on the robot.
```lua
function step()
  for i, sensor in ipairs(robot.ground_sensors)
    log('ground sensor ' .. i .. ':')
    log('  reflected = ' .. sensor.reflected)
    log('  background = ' .. sensor.background)
    log('  transform:')
    log('    anchor = ' .. sensor.transform.anchor)
    log('    position = ' .. sensor.transform.position)
    log('    orientation = ' .. sensor.transform.orientation)
  end
end
```

### `pipuck_rangefinders`
The rangefinders sensor is used to detect nearby obstacles around the Pi-Puck. The following configuration demonstrates how to use this sensor in a controller.
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <pipuck_rangefinders implementation="default" />
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

### `pipuck_system`
The system sensor is used to report the current time and temperature of the CPU. It can be added to a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <pipuck_system implementation="default" />
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
The following section lists the actuators that are specific to the Pi-Puck. There are also a few generic actuators (e.g., `simple_radios` for wifi) that can be added. These actuators are in part discoverable by running `argos3 -q actuators` in a terminal. 

### `pipuck_differential_drive`
The differential drive actuator controls the differential drive of the Pi-Puck and can be added to a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators>
    <pipuck_differential_drive implementation="default" />
  </actuators>
  <sensors />
  <params />
</lua_controller>
```

The actuator exports a single function in Lua called `set_linear_velocity` which takes two arguments: the target forwards velocity of the left and right wheel in centimeters per second.

```lua
function step()
  -- drive forwards at 5 centimeters per second
  robot.differential_drive.set_linear_velocity(5, 5)
end
```

### `pipuck_leds`
The LED actuator turns the LEDs of Pi-Puck off and on. It can be added to a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators>
    <pipuck_leds implementation="default" />
  </actuators>
  <sensors />
  <params />
</lua_controller>
```

In the Lua, the following functions are exported by the actuator:

| Function             | Arguments                                                    |
|----------------------|--------------------------------------------------------------|
| `set_ring_led_index` | The index (integer between 1..8) and mode (on/off) (boolean) |
| `set_ring_leds`      | The mode (on/off) (boolean)                                  |
| `set_front_led`      | The mode (on/off) (boolean)                                  |
| `set_body_led`       | The mode (on/off) (boolean)                                  |

For example, the following code turns off all LEDs on the Pi-Puck ring except for the LED with index 5.
```lua
function step()
  robot.leds.set_ring_leds(false)
  robot.leds.set_ring_led_index(5, true)
end
```

