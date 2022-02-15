# ARGoS Support

The Pi-Puck mobile robot is supported in the ARGoS simulator.[^1] The plugin in the main repository defines the control interface and provides an implementation of the Pi-Puck in simulation. The implementation provides a visualisation for the `qt-opengl` plugin and models for the `dynamics2d` and `dynamics3d` physics engine plugins.

[^1]: The ARGoS website `https://argos-sim.info/`.

The control interface defined in the main repository is also used on the real robot. The plugin for the real robot is available in the argos3-pipuck repository[^2] on the IRIDIA-ULB Github organization. This plugin can compiled as is, however, it is advisable to instead follow the instructions in the meta-pipuck repository,[^3] which give a step-by-step guide on how to set up a Docker image which builds a complete, bootable, SD card image for the Pi-Puck and that also includes support for ARGoS.

[^2]: Repository for ARGoS3 Pi-Puck: `https://github.com/iridia-ulb/argos3-pipuck`
[^3]: Repository for the Pi-Puck's Yocto Layer: `https://github.com/iridia-ulb/meta-pipuck`

## Usage
A Pi-Puck can be added to the ARGoS simulator by adding a `pipuck` tag under the `arena` section of the experiment configuration file as follows:
```xml
<pipuck id="pipuck">
  <body position="0,0,0" orientation="0,0,0"/>
  <controller config="my_controller"/>
</pipuck>
```

### Controllers
In the example above, we referenced a controller called `my_controller`, which must be delcared in the experiment configuration file under the `controllers` section, for example:
```xml
<lua_controller id="my_controller">
  <actuators/>
  <sensors />
  <params />
</lua_controller>
```
This controller defines a controller written in Lua, whose script can be passed using the `script` attribute in the `params` tag. The controller cannot do much at the moment since it does not declare any sensors or actuators.

### Physics Engines
To create in Pi-Puck in ARGoS, it must be hosted by either the `dynamics2d` or `dynamics3d` physics engine plugins. The `dynamics2d` model is faster since it essentially models a Pi-Puck has a circle in the XY plane. The `dynamics3d` model is somewhat slower but more realistic. This model actually simulates the wheels of the Pi-Puck as cylinderical bodies that drive the Pi-Puck forwards as a result of the friction between the wheels and the floor of the arena. You can use either physics engine, however, entities in one physics engine will not collide with entities from another. The only exception to this rule are static objects such as the walls of the arena which can be hosted by multiple engines at the same time.

### Media
Media in ARGoS are the indices in which different entities can be looked up based on their current location. These indices predominately enable the sensors of a robot to look up entities of interest. For example, by placing tags or LEDs in their respective indices, they can be detected by a camera on another robot.

The Pi-Puck's simulation model in ARGoS contains LEDs and a crude approximation of wifi whereas messages are simply broadcasted to all nearby robots. To enable the Pi-Puck's LEDs to be seen by another robot, the attribute `led_medium` must be set to a `directional_led` medium declared in the `media` section of the experiment configuration file. Likewise, to enable the sending of receiving of the messages, the attribute `wifi_medium` must be set to a `simple_radio` medium. The following XML provides an example of the required configuration:
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
Note that the LEDs can still appear in the visualisation and be controlled without `led_medium` being specified. By contrast, not specifying `wifi_medium` will result in the simple radio (used for approximating wifi) not being added to the robot.

## Sensors
The following section lists the sensors that are specific the Pi-Puck. There are also a few generic sensors that can be added which are discoverable by running `argos3 -q sensors` in a terminal.

### `pipuck_differential_drive`
This sensor is designed to read back the current velocity from the wheels. It's accuracy is inheritantly tied to the implementation details of the physic model. The sensor can be added to the `actuators` section of a controller as follows:
```xml
<lua_controller id="my_controller">
  <actuators />
  <sensors>
    <pipuck_differential_drive implementation="default" />
  </sensors>
  <params />
</lua_controller>
```
This configuration shows the sensor being added to a Lua controller. With this configuration, the current fowards wheel velocities in centimeters per second can be read from a controller as follows:
```lua
function step()
  log('left speed = ' .. robot.differential_drive.encoders.left)
  log('right speed = ' .. robot.differential_drive.encoders.right)
end
```
This code will write the speeds of the wheels to the ARGoS logger.

## Actuators

