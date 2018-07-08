-- Use Shift + Click to select a robot
-- When a robot is selected, its variables appear in this editor

-- Use Ctrl + Click (Cmd + Click on Mac) to move a selected robot to a different location



-- Put your global variables here



--[[ This function is executed every time you press the 'execute' button ]]
function init()
   reset()
end


--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
	if (#robot.cameras.turret_camera.tag_detector > 0) or
      (#robot.cameras.turret_camera.directional_led_detector > 0) or
      (#robot.cameras.turret_camera.led_detector > 0) then
		local message = "turret_camera detects: "
      if(#robot.cameras.turret_camera.tag_detector > 0) then 
         message = message .. "tags, "
      end
      if(#robot.cameras.turret_camera.directional_led_detector > 0) then 
         message = message .. "directional_leds, "
      end
      if(#robot.cameras.turret_camera.led_detector > 0) then 
         message = message .. "leds, "
      end
      log(message)
	end
	if (#robot.cameras.fixed_camera.tag_detector > 0) or
      (#robot.cameras.fixed_camera.directional_led_detector > 0) or
      (#robot.cameras.fixed_camera.led_detector > 0) then
		local message = "fixed_camera detects: "
      if(#robot.cameras.fixed_camera.tag_detector > 0) then 
         message = message .. "tags, "
      end
      if(#robot.cameras.fixed_camera.directional_led_detector > 0) then 
         message = message .. "directional_leds, "
      end
      if(#robot.cameras.fixed_camera.led_detector > 0) then 
         message = message .. "leds, "
      end
      log(message)
	end
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
	robot.turret.set_speed_control_mode()
	robot.turret.set_rotation_speed(10)
	robot.wheels.set_velocity(2,-2)
end



--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
