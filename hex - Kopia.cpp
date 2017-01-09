#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>

using namespace std;

const short menu_length = 79;
const short max_per_page = 5;
const char menu_char = '#';

const char text_mode_key = 'T';
const char hex_mode_key = 'H';
const char exit_key = 'Q';
const char new_file_key = 'O';
const char reopen_key = 'R';
const char next_page_key = '.';
const char prev_page_key = ',';

struct status_t
{
	bool hex_mode;
	bool file_opened;
	string file_name;
	fstream file;
	long file_length;
	long lines;
	int pages;
	int current_page;
	
};

inline void draw_line(char character, short length);
inline void draw_empty_line (short length);
void draw_menu(status_t* status);
void open_file_dialog(status_t* status);
inline void open_file(status_t* status);
inline void close_file(status_t* status);
void display_text(status_t* status);
void display_hex(status_t* status);
void display_hex_page(status_t* status);
void set_display_page(status_t* status, int chars_per_page);
void parse_keys(status_t* status);
void program(status_t* status);
void file_reload(status_t* status);

int main()
{
	status_t* status = new status_t;
	
	status->hex_mode = true;
	status->file_opened = true;
	status->current_page = 1;
	
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
			if(status->hex_mode) cout<<menu_char<<"                      Tryb wyswietlania: Hex + tekst"<<"                         "<<menu_char<<endl;
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
	if(status->hex_mode) 
	{
		status->file.open(status->file_name.c_str(), ios::in | ios::binary);
		
		
	}
	else status->file.open(status->file_name.c_str(), ios::in);
	
	if(status->file) status->file_opened = true;
	else status->file_opened = false;	
	
	status->file.seekg(0,ios::end);
	status->file_length = status->file.tellg();
	status->file.seekg(0);
	
}

inline void close_file(status_t* status)
{
	status->file.close();
	status->file.clear();
	status->file_opened = false;
}

void display_text(status_t* status)
{
	char character;
	short chars;
	int lines = 0;
	chars = 0;
	int chars_per_page = menu_length*max_per_page;
	cout<<"Tresc pliku:"<<endl<<endl;
	
	
	set_display_page(status, chars_per_page);
	
	while( status->file.get(character) )
	{
		cout<<character;
		chars++;
		if(chars >= menu_length) 
		{
			cout<<endl;
			chars = 0;
			lines++;
		}else if ( character == 10 ) 
		{
			chars = 0;
			lines++;		
		}
		
		if(lines >= max_per_page) break;
	}
	status->file.clear();
	status->file.seekg(0);
	cout<<endl<<"Strona "<<status->current_page<<"/"<<status->pages<<endl;
}
void set_display_page(status_t* status, int chars_per_page)
{
	
	
	
	if (status->hex_mode)
	{
		status->lines = ceil(status->file_length/16.0);
		status->pages = ceil(status->lines/(float)max_per_page);
	}else
	{
		status->lines = ceil(status->file_length/(float)menu_length);
		status->pages = ceil(status->lines/(float)max_per_page);
	}
	

	
	if (status->current_page > status->pages) status->current_page = status->pages;
	if (status->current_page < 1) status->current_page = 1;
	int page_pointer = chars_per_page*(status->current_page-1);
	
	status->file.seekg(page_pointer*sizeof(char));
}
void display_hex_page(status_t* status)
{
	
	char character;
	char buffer[16];
	bool end = false;
	int page ;
	int chars_per_page = max_per_page * 16;
	
	set_display_page(status, chars_per_page);
	page = status->current_page;
	for(int j = max_per_page*(page-1); j < max_per_page*page; j++ )
	{
		if(end) break;
		cout<<setw(8)<<setfill('0')<<hex<<j*16<<' ';
		cout.fill(' ');
		
		for(short i = 0; i < 16; i++)
		{
			if( status->file.read(&character,sizeof(char)) )
			{	
				if(character <= 16) 
					cout<<setw(2)<<'0'<<(int)character;
				else 
					cout<<setw(3)<<(int)character;
					
				if(character < 32) character = ' ';
				buffer[i] = character;
						
			}
			else
			{
				cout<<setw(3)<<' ';
				buffer[i] = ' ';
				end = true;				
			}
		}
		
		buffer[16] = 0;
		cout<<"   "<<buffer<<endl;
		
		
	}
}
void file_reload(status_t* status)
{
	close_file(status);
	open_file(status);
}
void display_hex(status_t* status)
{

	
	
	cout<<endl<<"Adres"<<setw(7);
	for(short i = 0; i < 16; i++) cout<<hex<<uppercase<<i<<setw(3);
	cout<<setw(8)<<"Tekst"<<endl<<endl;
	
	
	
//	cout<<"Dlugosc: "<<dec<<status->file_length<<" Wierszy: "<<status->lines<<" Stron: "<<status->pages<<endl;
//	cout<<endl;
	
	display_hex_page(status);
	status->file.clear();
	status->file.seekg(0);
	cout<<endl<<"Strona "<<status->current_page<<"/"<<status->pages<<endl;
	cout<<endl;
}

void parse_keys(status_t* status)
{
	char key, key_upper;
	cout<<"Wybierz opcje >";

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
		case reopen_key:    file_reload(status);
							break;
		case next_page_key: status->current_page++;
							break;
		case prev_page_key: status->current_page--;
							break;										
		case exit_key:		exit(0);
							break;
		default:			parse_keys(status);
							
	}
	program(status);
}
