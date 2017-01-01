#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const short menu_length = 20;

inline void draw_line(char character);

void draw_menu(void);

int main()
{
	
}

inline void draw_line(char character)
{
	cout << ' ';
	for(short i = 0; i < menu_length ;i++) cout << character;
	cout<<endl; 
}
