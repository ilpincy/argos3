function init()
   reset()
end

function step()
	-- increase counter
   counter = counter + 1
	-- for counter < 50, just go straight with gripper locked
	-- this way, as soon as you get the cylinder, it sticks to the gripper
	-- the object is actually reached after 32 steps, so initially the robot pushes the object forwards
	-- when 50 <= counter < 100 go backwards while gripping the cylinder
	-- when counter >= 100 release the object and keep going backwards
   if counter == 50 then
      robot.wheels.set_velocity(-5, -5)
   elseif counter == 100 then
      robot.gripper.unlock()
   end
end

function reset()
   counter = 0
   robot.gripper.lock_positive()
   robot.wheels.set_velocity(5, 5)
end

function destroy()
end
