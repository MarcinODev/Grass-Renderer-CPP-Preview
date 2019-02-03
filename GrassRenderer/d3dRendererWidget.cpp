#include <QMouseEvent>
#include <string>
#include "d3dRendererWidget.h"


D3DRendererWidget::D3DRendererWidget(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	setFixedWidth(parent->width());
	setFixedHeight(parent->height());
	initialize();
}

D3DRendererWidget::~D3DRendererWidget()
{
	if(rasterState != nullptr)
	{
		rasterState->Release();
	}
	
	if(device != nullptr)
	{
		device->Release();
	}

	if(deviceContext != nullptr)
	{
		deviceContext->Release();
	}

	if(swapChain != nullptr)
	{
		swapChain->Release();
	}

	if(renderTargetView != nullptr)
	{
		renderTargetView->Release();
	}

	if(depthStencilBuffer != nullptr)
	{
		depthStencilBuffer->Release();
	}

	if(depthStencilState != nullptr)
	{
		depthStencilState->Release();
	}

	if(depthStencilView != nullptr)
	{
		depthStencilView->Release();
	}
}

void D3DRendererWidget::setOnResizeHandler(std::function<void(int width, int height)> onResize)
{
	onResizeHandler = onResize;
}

void D3DRendererWidget::setOnPaintHandler(std::function<void(void)> onPaint)
{
	onPaintHandler = onPaint;
}

void D3DRendererWidget::setOnMouseClickHandler(std::function<void(QMouseEvent*evt, bool press)> onMouseClick)
{
	onMouseClickHandler = onMouseClick;
}

void D3DRendererWidget::setOnMouseMoveHandler(std::function<void(QMouseEvent*evt)> onMouseMove)
{
	onMouseMoveHandler = onMouseMove;
}

void D3DRendererWidget::setOnCloseHandler(std::function<void()> onClose)
{
	onCloseHandler = onClose;
}

void D3DRendererWidget::setRenderTargetForRendering()
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void D3DRendererWidget::finishRendering()
{
	swapChain->Present(vSync ? 1 : 0, 0);
}

ID3D11Device* D3DRendererWidget::getDevice()
{
	return device;
}

ID3D11DeviceContext* D3DRendererWidget::getDeviceContext()
{
	return deviceContext;
}

ID3D11RenderTargetView * D3DRendererWidget::getRenderTarget()
{
	return renderTargetView;
}

ID3D11DepthStencilView * D3DRendererWidget::getDepthStencilTarget()
{
	return depthStencilView;
}

void D3DRendererWidget::initialize()
{
	initializeRenderTarget();
	initializeRasterizer();
	initializeDepthAndStencilTarget();
}

void D3DRendererWidget::initializeRenderTarget()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.OutputWindow = (HWND)winId();
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width();
	swapChainDesc.BufferDesc.Height = height();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
												&d3dFeatureLevel, 1, D3D11_SDK_VERSION,
												&swapChainDesc, &swapChain, &device,
												NULL, &deviceContext);
	assert(!FAILED(hr));
	
	ID3D11Texture2D* backBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	assert(!FAILED(hr));

	hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	assert(!FAILED(hr));

	backBuffer->Release();
}

void D3DRendererWidget::initializeRasterizer()
{
	//In future this need to be moved and extended to allow change raster per camera/material setup
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.CullMode = disableCulling ? D3D11_CULL_MODE::D3D11_CULL_NONE : D3D11_CULL_MODE::D3D11_CULL_BACK;
	rasterDesc.FillMode = wireframeMode ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	HRESULT hr = device->CreateRasterizerState(&rasterDesc, &rasterState);
	assert(!FAILED(hr));

	deviceContext->RSSetState(rasterState);
}

void D3DRendererWidget::initializeDepthAndStencilTarget()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.Width = width();
	depthBufferDesc.Height = height();
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	HRESULT hr = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	assert(!FAILED(hr));
	
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	assert(!FAILED(hr));

	//In the future this needs to be moved and extended for per-material usage
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	
	//Front facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Back facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	assert(!FAILED(hr));

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

}

void D3DRendererWidget::resizeEvent(QResizeEvent * evt)
{
	if(onResizeHandler)
	{
		onResizeHandler(width(), height());
	}
}

void D3DRendererWidget::paintEvent(QPaintEvent * evt)
{
	if(onPaintHandler)
	{
		onPaintHandler();
	}
}

void D3DRendererWidget::mousePressEvent(QMouseEvent * evt)
{
	if(onMouseClickHandler)
	{
		onMouseClickHandler(evt, true);
	}
}

void D3DRendererWidget::mouseReleaseEvent(QMouseEvent * evt)
{
	if(onMouseClickHandler)
	{
		onMouseClickHandler(evt, false);
	}
}

void D3DRendererWidget::mouseMoveEvent(QMouseEvent * evt)
{
	if(onMouseMoveHandler)
	{
		onMouseMoveHandler(evt);
	}
}

void D3DRendererWidget::closeEvent(QCloseEvent * event)
{
	if(onCloseHandler)
	{
		onCloseHandler();
	}

	QWidget::closeEvent(event);
}
