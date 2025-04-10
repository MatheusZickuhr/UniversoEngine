#pragma once

#include "stdint.h"

struct GLFWwindow;

namespace engine {
	class Window {

	public:

		Window(const char* name, int32_t width, int32_t height);

		~Window();

		bool isRunning();

		void swapBuffers();

		void pollEvents();

		void close();

		void getCursorPos(float* xpos, float* ypos);

		bool keyPressed(int32_t key);

		bool mouseButtonPressed(int32_t key);

	private:
		const char* name;
		int32_t width, height;
		GLFWwindow* glfwWindow;

		void initializeGlfwWindow();

	public:
		static const int32_t KEY_SPACE = 32;
		static const int32_t KEY_APOSTROPHE = 39;
		static const int32_t KEY_COMMA = 44;
		static const int32_t KEY_MINUS = 45;
		static const int32_t KEY_PERIOD = 46;
		static const int32_t KEY_SLASH = 47;
		static const int32_t KEY_0 = 48;
		static const int32_t KEY_1 = 49;
		static const int32_t KEY_2 = 50;
		static const int32_t KEY_3 = 51;
		static const int32_t KEY_4 = 52;
		static const int32_t KEY_5 = 53;
		static const int32_t KEY_6 = 54;
		static const int32_t KEY_7 = 55;
		static const int32_t KEY_8 = 56;
		static const int32_t KEY_9 = 57;
		static const int32_t KEY_SEMICOLON = 59;
		static const int32_t KEY_EQUAL = 61;
		static const int32_t KEY_A = 65;
		static const int32_t KEY_B = 66;
		static const int32_t KEY_C = 67;
		static const int32_t KEY_D = 68;
		static const int32_t KEY_E = 69;
		static const int32_t KEY_F = 70;
		static const int32_t KEY_G = 71;
		static const int32_t KEY_H = 72;
		static const int32_t KEY_I = 73;
		static const int32_t KEY_J = 74;
		static const int32_t KEY_K = 75;
		static const int32_t KEY_L = 76;
		static const int32_t KEY_M = 77;
		static const int32_t KEY_N = 78;
		static const int32_t KEY_O = 79;
		static const int32_t KEY_P = 80;
		static const int32_t KEY_Q = 81;
		static const int32_t KEY_R = 82;
		static const int32_t KEY_S = 83;
		static const int32_t KEY_T = 84;
		static const int32_t KEY_U = 85;
		static const int32_t KEY_V = 86;
		static const int32_t KEY_W = 87;
		static const int32_t KEY_X = 88;
		static const int32_t KEY_Y = 89;
		static const int32_t KEY_Z = 90;
		static const int32_t KEY_LEFT_BRACKET = 91;
		static const int32_t KEY_BACKSLASH = 92;
		static const int32_t KEY_RIGHT_BRACKET = 93;
		static const int32_t KEY_GRAVE_ACCENT = 96;
		static const int32_t KEY_WORLD_1 = 161;
		static const int32_t KEY_WORLD_2 = 162;
		static const int32_t KEY_ESCAPE = 256;
		static const int32_t KEY_ENTER = 257;
		static const int32_t KEY_TAB = 258;
		static const int32_t KEY_BACKSPACE = 259;
		static const int32_t KEY_INSERT = 260;
		static const int32_t KEY_DELETE = 261;
		static const int32_t KEY_RIGHT = 262;
		static const int32_t KEY_LEFT = 263;
		static const int32_t KEY_DOWN = 264;
		static const int32_t KEY_UP = 265;
		static const int32_t KEY_PAGE_UP = 266;
		static const int32_t KEY_PAGE_DOWN = 267;
		static const int32_t KEY_HOME = 268;
		static const int32_t KEY_END = 269;
		static const int32_t KEY_CAPS_LOCK = 280;
		static const int32_t KEY_SCROLL_LOCK = 281;
		static const int32_t KEY_NUM_LOCK = 282;
		static const int32_t KEY_PRINT_SCREEN = 283;
		static const int32_t KEY_PAUSE = 284;
		static const int32_t KEY_F1 = 290;
		static const int32_t KEY_F2 = 291;
		static const int32_t KEY_F3 = 292;
		static const int32_t KEY_F4 = 293;
		static const int32_t KEY_F5 = 294;
		static const int32_t KEY_F6 = 295;
		static const int32_t KEY_F7 = 296;
		static const int32_t KEY_F8 = 297;
		static const int32_t KEY_F9 = 298;
		static const int32_t KEY_F10 = 299;
		static const int32_t KEY_F11 = 300;
		static const int32_t KEY_F12 = 301;
		static const int32_t KEY_F13 = 302;
		static const int32_t KEY_F14 = 303;
		static const int32_t KEY_F15 = 304;
		static const int32_t KEY_F16 = 305;
		static const int32_t KEY_F17 = 306;
		static const int32_t KEY_F18 = 307;
		static const int32_t KEY_F19 = 308;
		static const int32_t KEY_F20 = 309;
		static const int32_t KEY_F21 = 310;
		static const int32_t KEY_F22 = 311;
		static const int32_t KEY_F23 = 312;
		static const int32_t KEY_F24 = 313;
		static const int32_t KEY_F25 = 314;
		static const int32_t KEY_KP_0 = 320;
		static const int32_t KEY_KP_1 = 321;
		static const int32_t KEY_KP_2 = 322;
		static const int32_t KEY_KP_3 = 323;
		static const int32_t KEY_KP_4 = 324;
		static const int32_t KEY_KP_5 = 325;
		static const int32_t KEY_KP_6 = 326;
		static const int32_t KEY_KP_7 = 327;
		static const int32_t KEY_KP_8 = 328;
		static const int32_t KEY_KP_9 = 329;
		static const int32_t KEY_KP_DECIMAL = 330;
		static const int32_t KEY_KP_DIVIDE = 331;
		static const int32_t KEY_KP_MULTIPLY = 332;
		static const int32_t KEY_KP_SUBTRACT = 333;
		static const int32_t KEY_KP_ADD = 334;
		static const int32_t KEY_KP_ENTER = 335;
		static const int32_t KEY_KP_EQUAL = 336;
		static const int32_t KEY_LEFT_SHIFT = 340;
		static const int32_t KEY_LEFT_CONTROL = 341;
		static const int32_t KEY_LEFT_ALT = 342;
		static const int32_t KEY_LEFT_SUPER = 343;
		static const int32_t KEY_RIGHT_SHIFT = 344;
		static const int32_t KEY_RIGHT_CONTROL = 345;
		static const int32_t KEY_RIGHT_ALT = 346;
		static const int32_t KEY_RIGHT_SUPER = 347;
		static const int32_t KEY_MENU = 348;
	};
}


