-- create an empty table for a 2d vector
vec2 = {}

-- vec2 constructor
function vec2:new_xy(x, y)
   local obj = {x = x, y = y}
   setmetatable(obj, {__index = vec2})
   return obj
end

-- vec2 constructor from length and angle
function vec2:new_la(length, angle)
   local obj = {x = length*math.cos(angle), y = length*math.sin(angle)}
   setmetatable(obj, {__index = vec2})
   return obj
end

-- vec2 sum
function vec2:add(v)
   self.x = self.x + v.x
   self.y = self.y + v.y
end

-- vec2 scale
function vec2:scale(s)
   self.x = self.x / s
   self.y = self.y / s
end

-- vec2 angle
function vec2:angle()
   return math.atan2(self.y, self.x)
end

-- vec2 length
function vec2:length()
   return math.sqrt(self.x^2 + self.y^2)
end

-- Constants
straight_range = { min = -10 * math.pi/180,
                   max =  10 * math.pi/180 }
delta = 0.1

-- Controller init
function init()
end

-- Controller step
function step()
   accum = vec2:new_xy(0, 0)
   vec = {}
   for i = 1,24 do
      vec[i] = vec2:new_la(robot.proximity[i].value,
                           robot.proximity[i].angle)
      accum:add(vec[i])
   end
   angle = accum:angle()
   length = accum:length() / 24
   if angle > straight_range.min and
      angle < straight_range.max and
      length < delta then
      robot.wheels.set_velocity(5,5)
   else
      if angle > 0 then
         robot.wheels.set_velocity(5,0)
      else
         robot.wheels.set_velocity(0,5)
      end
   end
end

-- Controller reset
function reset()
end

-- Controller destroy
function destroy()
end
