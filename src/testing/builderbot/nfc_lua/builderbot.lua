function init()
   robot.simple_radios.nfc.send({'ping'})
end

function step()
   for index, message in ipairs(robot.simple_radios.nfc.recv) do
      if message[1] == 'ping' then
         robot.simple_radios.nfc.send({'pong'})
      elseif message[1] == 'pong' then
         robot.simple_radios.nfc.send({'ping'})
      end
   end
end

function reset()
end

function destroy()
end

