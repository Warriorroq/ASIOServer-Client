#pragma once
#include <iostream>
#include <string>
#define WaitMiliseconds(time) std::this_thread::sleep_for(std::chrono::milliseconds(time));
#include <net_common.h>

using namespace std;
using namespace olc;
using namespace net;

template<typename T>
inline void Print(const T& text)
{
	cout << text;
}

template<typename T>
inline void PrintLine(const T& text)
{
	Print(text);
	Print('\n');
}

template<typename TOut>
inline void InputLine(const TOut& question, string& input)
{
	Print(question);
	getline(std::cin, input);
}

inline void InputStringToMessage(Message<CustomMessages>& message, string& str)
{
	for (int i = str.size() - 1; i >= 0; i--) {
		message << str[i];
	}
}