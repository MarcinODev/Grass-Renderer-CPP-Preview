#pragma once
#include <QWidget>
#include <QCloseEvent>
#include <d3d11.h>

/// <summary>
/// DX11-QT device/rednerer initializer and QT input events catcher
/// </summary>
class D3DRendererWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(D3DRendererWidget)

public:
	D3DRendererWidget(QWidget *parent = Q_NULLPTR);
	~D3DRendererWidget();

	void setOnResizeHandler(std::function<void(int width, int height)> onResize);
	void setOnPaintHandler(std::function<void(void)> onPaint);
	void setOnMouseClickHandler(std::function<void(QMouseEvent* evt, bool press)> onMouseClick);
	void setOnMouseMoveHandler(std::function<void(QMouseEvent* evt)> onMouseMove);
	void setOnCloseHandler(std::function<void()> onClose);

	/// <summary>
	/// Enables renderTargetView for rendefing
	/// </summary>
	void setRenderTargetForRendering();

	/// <summary>
	/// Previews rendered data
	/// </summary>
	void finishRendering();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	ID3D11RenderTargetView* getRenderTarget();
	ID3D11DepthStencilView* getDepthStencilTarget();

protected:
	virtual void resizeEvent(QResizeEvent* evt);
	virtual void paintEvent(QPaintEvent* evt);
	virtual void mousePressEvent(QMouseEvent* evt);
	virtual void mouseReleaseEvent(QMouseEvent* evt);
	virtual void mouseMoveEvent(QMouseEvent* evt);
	virtual void closeEvent(QCloseEvent *event);

private:	
	const bool vSync = true;
	const bool wireframeMode = false;
	const bool disableCulling = true;
	const D3D_FEATURE_LEVEL d3dFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	std::function<void(int width, int height)> onResizeHandler;
	std::function<void(void)> onPaintHandler;
	std::function<void(QMouseEvent* evt, bool press)> onMouseClickHandler;
	std::function<void(QMouseEvent* evt)> onMouseMoveHandler;
	std::function<void(void)> onCloseHandler;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11RasterizerState* rasterState = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;

	void initialize();
	void initializeRenderTarget();
	void initializeRasterizer();
	void initializeDepthAndStencilTarget();
};
