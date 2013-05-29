Account = {}
Account.__index = Account

function Account.create(balance)
  local acnt = {}             -- our new object
  setmetatable(acnt,Account)  -- make Account handle lookup
  acnt.balance = balance      -- initialize our object
  return acnt
end

function init() end
function step() end
function reset() end
function destroy() end
