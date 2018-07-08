state = "begin"
timer = 0

function init()
   logerr("[NOTE] This example requires recalibration and is disabled")
	reset()
end

function reset()
	state = "begin"
   timer = 0
   set_speed(0,0)
   regulate_end_effector_position(0)
end

function step()
	-- update timer variable
	if(timer > 0) then
		timer = timer - 1
	end
	-- run finite state machine
	if state == "begin" then
		state = 	"move_forwards"
		set_speed(1,1)
	elseif state == "move_forwards" then
		regulate_end_effector_position(0.1)
		if(robot.proximity[6] > 0.98 and robot.proximity[7] > 0.98) then
			state = "lower_end_effector"			
			set_speed(0,0)
		end
	elseif state == "lower_end_effector" then
		regulate_end_effector_position(0.005)
		if(robot.proximity[14] > 0.99) then
			state = "pick_up_block"
			robot.joints.lift_fixture_vertical_link.set_target(0.0)
   		robot.magnets.magnet_0.set_current(1)
	   	robot.magnets.magnet_1.set_current(1)
			robot.magnets.magnet_2.set_current(1)
			robot.magnets.magnet_3.set_current(1)
			timer = 20;
		end
	elseif state == "pick_up_block" then
		if(timer == 0) then
			state = "raise_block"
   		robot.magnets.magnet_0.set_current(0.5)
	   	robot.magnets.magnet_1.set_current(0.5)
			robot.magnets.magnet_2.set_current(0.5)
			robot.magnets.magnet_3.set_current(0.5)
		end
	elseif state == "raise_block" then
		regulate_end_effector_position(0.0825)
		if(robot.joints.lift_fixture_vertical_link.encoder > 0.060) then
			state = "approach_block"
			robot.radios.radio.tx_data({1});
			set_speed(0.5,0.5)
		end
	elseif state == "approach_block" then
		regulate_end_effector_position(0.0825)
		if(robot.proximity[6] > 0.98 and robot.proximity[7] > 0.98) then
			state = "release_block"
   		robot.magnets.magnet_0.set_current(-0.5)
	   	robot.magnets.magnet_1.set_current(-0.5)
			robot.magnets.magnet_2.set_current(-0.5)
			robot.magnets.magnet_3.set_current(-0.5)
			set_speed(0,0)
		end
	end
end

function set_speed(left, right)
	robot.joints.lower_chassis_wheel_bl.set_target(left)
	robot.joints.lower_chassis_wheel_fl.set_target(left)
	robot.joints.lower_chassis_wheel_br.set_target(-right)
	robot.joints.lower_chassis_wheel_fr.set_target(-right)
end

function regulate_end_effector_position(target)
	local position = robot.joints.lift_fixture_vertical_link.encoder
	local set_velocity = robot.joints.lift_fixture_vertical_link.set_target
	if math.abs(target - position) > 0.005 then
		set_velocity(target - position)
	else
		set_velocity(0.0)
	end
end
