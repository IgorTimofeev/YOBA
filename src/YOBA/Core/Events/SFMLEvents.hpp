#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <optional>
#include <string_view>

#include <YOBA/Core.hpp>
#include <SFML/Graphics.hpp>
#include "YOBA/Core/Key.hpp"

namespace YOBA {
	class SFMLEvents {
		public:
			static void translate(
				const std::optional<sf::Event>& event,
				Application* application,
				const float renderingScale = 1.0f
			);

		private:
			static bool translateKey(const sf::Keyboard::Key sfKey, Key& yobaKey, std::string_view& yobaText);
	};
}

#endif