#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <YOBA/Core/Events/SFMLEvents.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	void SFMLEvents::handleMouse(
		const std::optional<sf::Event>& event,
		Application* application,
		const float renderingScale
	) {
		if (event->is<sf::Event::MouseButtonPressed>()) {
			const auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

			if (mouseEvent->button == sf::Mouse::Button::Left) {
				PointerDownEvent pointerDownEvent {{
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.x) / renderingScale),
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.y) / renderingScale)
				}};

				application->pushEvent(&pointerDownEvent);
			}
		}
		else if (event->is<sf::Event::MouseButtonReleased>()) {
			const auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>();

			if (mouseEvent->button == sf::Mouse::Button::Left) {
				PointerUpEvent pointerUpEvent {{
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.x) / renderingScale),
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.y) / renderingScale)
				}};

				application->pushEvent(&pointerUpEvent);
			}
		}
		else if (event->is<sf::Event::MouseMoved>() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			const auto mouseEvent = event->getIf<sf::Event::MouseMoved>();

			PointerDragEvent pointerDragEvent {{
				static_cast<int32_t>(static_cast<float>(mouseEvent->position.x) / renderingScale),
				static_cast<int32_t>(static_cast<float>(mouseEvent->position.y) / renderingScale)
			}};

			application->pushEvent(&pointerDragEvent);
		}
		else if (event->is<sf::Event::MouseWheelScrolled>()) {
			const auto mouseWheelScrolledEvent = event->getIf<sf::Event::MouseWheelScrolled>();

			MouseWheelEvent mouseWheelEvent {
				{
					static_cast<int32_t>(static_cast<float>(mouseWheelScrolledEvent->position.x) / renderingScale),
					static_cast<int32_t>(static_cast<float>(mouseWheelScrolledEvent->position.y) / renderingScale)
				},
				static_cast<int32_t>(mouseWheelScrolledEvent->delta) * 20
			};

			application->pushEvent(&mouseWheelEvent);
		}
	}
}

#endif