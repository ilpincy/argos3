count = 1

function init()
   reset()
end

function step()
   robot.leds.set_all_colors("black")
   robot.leds.set_single_color(count, "green")
   -- update count
   count = count + 1
   if count > 8 then
      count = 1
   end  
end

function reset()
   robot.leds.set_all_colors("black")
	robot.wheels.set_velocity(5,5)
   count = 1
end

function destroy()
end
