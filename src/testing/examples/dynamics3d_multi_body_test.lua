-- Use Shift + Click to select a robot
-- When a robot is selected, its variables appear in this editor

-- Use Ctrl + Click (Cmd + Click on Mac) to move a selected robot to a different location



-- Put your global variables here



--[[ This function is executed every time you press the 'execute' button ]]
function init()
	reset()
   log("[NOTE] This example demonstrates a four wheeled vehicle. The vehicle appears to slip at the moment and requires some further tuning of internal parameters");
end



--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
	log("wheel velocities: " ..
		 string.format("%.2f", robot.joints.base_wheel_bl.encoder) .. ", " ..
		 string.format("%.2f", robot.joints.base_wheel_fl.encoder) .. ", " ..
		 string.format("%.2f", robot.joints.base_wheel_br.encoder) .. ", " ..
		 string.format("%.2f", robot.joints.base_wheel_fr.encoder))
end



--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
	robot.joints.base_wheel_bl.set_target(1)
	robot.joints.base_wheel_fl.set_target(1)
	robot.joints.base_wheel_br.set_target(1)
	robot.joints.base_wheel_fr.set_target(1)
end



--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
