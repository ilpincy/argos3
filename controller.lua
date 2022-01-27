function init()
   reset()
end

function reset()
   robot.flight_system.set_target_pose(vector3(0,0,1.8), 0)
   for id, camera in pairs(robot.cameras_system) do
      camera.enable()
   end
   robot.leds.set_led_index(1, "red")
   robot.leds.set_led_index(2, "blue")
   robot.leds.set_led_index(3, "green")
   robot.leds.set_led_index(4, "yellow")
end

function step()
end

function destroy()
end

