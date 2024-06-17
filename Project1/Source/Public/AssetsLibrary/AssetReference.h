#pragma once

#include "Core.h"
#include "AssetsLibrary/AssetsLibrary.h"

template<class T>
struct FAssetReference {
public:
	FAssetReference() {
		static_assert( std::is_base_of<FAssetResource, T>::value, "An asset reference can only reference an FAssetResource.");
	}
	FAssetReference(SharedPtr<T> ref, BString path) : AssetPath(path), Ref(ref) {}
	bool IsValid() {
		bool valid = !Ref.expired() && FAssetsLibrary::AssetExists(AssetPath);
		if (!valid && !Ref.expired()) Ref.reset();
		return valid;
	}
	SharedPtr<T> Get() {
		if (!IsValid()) {
			return nullptr;
		}
		return SharedPtr<T>(Ref);
	}
	void Set(SharedPtr<T> newRef) {
		if (!Ref.expired()) Ref.reset();
		Ref = newRef;
		if (newRef == nullptr) { 
			Ref.reset();
			return;
		}
		SharedPtr<FAssetResource> r = std::static_pointer_cast<FAssetResource>(newRef);
		AssetPath = r->FilePath;
	}

protected:
	BString AssetPath;
	WeakPtr<T> Ref;
};
