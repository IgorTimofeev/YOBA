#pragma once

#include <YOBA/System.hpp>

// Rendering targets
#include <YOBA/Rendering/Targets/RenderingTarget.hpp>

#if defined(YOBA_SYSTEM_SFML)
	#include <YOBA/Rendering/Targets/SFMLSpriteRenderingTarget.hpp>
#endif

// Renderers
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/IndexedColorsBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/IndexedColors8TBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/RGB565BufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/MonochromeBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/ARGBBufferedRenderer.hpp>
