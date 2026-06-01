#pragma once

#include <optional>

#include <YOBA/main/key.h>
#include <YOBA/UI/button.h>
#include <YOBA/UI/relativeStackLayout.h>
#include <YOBA/UI/keyboard/generic.h>

namespace YOBA {
	class Keyboard;

	class KeyboardButton : public Button {
		public:
			KeyboardButton(
				KeyboardKeyType type,
				Key key,
				std::string_view name,
				float width
			);

			constexpr static float stretched = -1;

			void assignKeyboard(Keyboard* keyboard, uint8_t row, uint8_t column);

			virtual Key getKeyFromCase() const;
			virtual std::string_view getNameFromCase() const;
			virtual std::optional<std::string_view> getTextFromCase() const;
			virtual bool canBeAdded();
			virtual bool canBeEnabled();

			Key getKey() const;
			float getKeyWidth() const;
			KeyboardKeyType getType() const;
			std::string_view getName() const;
			Keyboard* getKeyboard() const;
			uint8_t getRow() const;
			uint8_t getColumn() const;
			void updateFromCase();

		protected:
			void onIsActiveChanged() override;

		private:
			KeyboardKeyType _type;
			Key _key;
			const std::string_view _name;
			float _width;

			Keyboard* _keyboard = nullptr;
			uint8_t _row = 0;
			uint8_t _column = 0;

			void updateTextFromCase();
	};

	class TextKeyboardButton : public KeyboardButton {
		public:
			TextKeyboardButton(
				Key key,
				std::string_view name,
				Key uppercaseKey,
				std::string_view uppercaseName,
				float width
			);

			TextKeyboardButton(
				Key key,
				std::string_view name,
				float width
			);

			Key getUppercaseKey() const;
			std::string_view getUppercaseName() const;

			Key getKeyFromCase() const override;

			std::string_view getNameFromCase() const override;
			std::optional<std::string_view> getTextFromCase() const override;

		protected:
			void onIsActiveChanged() override;

		private:
			Key _uppercaseKey;
			const std::string_view _uppercaseName;
	};

	class ShiftKeyboardButton : public KeyboardButton {
		public:
			ShiftKeyboardButton(std::string_view name, std::string_view uppercaseName, std::string_view capsName, float width);
			std::string_view getNameFromCase() const override;

		protected:
			void onIsActiveChanged() override;

		private:
			const std::string_view _uppercaseName;
			const std::string_view _capsName;
	};

	class BackspaceKeyboardButton : public KeyboardButton {
		public:
			BackspaceKeyboardButton(std::string_view name, float width);
	};

	class EnterKeyboardButton : public KeyboardButton {
		public:
			explicit EnterKeyboardButton(float width);

		protected:
			void onIsActiveChanged() override;
	};

	class SpaceKeyboardButton : public TextKeyboardButton {
		public:
			SpaceKeyboardButton();
	};

	class PeriodKeyboardButton : public TextKeyboardButton {
		public:
			PeriodKeyboardButton(const float width);
			bool canBeEnabled() override;
	};

	class NumericSignMinusKeyboardKey : public TextKeyboardButton {
		public:
			NumericSignMinusKeyboardKey(const float width);
			bool canBeEnabled() override;
	};

	class DummyKeyboardButton : public TextKeyboardButton {
		public:
			DummyKeyboardButton(const float width);
			bool canBeEnabled() override;
	};

	class CharactersLayoutKeyboardButton : public KeyboardButton {
		public:
			CharactersLayoutKeyboardButton(std::string_view name, float width);

			bool canBeAdded() override;

		protected:
			void onIsActiveChanged() override;

	};

	class CurrentCyclicLayoutKeyboardButton : public KeyboardButton {
		public:
			CurrentCyclicLayoutKeyboardButton(std::string_view name, float width);

		protected:
			void onIsActiveChanged() override;

	};

	class CyclicLayoutKeyboardButton : public KeyboardButton {
		public:
			CyclicLayoutKeyboardButton(std::string_view name, float width);

			bool canBeAdded() override;

		protected:
			void onIsActiveChanged() override;

	};
}