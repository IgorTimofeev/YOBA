#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <optional>

#include <YOBA/Core.hpp>
#include <SFML/Graphics.hpp>

namespace YOBA {
	class SFMLEvents {
		public:
			static void handleMouse(
				const std::optional<sf::Event>& event,
				Application* application,
				const float renderingScale = 1.0f
			);
	};
}

#endif