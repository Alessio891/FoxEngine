-- Expose to editor
Data = {
	["Speed"] = { Type = "float", Value = 1.0, IsSlider = true, Min = 0.0, Max = 5.0 },
	["LookAt"] = { Type = "object", Value = nil },
	["Distance"] = { Type = "float", Value = 2.0 }
}

Speed = 1.0
LookAt = nil
Distance = 2.0

function Tick(delta)
	if LookAt == nil then return end

	d = LookAt.Transform.Position - Object.Transform.Position 
	d = Normalize(d)
	Object.Transform.Position = LookAt.Transform.Position - (d * Distance)

	right = Object.Transform:GetRightVector()
	
	Object.Transform:LookAt(LookAt.Transform.Position)

	Object.Transform.Position = Object.Transform.Position + (right * Speed * delta)
end