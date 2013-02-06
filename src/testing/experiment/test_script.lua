function init()
   io.write("init");
   footbot.wheel_speed.left = 5
   footbot.wheel_speed.right = 5
   footbot.leds[6].red = 255
   footbot.leds[7].green = 255
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
