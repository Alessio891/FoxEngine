
--[[
	Each component script should have a unique name.
	Each component script will be a subclass of SceneObjectComponent
	Each subclass will be named with the name of the file
--]]
MyFirstScript.Type = "My Script"
MyFirstScript.ScriptType = ScriptType.ObjectComponent
--[[ 
	The Data table is used to expose variable to the editor inspector.
	Metadata can be specified to modify the behavior of each variable
--]]
MyFirstScript.Data = {
	["Speed"] = {
		Name = "Speed",
		Type = "float",
		Value = 0.5,
		FloatSpeed = 0.1
	},
	["Alpha"] = {
		Name = "Alpha",
		Type = "float",
		Value = 0.0,
		FloatSpeed = 0.1,
		Min = 0.0,
		Max = 1.0
	}
}

-- Called when the component is added to a scene object
function MyFirstScript:Begin()
	FoxLog("My owner is " ..MyFirstScript.Object.Name)
	FoxLog("My Type is " ..MyFirstScript.ScriptType)
end

-- Called each frame
function MyFirstScript:Tick(delta, other)
	--[[
		A good practice is to get a reference to the data table and each variable value
		for easy access in the code.
		Be careful that access to [ThisClass].Object is denied until the Begin method has been called.
	--]]
	local data = MyFirstScript.Data
	local speed = data["Speed"].Value
	local alpha = data["Alpha"].Value

	if IsKeyDown(Key.H) then
		data["Alpha"].Value = data["Alpha"].Value + 0.1 * delta
		if (data["Alpha"].Value > 1.0) then data["Alpha"].Value = 1.0 end
	end
	if IsKeyDown(Key.L) then
		data["Alpha"].Value = data["Alpha"].Value - 0.1 * delta
		if (data["Alpha"].Value < 0.0) then data["Alpha"].Value = 0.0 end
	end
	local rot = MyFirstScript.Object.Transform.Rotation	
	local pos = MyFirstScript.Object.Transform.Position	
	rot.y = rot.y + 0.1 * delta * alpha
	c = Vector3F.new()
	c.x = Lerp(0, 0.5, alpha)
	c.y = Lerp(0.4, 0.9, alpha)
	c.z = Lerp(0.2, 0.3, alpha)

	MyFirstScript.Object.Renderer.Color = c
	pos.x = 0
	

end

-- Called when the component is removed or the object is destroyed
function MyFirstScript:End()
end