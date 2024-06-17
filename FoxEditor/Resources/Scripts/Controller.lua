Data = {
	["Speed"] = { Type = "float", Value = 1.0, IsSlider = true, Min = 0.0, Max = 5.0 },
	["Offset"] = { Type = "vector",	Value = Vector3F.new(0.0, 6.0, -15.0) },
	["Gravity"] = {	Type = "float",	Value = 2.7, FloatSpeed = 0.1 },
	["JumpForce"] = { Type = "float", Value = 9.2, FloatSpeed = 0.3 }
}

JumpVel = 0.0
Gravity = 2.0
JumpForce = 20.0
Speed = 1.0

function Tick(delta)
	
	t = GetCameraTransform()
	t.Position = Object.Transform.Position + Data["Offset"].Value
	t:LookAt(Object.Transform.Position)
	SetCameraTransform(t)

	if IsKeyHeld(Key.W) then
		Object.Transform.Position.z = Object.Transform.Position.z + Speed * delta
	elseif IsKeyHeld(Key.S) then
		Object.Transform.Position.z = Object.Transform.Position.z - Speed * delta
	end

	if IsKeyHeld(Key.A) then
		Object.Transform.Position.x = Object.Transform.Position.x + Speed * delta
	elseif IsKeyHeld(Key.D) then
		Object.Transform.Position.x = Object.Transform.Position.x - Speed * delta
	end

	if IsKeyDown(Key.Space) then
		JumpVel = JumpForce
	end
	
	Object.Transform.Position.y = Object.Transform.Position.y + JumpVel * delta
	if JumpVel > 0.0 then
		
		JumpVel = JumpVel - Gravity * delta
	elseif JumpVel <= 0.0 then
		
		JumpVel = 0.0
	end
	if Object.Transform.Position.y > 0.0 then
		c = Vector3F.new()
		c.x = 255.0
		c.y = 0.0
		c.z = 0.0
		Object.Renderer:SetColor(c)
		Object.Transform.Position.y = Object.Transform.Position.y - Gravity * delta
	else
		c = Vector3F.new()
		c.x = 1.0
		c.y = 1.0
		c.z = 1.0
		Object.Renderer:SetColor(c)
	end
	

end