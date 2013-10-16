

--[[ This function is executed every time you press the 'execute'
     button ]]
function init()
   robot.rotors.set_velocity(VELOCITY,VELOCITY,VELOCITY,VELOCITY)
end



--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
end



--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
end



--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
end
