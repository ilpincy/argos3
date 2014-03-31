function init()
	reset()
end

function step()
	counter = counter + 1
	if counter < 30 then
		robot.quadrotor.set_position(1,1,3)
		robot.quadrotor.set_yaw(45)
	elseif counter < 60 then
		robot.quadrotor.set_position(-1,1,2)
		robot.quadrotor.set_yaw(90)
	elseif counter < 90 then
		robot.quadrotor.set_position(-1,-1,3)
		robot.quadrotor.set_yaw(135)
	elseif counter < 120 then
		robot.quadrotor.set_position(1,-1,2)
		robot.quadrotor.set_yaw(180)
	end
end

function reset()
	robot.leds.set_all_colors("red")
	counter = 0
end

function destroy()
end
