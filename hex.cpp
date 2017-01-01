#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

const short menu_length = 79;
const char menu_char = '#';

const char text_mode_key = 'T';
const char hex_mode_key = 'H';
const char exit_key = 'Q';
const char new_file_key = 'O';
const char reopen_key = 'R';

struct status_t
{
	bool hex_mode;
	bool file_opened;
	string file_name;
	fstream file;
	
};

inline void draw_line(char character, short length);
inline void draw_empty_line (short length);
void draw_menu(status_t* status);
void open_file_dialog(status_t* status);
inline void open_file(status_t* status);
inline void close_file(status_t* status);
void display_text(status_t* status);
void display_hex(status_t* status);
void parse_keys(status_t* status);
void program(status_t* status);

int main()
{
	status_t* status = new status_t;
	
	status->hex_mode = false;
	status->file_opened = false;
	
	program(status);
	
	delete status;
}

void program(status_t* status)
{
	draw_menu(status);
	
		if(status->file_opened == false) 
		{
			open_file_dialog(status);
			program(status);
		}
		
		if(status->hex_mode) display_hex(status);
		else display_text(status);
		
	parse_keys(status);
}

inline void draw_line(char character, short length)
{
	for(short i = 0; i < length ;i++) cout << character; 
}

inline void draw_empty_line (short length)
{
	cout<<menu_char;
	draw_line(' ', length-2);
	cout<<menu_char;
}

void draw_menu(status_t* status)
{
	system("cls");
	short name_length = status->file_name.length();
	short spaces = menu_length - name_length - 16;
	draw_line(menu_char, menu_length);
	cout<<endl;
	draw_empty_line(menu_length);
	cout<<endl;
		
	if (status->file_opened) 
		{
			
			cout<<menu_char<<" "<<"Nazwa pliku: "<<status->file_name;
			for(short i = 0; i < spaces; i++) cout<<" ";
			cout<<menu_char<<endl;
			draw_empty_line(menu_length);
			cout<<endl;
			if(status->hex_mode) cout<<menu_char<<"                      Tryb wyswietlania: Hex + tekst"<<"                   "<<menu_char<<endl;
			else cout<<menu_char<<"                            Tryb wyswietlania: Tekst"<<"                         "<<menu_char<<endl;
		}    
	else
		{  
			cout<<menu_char<<"                            Brak otwartego pliku!                            "<<menu_char<<endl;
		}
	
	
	draw_empty_line(menu_length);
	cout<<endl;
	cout<<menu_char<<"       "<<"Tryb tekst: "<<text_mode_key<<"  Tryb hex + tekst: "<<hex_mode_key<<"  Dalej: PgDn  Wstecz: PgUp         "<<menu_char<<endl;	
	cout<<menu_char<<"          "<<"Otworz nowy plik: "<<new_file_key<<"  Wczytaj plik ponownie: "<<reopen_key<<" Wyjscie: "<<exit_key<<"           "<<menu_char<<endl;
	draw_empty_line(menu_length);
	cout<<endl;
	draw_line(menu_char, menu_length);
	cout<<endl;
}

void open_file_dialog(status_t* status)
{	
	cout<<endl<<"Podaj nazwe pliku [z rozszerzeniem] >";
	cin>>status->file_name;
	if (status->file_name.compare("Q") == 0 || status->file_name.compare("q") == 0) exit(0);
	open_file(status);
	
	if(status->file_opened == false)
	{
		cerr<<"Blad otwarcia pliku!!!   Sprobuj ponownie"<<endl;
		open_file_dialog(status);
		
 	}
}

inline void open_file(status_t* status)
{
	if(status->hex_mode) status->file.open(status->file_name.c_str(), ios::in | ios::binary);
	else status->file.open(status->file_name.c_str(), ios::in);
	
	if(status->file) status->file_opened = true;
	else status->file_opened = false;	
}

inline void close_file(status_t* status)
{
	status->file.close();
	status->file.clear();
	status->file_opened = false;
}

void display_text(status_t* status)
{
	char *character = new char;
	short chars;
	chars = 0;
	cout<<"Tresc pliku:"<<endl<<endl;
	
	while(status->file.get(*character))
	{
		cout<<*character;
		chars++;
		if(chars >= menu_length) 
		{
			cout<<endl;
			chars = 0;
		}else if ( *character == 10 ) chars = 0;	
	}
	cout<<endl;
}

void display_hex(status_t* status)
{
}

void parse_keys(status_t* status)
{
	char key, key_upper;
	cout<<">";

	cin>>key;
	cin.sync();
	key_upper = toupper(key); // dla obs³ugi ma³ych i wielkich liter
	
	switch(key_upper)
	{
		case text_mode_key: status->hex_mode = false;
							break;
		case hex_mode_key:  status->hex_mode = true;
							break;
		case new_file_key:  close_file(status);							
							break;
		case reopen_key:    close_file(status);
							open_file(status);
							break;
		case exit_key:		exit(0);
							break;
		default:			parse_keys(status);
							
	}
	program(status);
}
