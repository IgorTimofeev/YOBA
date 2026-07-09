#pragma once

#include <YOBA/System.hpp>

// Rendering targets
#include <YOBA/Rendering/Targets/RenderingTarget.hpp>

#if defined(YOBA_SYSTEM_DESKTOP)
	#include <YOBA/Rendering/Targets/SFMLWindowRenderingTarget.hpp>
#endif

// Renderers
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/TransactionalBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/PaletteRenderer.hpp>
#include <YOBA/Rendering/Renderers/Bit8PaletteRenderer.hpp>
#include <YOBA/Rendering/Renderers/RGB565Renderer.hpp>
#include <YOBA/Rendering/Renderers/MonochromeRenderer.hpp>
#include <YOBA/Rendering/Renderers/RGB888BufferedRenderer.hpp>