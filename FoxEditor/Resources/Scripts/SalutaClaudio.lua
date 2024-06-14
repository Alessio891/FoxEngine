
--[[
	Each component script should have a unique name.
	Each component script will be a subclass of SceneObjectComponent
	Each subclass will be named with the name of the file
--]]
SalutaClaudio.ScriptType = ScriptType.ObjectComponent
SalutaClaudio.Data = {
	["Saluto"] = {
		Name = "Il Saluto",
		Type = "string",
		Value = "Ciao"
	}
}

-- Called when the component is added to a scene object
function SalutaClaudio:Begin()
end

-- Called each frame
function SalutaClaudio:Tick(delta, other)
	if IsKeyDown(Key.H) then
		local c = Vector3F.new()
		c.x = 1.0
		c.y = 0.0
		c.z = 0.0
		SalutaClaudio.Object.Renderer.Color = c
		SalutaClaudio.Object.Renderer:SetTexture("Resources/Images/crate.png")
	else
		local c = Vector3F.new()
		c.x = 1.0
		c.y = 1.0
		c.z = 1.0
		SalutaClaudio.Object.Renderer.Color = c
		SalutaClaudio.Object.Renderer:SetTexture("Resources/Images/test.png")
	end
end

-- Called when the component is removed or the object is destroyed
function SalutaClaudio:End()
end