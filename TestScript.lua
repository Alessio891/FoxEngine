
--[[
	Each component script should have a unique name.
	Each component script will be a subclass of SceneObjectComponent
	Each subclass will be named with the name of the file
--]]
TestScript.Type = "My Script"
TestScript.ScriptType = ScriptType.ObjectComponent
--[[ 
	The Data table is used to expose variable to the editor inspector.
	Metadata can be specified to modify the behavior of each variable
--]]
TestScript.Data = {
	["MinZ"] = {
		Name = "Minimum Z",
		Type = "float",
		Min = -5.0,
		Max = 5.0,
		FloatSpeed = 0.2,
		Value = 0.0
	},
	["MaxZ"] = {
		Name = "Maximum Z",
		Type = "float",
		Min = -5.0,
		Max = 5.0,
		FloatSpeed = 0.2,
		Value = 0.0	
	},
	["Alpha"] = {
		Name = "Percentage",
		Type = "float",
		Min = 0.0,
		Max = 1.0,
		FloatSpeed = 0.01,
		Value = 0.0
	}
}

-- Called when the component is added to a scene object
function TestScript:Begin()
end

-- Called each frame
function TestScript:Tick(delta, other)
	--[[
		A good practice is to get a reference to the data table and each variable value
		for easy access in the code.
		Be careful that access to [ThisClass].Object is denied until the Begin method has been called.
	--]]

	local min = TestScript.Data["MinZ"].Value
	local max = TestScript.Data["MaxZ"].Value
	local alpha = TestScript.Data["Alpha"].Value

	local z = Lerp(min, max, alpha)
	TestScript.Object.Transform.Position.z = z

end

-- Called when the component is removed or the object is destroyed
function TestScript:End()
end