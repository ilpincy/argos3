function init()
   reset()
end

function reset()
   robot.electromagnet_system.set_discharge_mode('disable')
   robot.lift_system.set_position(0.0275)
   state = 'raise_manipulator'
end

function step()
   if state == 'raise_manipulator' then
      if robot.lift_system.state == 'inactive' then
         robot.differential_drive.set_target_velocity(0.01, -0.01)
         state = 'drive_forward'
      end
   elseif state == 'drive_forward' then
      if robot.rangefinders.left.proximity + robot.rangefinders.right.proximity < 0.01 then
         robot.differential_drive.set_target_velocity(0, 0)
         robot.lift_system.set_position(0.0025)
         state = 'lower_manipulator_precharge'
      end
   elseif state == 'lower_manipulator_precharge' then
      if robot.lift_system.state == 'inactive' then
         if robot.electromagnet_system.voltage > 21 then
            state = 'discharge'
         end
      end
   elseif state == 'discharge' then
      if robot.rangefinders.underneath.proximity < 0.005 then
         robot.electromagnet_system.set_discharge_mode('disable')
         robot.lift_system.set_position(0.1375)
         state = 'raise_block'
      end
   end
end

function destroy()
end

