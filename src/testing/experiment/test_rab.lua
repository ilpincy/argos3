function init()
   if robot.id == "fb0" then
      log("fb0")
      robot.range_and_bearing.set_data(1, 17)
   else if robot.id == "fb1" then
      log("fb1")
      robot.range_and_bearing.set_data(2, 19)
   end
   end
end

function step()
end

function reset()
end

function destroy()
end
