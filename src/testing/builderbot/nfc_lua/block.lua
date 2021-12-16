function init()
   robot.simple_radios.south.send({'ping'})
end

function step()
   for face, radio in pairs(robot.simple_radios) do
      for index, message in ipairs(radio.recv) do
	 if message[1] == 'ping' then
            radio.send({'pong'})
         elseif message[1] == 'pong' then
            radio.send({'ping'})
         end
      end
   end
end

function reset()
end

function destroy()
end

