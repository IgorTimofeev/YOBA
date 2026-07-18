cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(YOBA_SOURCES
  src/YOBA/Core/Events/Event.cpp
  src/YOBA/Core/Events/KeyEvent.cpp
  src/YOBA/Core/Events/MouseWheelEvent.cpp
  src/YOBA/Core/Events/PinchEvent.cpp
  src/YOBA/Core/Events/PointerEvent.cpp
  src/YOBA/Core/Events/ScreenEvent.cpp
  src/YOBA/Core/Events/ScrollIntoViewEvent.cpp
  src/YOBA/Core/Font.cpp
  src/YOBA/Core/Margin.cpp
  src/YOBA/Core/Rectangle.cpp
  src/YOBA/Core/Size.cpp
  src/YOBA/Core/Text.cpp
  src/YOBA/Core/UTF-8.cpp
  src/YOBA/Hardware/Displays/Display.cpp
  src/YOBA/Hardware/Displays/GC9A01Display.cpp
  src/YOBA/Hardware/Displays/ILI9341Display.cpp
  src/YOBA/Hardware/Displays/SH1106Display.cpp
  src/YOBA/Hardware/Displays/SPIDisplay.cpp
  src/YOBA/Hardware/Displays/ST7565Display.cpp
  src/YOBA/Hardware/Displays/ST7789Display.cpp
  src/YOBA/Hardware/TouchPanels/FT6336UTouchPanel.cpp
  src/YOBA/Hardware/TouchPanels/TouchPoint.cpp
  src/YOBA/Hardware/Encoder.cpp
  src/YOBA/Rendering/Renderers/ARGBBufferedRenderer.cpp
  src/YOBA/Rendering/Renderers/BufferedRenderer.cpp
  src/YOBA/Rendering/Renderers/IndexedColors8TBufferedRenderer.cpp
  src/YOBA/Rendering/Renderers/MonochromeBufferedRenderer.cpp
  src/YOBA/Rendering/Renderers/Renderer.cpp
  src/YOBA/Rendering/Renderers/RGB565BufferedRenderer.cpp
  src/YOBA/Rendering/Targets/RenderingTarget.cpp
  src/YOBA/System/Desktop.cpp
  src/YOBA/System/ESP-IDF.cpp
  src/YOBA/UI/Animations/Animation.cpp
  src/YOBA/UI/Animations/CallbackAnimation.cpp
  src/YOBA/UI/Animations/FloatAnimation.cpp
  src/YOBA/UI/Animations/ScaleTransformAnimation.cpp
  src/YOBA/UI/Animations/SizeAnimation.cpp
  src/YOBA/UI/Animations/TargetAnimation.cpp
  src/YOBA/UI/Keyboard/Button.cpp
  src/YOBA/UI/Keyboard/Controller.cpp
  src/YOBA/UI/Keyboard/Keyboard.cpp
  src/YOBA/UI/Keyboard/Layout.cpp
  src/YOBA/UI/Spatial/Line.cpp
  src/YOBA/UI/Spatial/LinearCube.cpp
  src/YOBA/UI/Spatial/LinearMesh.cpp
  src/YOBA/UI/Spatial/LinearSphere.cpp
  src/YOBA/UI/Spatial/Mesh.cpp
  src/YOBA/UI/Spatial/Scene.cpp
  src/YOBA/UI/Spatial/SceneElement.cpp
  src/YOBA/UI/Transforms/MarginTransform.cpp
  src/YOBA/UI/Transforms/ScaleTransform.cpp
  src/YOBA/UI/Transforms/Transform.cpp
  src/YOBA/UI/Transforms/TransformGroup.cpp
  src/YOBA/UI/Transforms/TranslateTransform.cpp
  src/YOBA/UI/Application.cpp
  src/YOBA/UI/Button.cpp
  src/YOBA/UI/Control.cpp
  src/YOBA/UI/Decorator.cpp
  src/YOBA/UI/Divider.cpp
  src/YOBA/UI/Element.cpp
  src/YOBA/UI/ImageView.cpp
  src/YOBA/UI/Knob.cpp
  src/YOBA/UI/Layout.cpp
  src/YOBA/UI/Parent.cpp
  src/YOBA/UI/ProgressBar.cpp
  src/YOBA/UI/RelativeStackLayout.cpp
  src/YOBA/UI/ScrollBar.cpp
  src/YOBA/UI/ScrollView.cpp
  src/YOBA/UI/Selector.cpp
  src/YOBA/UI/SevenSegment.cpp
  src/YOBA/UI/Slider.cpp
  src/YOBA/UI/Spinner.cpp
  src/YOBA/UI/StackLayout.cpp
  src/YOBA/UI/Switch.cpp
  src/YOBA/UI/TextField.cpp
  src/YOBA/UI/TextView.cpp
  src/YOBA/UI/WrapLayout.cpp
  src/YOBA/UI/HSBColorPalette.cpp
  src/YOBA/UI/ColorPicker.cpp
  src/YOBA/UI/Titler.cpp
  src/YOBA/UI/TextAndSwitch.cpp
  src/YOBA/UI/Dialogs/Dialog.cpp
  src/YOBA/UI/Dialogs/BottomSlidingDialog.cpp
  src/YOBA/UI/Dialogs/ColorPickerDialog.cpp
)

#file(GLOB_RECURSE YOBA_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

if(ESP_PLATFORM)
  idf_component_register(
	SRCS ${YOBA_SOURCES}
	INCLUDE_DIRS src
	REQUIRES
	esp_timer
	esp_driver_spi
	esp_driver_gpio
	esp_driver_i2c
  )

  return()
else()
  project(YOBA)

  add_library(YOBA ${YOBA_SOURCES})
  target_include_directories(YOBA PUBLIC src)
endif()