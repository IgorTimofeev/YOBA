#pragma once

#include <YOBA/System.hpp>

// Rendering targets
#include <YOBA/Rendering/Targets/RenderingTarget.hpp>

#if defined(YOBA_SYSTEM_DESKTOP)
	#include <YOBA/Rendering/Targets/SFMLSpriteRenderingTarget.hpp>
#endif

// Renderers
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/TransactionalBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/IndexedColorsTransactionalBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/IndexedColors8TransactionalBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/RGB565TransactionalBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/MonochromeBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/RGB888BufferedRenderer.hpp>