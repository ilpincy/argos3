function init()
   reset()
end

function step()
   robot.leds.set_single_color(counter+1, "red")
   counter = (counter + 1) % 13
   robot.leds.set_single_color(counter+1, "green")
end

function reset()
   counter = 0
   robot.leds.set_all_colors("red")
	robot.colored_blob_omnidirectional_camera.enable()
	robot.wheels.set_velocity(5,5)
end

function destroy()
end
