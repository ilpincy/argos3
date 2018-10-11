-- global variables
local _state
local _manipulator_state
local _manipulator_target

function init()
	-- set global variables
	_state = "begin"
	_manipulator_state = "idle"
	_manipulator_target = 0.0275
	set_speed(0,0)
end

function reset()
	-- just recall init
	init()
end

function step()
	-- step the manipulator control loop
	_manipulator_state = step_manipulator()
	-- step the finite state machine
	if _state == "begin" then
		set_speed(3,3)
		_state = "forwards"
	elseif _state == "forwards" then
		if robot.proximity[15] > 0.99 and robot.proximity[16] > 0.99 then			
			set_speed(0,0)
			_manipulator_target = 0
			_state = "lower_manipulator"
		end
	elseif _state == "lower_manipulator" then
		if _manipulator_state == "idle" then
			set_electromagnet_current(1)
			_state = "attach_block"
		end
	elseif _state == "attach_block" then
		if robot.proximity[14] > 0.999 then
			set_electromagnet_current(0)
			_manipulator_target = 0.0825		 
			_state = "raise_manipulator"
		end		
	elseif _state == "raise_manipulator" then
		if _manipulator_state == "idle" then
			set_electromagnet_current(0)
			set_speed(3,3)
			robot.radios.radio.tx_data({1});
			_state = "continue_forwards"
		end
	elseif _state == "continue_forwards" then
		if robot.proximity[6] > 0.98 and robot.proximity[7] > 0.98 then
			set_electromagnet_current(-1)
			set_speed(0,0)
			_state = "detach_block"
		end
	elseif _state == "detach_block" then
		if robot.proximity[14] < 0.99 then
			set_electromagnet_current(0)
			set_speed(-1,-1)
			_manipulator_target = 0.11
			_state = "reverse"
		end		
	elseif _state == "reverse" then
		if robot.proximity[6] < 0.90 and robot.proximity[7] < 0.90 then
			set_speed(-0.125,-0.125)
			_state = "end"
		end
	end
end

function set_speed(left, right)
	robot.joints.lower_chassis_wheel_bl.set_target(left)
	robot.joints.lower_chassis_wheel_fl.set_target(left)
	robot.joints.lower_chassis_wheel_br.set_target(-right)
	robot.joints.lower_chassis_wheel_fr.set_target(-right)
end

function set_electromagnet_current(target)
	robot.magnets.magnet_0.set_current(target)
	robot.magnets.magnet_1.set_current(target)
	robot.magnets.magnet_2.set_current(target)
	robot.magnets.magnet_3.set_current(target)
end

function step_manipulator()
	local position = robot.joints.lift_fixture_vertical_link.encoder
	local set_velocity = robot.joints.lift_fixture_vertical_link.set_target
	local manipulator_speed = 0.03

	if math.abs(_manipulator_target - position) < 0.01 then
		manipulator_speed = math.abs(_manipulator_target - position) * 3
	end

	if math.abs(_manipulator_target - position) > 0.005 then
		if _manipulator_target > position then
			set_velocity(manipulator_speed)
		else
			set_velocity(-manipulator_speed)
		end
		return "active"
	else
		set_velocity(0.0)
		return "idle"
	end
end
