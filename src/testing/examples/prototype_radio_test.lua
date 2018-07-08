-- Use Shift + Click to select a robot
-- When a robot is selected, its variables appear in this editor

-- Use Ctrl + Click (Cmd + Click on Mac) to move a selected robot to a different location

-- Put your global variables here

--[[ This function is executed every time you press the 'execute' button ]]
function init()
   reset()
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   robot.leds.set_all_colors(0,0,0)
   if #robot.radios.radio_rx.rx_data > 0 then
      robot.leds.set_all_colors(255,0,0)
      robot.radios.radio_tx.tx_data({robot.radios.radio_rx.rx_data[1][1] + 1})
   end
end



--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   if robot.id == "repeater0" then
      robot.radios.radio_tx.tx_data({0})
   end
end



--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
