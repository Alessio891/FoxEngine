--[[-
The Data table is used to expose variables to the editor.
Each entry of the table will be bound to a variable with the same name.
Keep in mind three main concepts:
1. Changes to the *variable* in the code will be reset on the next loop
2. Changes to the *Data Variable Value* will reflect to the variable
3. The declaration of the variable is just for convenience, since the value can still be accessed through Data["variable_name"].Value regardless
-]]
Data = {
	["Speed"] = {
		Name = "Speed",
		Value = 1.0,
		Type = "float",
		FloatSpeed = 0.01,
		Min = -0.3,
		Max = 2.0
	}
}

-- Variables bound to the Data Table can't be directly modified (not for more than one loop anyway)
Speed = 1.2

-- Variables not bound to the Data Table will be reset to the default value at each recompile
OtherSpeed = 0.1 

ascending = true

function Tick(delta)
	if IsKeyDown(Key.H) then
		-- Use Set(varName, value) to correctly set a bound value
		Set("Speed", Get("Speed") + 0.1 * delta)
	end
	Object.Transform.Rotation.y = Object.Transform.Rotation.y + Speed * delta
	Object.Transform.Rotation.x = Object.Transform.Rotation.x + OtherSpeed * delta

	if ascending then
		Object.Transform.Position.y = Object.Transform.Position.y + Speed * delta
		if Object.Transform.Position.y > 4 then ascending = false end
	else
		Object.Transform.Position.y = Object.Transform.Position.y - Speed * delta
		if Object.Transform.Position.y < 0 then ascending = true end
	end
end

function OnDrawGUI(delta)

end