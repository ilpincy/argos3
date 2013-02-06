function init()
   io.write("init");
   footbot.wheel_speed.left = 5
   footbot.wheel_speed.right = 5
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
