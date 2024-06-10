#pragma once
#include "Core.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class ISerializedAsset {
public:
	ISerializedAsset() {}
	virtual void Deserialize(json Json) {};
	virtual void Serialize(json& outJson) {};
};
