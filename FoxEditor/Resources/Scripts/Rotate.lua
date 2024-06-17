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
		Speed = 0.01,
		Min = -0.3,
		Max = 2.0
	},
	["RotationAxis"] = {
		Type = "vector",
		Value = Vector3F.new(0.0,1.0,0.0)
	}
}

-- Variables bound to the Data Table can't be directly modified (not for more than one loop anyway)
Speed = 1.2
RotationAxis = Vector3F.new(0.0, 1.0, 0.0)

-- Variables not bound to the Data Table will be reset to the default value at each recompile\start
OtherSpeed = 0.1 


function Tick(delta)
	Object.Transform.Rotation.y = Object.Transform.Rotation.y + Speed * delta * RotationAxis.y
	Object.Transform.Rotation.x = Object.Transform.Rotation.x + Speed * delta * RotationAxis.x
	Object.Transform.Rotation.z = Object.Transform.Rotation.z + Speed * delta * RotationAxis.z
end

function OnDrawGUI(delta)

end