#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_Desc{}
	, m_ElementCount(0)
	, m_ElementSize(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, void* _InitData)
{
	m_SB = nullptr;
	m_SRV = nullptr;

	m_ElementSize = _ElementSize;
	m_ElementCount = _ElementCount;

	// ID3D11Buffer�� StructedBuffer �뵵�� �����ϱ�
	m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.StructureByteStride = m_ElementSize;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	HRESULT hr = S_OK;
	if (_InitData == nullptr)
	{
		hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_SB.GetAddressOf());
	}
	else
	{
		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = _InitData;
		hr = DEVICE->CreateBuffer(&m_Desc, &sub, m_SB.GetAddressOf());
	}

	if (FAILED(hr))
		return E_FAIL;


	// ShaderResourceView �����ϱ�
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	SRVDesc.BufferEx.NumElements = m_ElementCount;

	hr = DEVICE->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf());
	if (FAILED(hr))
	{
		hr;
		return E_FAIL;
	}

	return S_OK;
}

void CStructuredBuffer::SetData(void* _pData, UINT _DataSize)
{
	// �Է� �����Ͱ� ����ȭ������ ũ�⺸�� �۾ƾ� �Ѵ�
	assert(_DataSize <= m_Desc.ByteWidth);

	if (_DataSize == 0)
	{
		_DataSize = m_Desc.ByteWidth;
	}
	
	D3D11_MAPPED_SUBRESOURCE tMapSub = {};
	CONTEXT->Map(m_SB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
	memcpy(tMapSub.pData, _pData, _DataSize);
	CONTEXT->Unmap(m_SB.Get(), 0);
}

void CStructuredBuffer::Binding(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}