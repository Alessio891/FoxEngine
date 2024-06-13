
--[[
	Each component script should have a unique name.
	Each component script will be a subclass of SceneObjectComponent
	Each subclass will be named with the name of the file
--]]
MyFirstScript.Type = "My Script"

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
	["MyVar"] = {
		Name = "Una variabile",
		Type = "string",
		Value = "Ciao"
	}
}
asd
-- Called when the component is added to a scene object
function MyFirstScript:Begin()
	FoxLog("My owner is " .. MyFirstScript.Object.Name)
end

-- Called each frame
function MyFirstScript:Tick(delta, other)
	--[[
		A good practice is to get a reference to the data table and each variable value
		for easy access in the code.
		Be careful that access to [ThisClass].Object is denied until the Begin method has been called.
	--]]
	local dat = MyFirstScript.Data
	local speed = dat["Speed"].Value

	if IsKeyDown(82) then
		local rot = MyFirstScript.Object.Transform.Rotation	
		rot.y = rot.y + delta * speed
	end
end

-- Called when the component is removed or the object is destroyed
function MyFirstScript:End()
end