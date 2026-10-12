#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <YOBA/Core/Events/SFMLEvents.hpp>
#include <YOBA/UI/Application.hpp>

#include "KeyEvent.hpp"

namespace YOBA {
	void SFMLEvents::translate(
		const std::optional<sf::Event>& event,
		Application* application,
		const float renderingScale
	) {
		if (event->is<sf::Event::MouseButtonPressed>()) {
			const auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

			if (mouseEvent->button == sf::Mouse::Button::Left) {
				PointerDownEvent yobaEvent {{
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.x) / renderingScale),
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.y) / renderingScale)
				}};

				application->pushEvent(&yobaEvent);
			}
		}
		else if (event->is<sf::Event::MouseButtonReleased>()) {
			const auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>();

			if (mouseEvent->button == sf::Mouse::Button::Left) {
				PointerUpEvent yobaEvent {{
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.x) / renderingScale),
					static_cast<int32_t>(static_cast<float>(mouseEvent->position.y) / renderingScale)
				}};

				application->pushEvent(&yobaEvent);
			}
		}
		else if (event->is<sf::Event::MouseMoved>() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			const auto mouseEvent = event->getIf<sf::Event::MouseMoved>();

			PointerDragEvent yobaEvent {{
				static_cast<int32_t>(static_cast<float>(mouseEvent->position.x) / renderingScale),
				static_cast<int32_t>(static_cast<float>(mouseEvent->position.y) / renderingScale)
			}};

			application->pushEvent(&yobaEvent);
		}
		else if (event->is<sf::Event::MouseWheelScrolled>()) {
			const auto mouseWheelScrolledEvent = event->getIf<sf::Event::MouseWheelScrolled>();

			MouseWheelEvent yobaEvent {
				{
					static_cast<int32_t>(static_cast<float>(mouseWheelScrolledEvent->position.x) / renderingScale),
					static_cast<int32_t>(static_cast<float>(mouseWheelScrolledEvent->position.y) / renderingScale)
				},
				static_cast<int32_t>(mouseWheelScrolledEvent->delta) * 20
			};

			application->pushEvent(&yobaEvent);
		}
		else if (event->is<sf::Event::KeyPressed>()) {
			const auto keyPressedEvent = event->getIf<sf::Event::KeyPressed>();

			Key yobaKey {};
			std::string_view yobaText {};

			if (translateKey(keyPressedEvent->code, yobaKey, yobaText)) {
				KeyDownEvent yobaEvent { yobaKey, yobaText };

				application->pushEvent(&yobaEvent);
			}
		}
		else if (event->is<sf::Event::KeyReleased>()) {
			const auto keyReleasedEvent = event->getIf<sf::Event::KeyReleased>();

			Key yobaKey {};
			std::string_view yobaText {};

			if (translateKey(keyReleasedEvent->code, yobaKey, yobaText)) {
				KeyUpEvent yobaEvent { yobaKey, yobaText };

				application->pushEvent(&yobaEvent);
			}
		}
	}

	bool SFMLEvents::translateKey(const sf::Keyboard::Key sfKey, Key& yobaKey, std::string_view& yobaText) {
		const auto set = [&yobaKey, &yobaText](const Key key, const std::string_view text) {
			yobaKey = key;
			yobaText = text;
		};

		const auto setCase = [&set](const Key key, const std::string_view upper, const std::string_view lower) {
			set(
				key,
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)
					? upper
					: lower
			);
		};

		switch (sfKey) {
			case sf::Keyboard::Key::Num0: set(Key::number0, "0"); break;
			case sf::Keyboard::Key::Num1: set(Key::number1, "1"); break;
			case sf::Keyboard::Key::Num2: set(Key::number2, "2"); break;
			case sf::Keyboard::Key::Num3: set(Key::number3, "3"); break;
			case sf::Keyboard::Key::Num4: set(Key::number4, "4"); break;
			case sf::Keyboard::Key::Num5: set(Key::number5, "5"); break;
			case sf::Keyboard::Key::Num6: set(Key::number6, "6"); break;
			case sf::Keyboard::Key::Num7: set(Key::number7, "7"); break;
			case sf::Keyboard::Key::Num8: set(Key::number8, "8"); break;
			case sf::Keyboard::Key::Num9: set(Key::number9, "9"); break;

			case sf::Keyboard::Key::A: setCase(Key::a, "a", "A"); break;
			case sf::Keyboard::Key::B: setCase(Key::b, "b", "B"); break;
			case sf::Keyboard::Key::C: setCase(Key::c, "c", "C"); break;
			case sf::Keyboard::Key::D: setCase(Key::d, "d", "D"); break;
			case sf::Keyboard::Key::E: setCase(Key::e, "e", "E"); break;
			case sf::Keyboard::Key::F: setCase(Key::f, "f", "F"); break;
			case sf::Keyboard::Key::G: setCase(Key::g, "g", "G"); break;
			case sf::Keyboard::Key::H: setCase(Key::h, "h", "H"); break;
			case sf::Keyboard::Key::I: setCase(Key::i, "i", "I"); break;
			case sf::Keyboard::Key::J: setCase(Key::j, "j", "J"); break;
			case sf::Keyboard::Key::K: setCase(Key::k, "k", "K"); break;
			case sf::Keyboard::Key::L: setCase(Key::l, "l", "L"); break;
			case sf::Keyboard::Key::M: setCase(Key::m, "m", "M"); break;
			case sf::Keyboard::Key::N: setCase(Key::n, "n", "N"); break;
			case sf::Keyboard::Key::O: setCase(Key::o, "o", "O"); break;
			case sf::Keyboard::Key::P: setCase(Key::p, "p", "P"); break;
			case sf::Keyboard::Key::Q: setCase(Key::q, "q", "Q"); break;
			case sf::Keyboard::Key::R: setCase(Key::r, "r", "R"); break;
			case sf::Keyboard::Key::S: setCase(Key::s, "s", "S"); break;
			case sf::Keyboard::Key::T: setCase(Key::t, "t", "T"); break;
			case sf::Keyboard::Key::U: setCase(Key::u, "u", "U"); break;
			case sf::Keyboard::Key::V: setCase(Key::v, "v", "V"); break;
			case sf::Keyboard::Key::W: setCase(Key::w, "w", "W"); break;
			case sf::Keyboard::Key::X: setCase(Key::x, "x", "X"); break;
			case sf::Keyboard::Key::Y: setCase(Key::y, "y", "Y"); break;
			case sf::Keyboard::Key::Z: setCase(Key::z, "z", "Z"); break;

			case sf::Keyboard::Key::Space: set(Key::space, " "); break;
			case sf::Keyboard::Key::Comma: set(Key::comma, ","); break;
			case sf::Keyboard::Key::Period: set(Key::period, "."); break;
			case sf::Keyboard::Key::Semicolon: set(Key::semicolon, ";"); break;
			case sf::Keyboard::Key::Slash: set(Key::slash, "/"); break;
			case sf::Keyboard::Key::Backslash: set(Key::backSlash, "\\"); break;
			case sf::Keyboard::Key::Equal: set(Key::equals, "="); break;
			case sf::Keyboard::Key::Add: set(Key::plus, "+"); break;
			case sf::Keyboard::Key::Subtract: set(Key::plus, "-"); break;
			case sf::Keyboard::Key::Multiply: set(Key::multiplication, "*"); break;
			case sf::Keyboard::Key::Divide: set(Key::division, "/"); break;
			case sf::Keyboard::Key::LBracket: set(Key::leftBracket, "["); break;
			case sf::Keyboard::Key::RBracket: set(Key::rightBracket, "]"); break;
			case sf::Keyboard::Key::Apostrophe: set(Key::quote, "\""); break;

			case sf::Keyboard::Key::Backspace: set(Key::backspace, std::string_view {}); break;
			case sf::Keyboard::Key::Enter: set(Key::enter, std::string_view {}); break;
			case sf::Keyboard::Key::Left: set(Key::left, std::string_view {}); break;
			case sf::Keyboard::Key::Right: set(Key::right, std::string_view {}); break;
			case sf::Keyboard::Key::Up: set(Key::up, std::string_view {}); break;
			case sf::Keyboard::Key::Down: set(Key::down, std::string_view {}); break;

			default: return false;
		}

		return true;
	}
}

#endif