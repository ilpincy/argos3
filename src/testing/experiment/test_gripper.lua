counter = 0

function init()
   reset()
end

function step()
   counter = counter + 1
   if counter == 50 then
      robot.wheels.left = -5
      robot.wheels.right = -5
   else if counter == 100 then
      robot.gripper.aperture = 0
   end
   end
end

function reset()
   counter = 0
   robot.gripper.aperture = math.pi / 2
   robot.wheels.left = 5
   robot.wheels.right = 5
end

function destroy()
end
