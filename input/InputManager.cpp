#include "InputManager.hpp"

InputManager::InputManager() :
	pressed(101,false),
	released(101,false),
	down(100,false){
	keys[0] = sf::Keyboard::A;            ///< The A key
	keys[1] = sf::Keyboard::B;            ///< The B key
	keys[2] = sf::Keyboard::C;            ///< The C key
	keys[3] = sf::Keyboard::D;            ///< The D key
	keys[4] = sf::Keyboard::E;            ///< The E key
	keys[5] = sf::Keyboard::F;            ///< The F key
	keys[6] = sf::Keyboard::G;            ///< The G key
	keys[7] = sf::Keyboard::H;            ///< The H key
	keys[8] = sf::Keyboard::I;            ///< The I key
	keys[9] = sf::Keyboard::J;            ///< The J key
	keys[10] = sf::Keyboard::K;            ///< The K key
	keys[11] = sf::Keyboard::L;            ///< The L key
	keys[12] = sf::Keyboard::M;            ///< The M key
	keys[13] = sf::Keyboard::N;            ///< The N key
	keys[14] = sf::Keyboard::O;            ///< The O key
	keys[15] = sf::Keyboard::P;            ///< The P key
	keys[16] = sf::Keyboard::Q;            ///< The Q key
	keys[17] = sf::Keyboard::R;            ///< The R key
	keys[18] = sf::Keyboard::S;            ///< The S key
	keys[19] = sf::Keyboard::T;            ///< The T key
	keys[20] = sf::Keyboard::U;            ///< The U key
	keys[21] = sf::Keyboard::V;            ///< The V key
	keys[22] = sf::Keyboard::W;            ///< The W key
	keys[23] = sf::Keyboard::X;            ///< The X key
	keys[24] = sf::Keyboard::Y;            ///< The Y key
	keys[25] = sf::Keyboard::Z;            ///< The Z key
	keys[26] = sf::Keyboard::Num0;         ///< The 0 key
	keys[27] = sf::Keyboard::Num1;         ///< The 1 key
	keys[28] = sf::Keyboard::Num2;         ///< The 2 key
	keys[29] = sf::Keyboard::Num3;         ///< The 3 key
	keys[30] = sf::Keyboard::Num4;         ///< The 4 key
	keys[31] = sf::Keyboard::Num5;         ///< The 5 key
	keys[32] = sf::Keyboard::Num6;         ///< The 6 key
	keys[33] = sf::Keyboard::Num7;         ///< The 7 key
	keys[34] = sf::Keyboard::Num8;         ///< The 8 key
	keys[35] = sf::Keyboard::Num9;         ///< The 9 key
	keys[36] = sf::Keyboard::Escape;       ///< The Escape key
	keys[37] = sf::Keyboard::LControl;     ///< The left Control key
	keys[38] = sf::Keyboard::LShift;       ///< The left Shift key
	keys[39] = sf::Keyboard::LAlt;         ///< The left Alt key
	keys[40] = sf::Keyboard::LSystem;      ///< The left OS specific key: window (Windows and Linux) apple (MacOS X) ...
	keys[41] = sf::Keyboard::RControl;     ///< The right Control key
	keys[42] = sf::Keyboard::RShift;       ///< The right Shift key
	keys[43] = sf::Keyboard::RAlt;         ///< The right Alt key
	keys[44] = sf::Keyboard::RSystem;      ///< The right OS specific key: window (Windows and Linux) apple (MacOS X) ...
	keys[45] = sf::Keyboard::Menu;         ///< The Menu key
	keys[46] = sf::Keyboard::LBracket;     ///< The [ key
	keys[47] = sf::Keyboard::RBracket;     ///< The ] key
	keys[48] = sf::Keyboard::SemiColon;    ///< The ; key
	keys[49] = sf::Keyboard::Comma;        ///< The , key
	keys[50] = sf::Keyboard::Period;       ///< The . key
	keys[51] = sf::Keyboard::Quote;        ///< The ' key
	keys[52] = sf::Keyboard::Slash;        ///< The / key
	keys[53] = sf::Keyboard::BackSlash;    ///< The \ key
	keys[54] = sf::Keyboard::Tilde;        ///< The ~ key
	keys[55] = sf::Keyboard::Equal;        ///< The = key
	keys[56] = sf::Keyboard::Dash;         ///< The - key
	keys[57] = sf::Keyboard::Space;        ///< The Space key
	keys[58] = sf::Keyboard::Return;       ///< The Return key
	keys[59] = sf::Keyboard::Back;         ///< The Backspace key
	keys[60] = sf::Keyboard::Tab;          ///< The Tabulation key
	keys[61] = sf::Keyboard::PageUp;       ///< The Page up key
	keys[62] = sf::Keyboard::PageDown;     ///< The Page down key
	keys[63] = sf::Keyboard::End;          ///< The End key
	keys[64] = sf::Keyboard::Home;         ///< The Home key
	keys[65] = sf::Keyboard::Insert;       ///< The Insert key
	keys[66] = sf::Keyboard::Delete;       ///< The Delete key
	keys[67] = sf::Keyboard::Add;          ///< +
	keys[68] = sf::Keyboard::Subtract;     ///< -
	keys[69] = sf::Keyboard::Multiply;     ///< *
	keys[70] = sf::Keyboard::Divide;       ///< /
	keys[71] = sf::Keyboard::Left;         ///< Left arrow
	keys[72] = sf::Keyboard::Right;        ///< Right arrow
	keys[73] = sf::Keyboard::Up;           ///< Up arrow
	keys[74] = sf::Keyboard::Down;         ///< Down arrow
	keys[75] = sf::Keyboard::Numpad0;      ///< The numpad 0 key
	keys[76] = sf::Keyboard::Numpad1;      ///< The numpad 1 key
	keys[77] = sf::Keyboard::Numpad2;      ///< The numpad 2 key
	keys[78] = sf::Keyboard::Numpad3;      ///< The numpad 3 key
	keys[79] = sf::Keyboard::Numpad4;      ///< The numpad 4 key
	keys[80] = sf::Keyboard::Numpad5;      ///< The numpad 5 key
	keys[81] = sf::Keyboard::Numpad6;      ///< The numpad 6 key
	keys[82] = sf::Keyboard::Numpad7;      ///< The numpad 7 key
	keys[83] = sf::Keyboard::Numpad8;      ///< The numpad 8 key
	keys[84] = sf::Keyboard::Numpad9;      ///< The numpad 9 key
	keys[85] = sf::Keyboard::F1;           ///< The F1 key
	keys[86] = sf::Keyboard::F2;           ///< The F2 key
	keys[87] = sf::Keyboard::F3;           ///< The F3 key
	keys[88] = sf::Keyboard::F4;           ///< The F4 key
	keys[89] = sf::Keyboard::F5;           ///< The F5 key
	keys[90] = sf::Keyboard::F6;           ///< The F6 key
	keys[91] = sf::Keyboard::F7;           ///< The F7 key
	keys[92] = sf::Keyboard::F8;           ///< The F8 key
	keys[93] = sf::Keyboard::F9;           ///< The F8 key
	keys[94] = sf::Keyboard::F10;          ///< The F10 key
	keys[95] = sf::Keyboard::F11;          ///< The F11 key
	keys[96] = sf::Keyboard::F12;          ///< The F12 key
	keys[97] = sf::Keyboard::F13;          ///< The F13 key
	keys[98] = sf::Keyboard::F14;          ///< The F14 key
	keys[99] = sf::Keyboard::F15;          ///< The F15 key
	keys[100] = sf::Keyboard::Pause;        ///< The Pause key
}

InputManager::~InputManager() {

}

void InputManager::pressKey(sf::Keyboard::Key key) {
	pressed[key] = true;
	down[key] = true;
}

void InputManager::releaseKey(sf::Keyboard::Key key) {
	released[key] = true;
	down[key] = false;
}

void InputManager::update() {
	pressed = std::vector<bool>(100,false);
	released = std::vector<bool>(100,false);
}

bool InputManager::keyPressed(sf::Keyboard::Key key) {
	return pressed[key];
}

bool InputManager::keyDown(sf::Keyboard::Key key) {
	return down[key];
}

bool InputManager::keyReleased(sf::Keyboard::Key key) {
	return released[key];
}

