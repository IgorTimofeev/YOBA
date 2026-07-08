#pragma once

#include <YOBA/System.hpp>

// Rendering targets
#include <YOBA/Rendering/RenderingTarget.hpp>

#if defined(YOBA_SYSTEM_DESKTOP)
	#include <YOBA/Rendering/Desktop/SFMLRenderingTarget.hpp>
#endif

// Renderers
#include <YOBA/Rendering/Renderer.hpp>
#include <YOBA/Rendering/BufferedRenderer.hpp>
#include <YOBA/Rendering/TransactionalBufferedRenderer.hpp>
#include <YOBA/Rendering/PaletteRenderer.hpp>
#include <YOBA/Rendering/Bit8PaletteRenderer.hpp>
#include <YOBA/Rendering/RGB565Renderer.hpp>
#include <YOBA/Rendering/MonochromeRenderer.hpp>

#if defined(YOBA_SYSTEM_DESKTOP)
	#include <YOBA/Rendering/Desktop/SFMLRenderer.hpp>
#endif