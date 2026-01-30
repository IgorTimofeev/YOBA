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
				std::wstring_view name,
				float width
			);

			constexpr static float stretched = -1;

			void assignKeyboard(Keyboard* keyboard, uint8_t row, uint8_t column);

			virtual Key getKeyFromCase() const;
			virtual std::wstring_view getNameFromCase() const;
			virtual std::optional<std::wstring_view> getTextFromCase() const;
			virtual bool canBeAdded();
			virtual bool canBeEnabled();

			Key getKey() const;
			float getKeyWidth() const;
			KeyboardKeyType getType() const;
			std::wstring_view getName() const;
			Keyboard* getKeyboard() const;
			uint8_t getRow() const;
			uint8_t getColumn() const;
			void updateFromCase();

		protected:
			void onIsActiveChanged() override;

		private:
			KeyboardKeyType _type;
			Key _key;
			const std::wstring_view _name;
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
				std::wstring_view name,
				Key uppercaseKey,
				std::wstring_view uppercaseName,
				float width
			);

			TextKeyboardButton(
				Key key,
				std::wstring_view name,
				float width
			);

			Key getUppercaseKey() const;
			std::wstring_view getUppercaseName() const;

			Key getKeyFromCase() const override;

			std::wstring_view getNameFromCase() const override;
			std::optional<std::wstring_view> getTextFromCase() const override;

		protected:
			void onIsActiveChanged() override;

		private:
			Key _uppercaseKey;
			const std::wstring_view _uppercaseName;
	};

	class ShiftKeyboardButton : public KeyboardButton {
		public:
			ShiftKeyboardButton(std::wstring_view name, std::wstring_view uppercaseName, std::wstring_view capsName, float width);
			std::wstring_view getNameFromCase() const override;

		protected:
			void onIsActiveChanged() override;

		private:
			const std::wstring_view _uppercaseName;
			const std::wstring_view _capsName;
	};

	class BackspaceKeyboardButton : public KeyboardButton {
		public:
			BackspaceKeyboardButton(std::wstring_view name, float width);
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
			CharactersLayoutKeyboardButton(std::wstring_view name, float width);

			bool canBeAdded() override;

		protected:
			void onIsActiveChanged() override;

	};

	class CurrentCyclicLayoutKeyboardButton : public KeyboardButton {
		public:
			CurrentCyclicLayoutKeyboardButton(std::wstring_view name, float width);

		protected:
			void onIsActiveChanged() override;

	};

	class CyclicLayoutKeyboardButton : public KeyboardButton {
		public:
			CyclicLayoutKeyboardButton(std::wstring_view name, float width);

			bool canBeAdded() override;

		protected:
			void onIsActiveChanged() override;

	};
}