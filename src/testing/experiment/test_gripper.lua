counter = 0

function init()
   reset()
end

function step()
   counter = counter + 1
   if counter == 50 then
      robot.wheels.set_velocity(-5, -5)
   else if counter == 100 then
      robot.gripper.unlock()
   end
   end
end

function reset()
   counter = 0
   robot.gripper.lock_positive()
   robot.wheels.set_velocity(5, 5)
end

function destroy()
end
