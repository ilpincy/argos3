-- Use Shift + Click to select a robot
-- When a robot is selected, its variables appear in this editor

-- Use Ctrl + Click (Cmd + Click on Mac) to move a selected robot to a different location



-- Put your global variables here



--[[ This function is executed every time you press the 'execute' button ]]
function init()
	robot.turret.set_speed_control_mode()
	robot.turret.set_rotation_speed(1)
	robot.wheels.set_velocity(0.125,-0.125)
end



--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
	if #robot.cameras.turret_camera.tag_detector > 0 then
		log("turret_camera detects tags")
	end
	if #robot.cameras.turret_camera.led_detector > 0 then
		log("turret_camera detects LEDs")
	end

end



--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   -- put your code here
end



--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
