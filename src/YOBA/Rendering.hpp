#pragma once

#include <YOBA/System.hpp>

// Rendering targets
#include <YOBA/Rendering/Targets/RenderingTarget.hpp>

#ifdef YOBA_SYSTEM_SFML
	#include <YOBA/Rendering/Targets/SFMLRenderingTarget.hpp>
#endif

// Renderers
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/IndexedBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/Indexed8BufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/RGB565BufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/MonochromeBufferedRenderer.hpp>
#include <YOBA/Rendering/Renderers/ARGBBufferedRenderer.hpp>

#ifdef YOBA_SYSTEM_SFML
	#include <YOBA/Rendering/Renderers/SFMLRenderer.hpp>
#endif
