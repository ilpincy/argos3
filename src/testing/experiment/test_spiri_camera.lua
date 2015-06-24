function init()
	robot.colored_blob_perspective_camera.enable()
	robot.quadrotor.set_relative_position(0,0,3.5)
end

function step()
	robot.quadrotor.set_relative_yaw(1)
end

function reset()
end

function destroy()
end
