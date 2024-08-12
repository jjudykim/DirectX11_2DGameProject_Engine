#pragma once

template<typename T>
class Ptr
{
private:
	T* Asset;

public:
	T* Get() const { return Asset; }
	T** GetAddressOf() const { return &Asset; }
	T* operator ->() const { return Asset; }

public:
	bool operator ==(Ptr<T> _Other) { return Asset == _Other.Asset; }
	bool operator ==(T* _Other) { return Asset == _Other; }
	bool operator !=(Ptr<T> _Other) { return Asset != _Other.Asset; }
	bool operator !=(T* _Other) { return Asset != _Other; }

public:
	bool operator !() { return !Asset; }

	Ptr& operator = (T* _Other)
	{
		if (Asset != nullptr)
			Asset->Release();

		Asset = _Other;

		if (Asset != nullptr)
			Asset->AddRef();

		return *this;
	}

	Ptr& operator = (const Ptr<T>& _Other)
	{
		if (Asset != nullptr)
			Asset->Release();

		Asset = _Other.Asset;

		if (Asset != nullptr)
			Asset->AddRef();

		return *this;
	}

public:
	Ptr()
		: Asset(nullptr)
	{}

	Ptr(T* _Asset)
		: Asset(_Asset)
	{
		if (Asset != nullptr)
			Asset->AddRef();
	}

	Ptr(const Ptr<T>& _Ptr)
		: Asset(_Ptr.Asset)
	{
		if (Asset != nullptr)
			Asset->AddRef();
	}

	~Ptr()
	{
		if (Asset != nullptr)
			Asset->Release();
	}
};

template<typename T>
bool operator == (void* _Asset, Ptr<T> _PtrAsset)
{
	return _Asset == _PtrAsset.Get();
}

template<typename T>
bool operator != (void* _Asset, Ptr<T> _PtrAsset)
{
	return _Asset != _PtrAsset.Get();
}