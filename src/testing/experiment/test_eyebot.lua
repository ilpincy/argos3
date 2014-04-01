--[[
	EYEBOT TEST CONTROLLER

	Lets a robot draw a circle in the space

	At the beginning of the experiment, it takes the current position
	as the circle center using the positioning sensor

	Then it moves along X for 1 meter (radius of the circle), at
 	altitude 3 meters

	Then it moves along the circle for two complete rounds

	And finally it goes back to the center
  ]]

-- Used as internal counter to draw the circle
-- The equation of a circle is:
-- x = R cos t
-- y = R cos t
t = 0

-- Circle radius measured in meters
R = 1

--[[ Vector 3 math ]]

-- Vector3 namespace and metatable
vec3 = {}
vec3_mt = {}
vec3.__index = vec3_mt

function vec3:new(x, y, z)
	setmetatable({ x = x, y = y, z = z }, vec3_mt)
end

function vec3:add(v)
	x = x + v.x
	y = y + v.y
	z = z + v.z
end

-- Takes off the robot
function take_off()
end

-- Moves the robot away from the circle center
function leave_center()
end

-- Moves the robot along the circle
function move_along_circle()
end

-- Moves the robot back to the center
function return_to_center()
end

function init()
	-- reset state
	reset()
end

function step()
	robot.quadrotor.set_position()
end

function reset()
	-- switch all LEDs to red
	robot.leds.set_all_colors("red")
	-- zero the internal counter
	t = 0
end

function destroy()
end
