function init()
   io.write("init");
   robot.wheels.left = 5
   robot.wheels.right = 5
   robot.leds[6].red = 255
   robot.leds[7].green = 255
end

function step()
   io.write("step");
end

function reset()
   io.write("reset");
end

function destroy()
   io.write("destroy");
end
