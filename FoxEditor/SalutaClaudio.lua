
--[[
	Each component script should have a unique name.
	Each component script will be a subclass of SceneObjectComponent
	Each subclass will be named with the name of the file
--]]
TestScript.ScriptType = ScriptType.ObjectComponent


-- Called when the component is added to a scene object
function TestScript:Begin()
end

-- Called each frame
function TestScript:Tick(delta, other)
	
end

-- Called when the component is removed or the object is destroyed
function TestScript:End()
end