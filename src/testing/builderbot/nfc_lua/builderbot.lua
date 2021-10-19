function init()
   robot.radios.nfc.send({'ping'})
end

function step()
   for index, message in ipairs(robot.radios.nfc.recv) do
      if message[1] == 'ping' then
         robot.radios.nfc.send({'pong'})
      elseif message[1] == 'pong' then
         robot.radios.nfc.send({'ping'})
      end
   end
end

function reset()
end

function destroy()
end

