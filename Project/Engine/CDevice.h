﻿#pragma once

#include "CTexture.h"

class CConstBuffer;
class CStructuredBuffer;

// Directx11 기준 GPU 제어
class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
private:
	ComPtr<ID3D11Device>			m_Device;	// GPU 메모리 관리, 객체 생성
	ComPtr<ID3D11DeviceContext>		m_Context;	// GPU 렌더링 명령 

	ComPtr<IDXGISwapChain>			m_SwapChain; // 스왚체인(출력 버퍼 지정)

	HWND							m_hRenderWnd;
	Vec2							m_vRenderResolution;

	CConstBuffer*					m_arrCB[(UINT)CB_TYPE::END];

	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];
	ComPtr<ID3D11SamplerState>		m_arrSampler[2];

	CStructuredBuffer* m_Light2DBuffer;
	CStructuredBuffer* m_Light3DBuffer;

	CStructuredBuffer* m_CrossBuffer;	    // 마우스 피킹되는 지점 정보 받는 버퍼
	CStructuredBuffer* m_WeightMapBuffer;	// Landscape에서 사용하는 가중치 버퍼

public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void Present();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }
	CConstBuffer* GetConstBuffer(CB_TYPE _type) { return m_arrCB[(UINT)_type]; }
	Vec2 GetRenderResolution() { return m_vRenderResolution; }

	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_arrRS[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_arrDS[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_arrBS[(UINT)_Type]; }

	CStructuredBuffer* GetLight2DBuffer() { return m_Light2DBuffer; }
	CStructuredBuffer* GetLight3DBuffer() { return m_Light3DBuffer; }
	
	int CreateWeightMapBuffer(UINT _Width, UINT _Height);

	CStructuredBuffer* GetCrossBuffer() { return m_CrossBuffer; }
	CStructuredBuffer* GetWeightMapBuffer() { return m_WeightMapBuffer; }

	
private:
	int CreateSwapChain();
	int CreateTargetView();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();

	int CreateConstBuffer();
	int CreateLightBuffer();
	int CreateCrossBuffer();
	
};

