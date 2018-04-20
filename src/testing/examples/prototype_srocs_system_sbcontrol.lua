function init()
   reset();  
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
	for identifer,radio in pairs(robot.radios) do
		if (#radio.rx_data >= 1) then
			robot.directional_leds.set_all_colors("red");
		end
	end
end

function reset()
   robot.directional_leds.set_all_colors("blue");
end

function destroy()
   -- put your code here
end
