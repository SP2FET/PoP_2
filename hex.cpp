// POP 2017 01 20 projekt 2 Cichocki Bartosz AIR 2 165355 Srodowisko: Dev-C++ 5.10 Kompilator: TDM-GCC 4.8.1 64-bit Release
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>

using namespace std;

////////////////////////////////////// STA�E //////////////////////////////////////

const short menu_length = 79; //d�ugo�� linii menu i wy�wietlanego tekstu
const short lines_per_page = 5;
const char menu_char = '#';

const char text_mode_key = 'T';
const char hex_mode_key = 'H';
const char exit_key = 'Q';
const char new_file_key = 'O';
const char reopen_key = 'R';
const char next_page_key = '.';
const char prev_page_key = ',';

///////////////////////////////////////////////////////////////////////////////////

struct input_data_t
{
	bool hex_mode; 		//flaga aktualnego trybu
	bool file_opened;
	string file_name;
	fstream file;
	double file_length; //d�ugo�� pliku w znakach
	long lines;
	int pages;
	int current_page;	
};

//////////////////////////////// PROTOTYPY FUNKCJI ////////////////////////////////

inline void draw_line(char character, short length);
inline void draw_empty_line (short length);
void draw_menu(input_data_t* input_data);

void open_file_dialog(input_data_t* input_data);
inline void open_file(input_data_t* input_data);
inline void close_file(input_data_t* input_data);
inline void file_reload(input_data_t* input_data);

void display_text(input_data_t* input_data);
void display_hex(input_data_t* input_data);
void set_display_page_hex(input_data_t* input_data);
void set_display_page_text(input_data_t* input_data);
void parse_keys(input_data_t* input_data);
void program(input_data_t* input_data);


///////////////////////////////////////////////////////////////////////////////////

int main()
{
	input_data_t* input_data = new input_data_t;
	
	input_data->hex_mode = true;
	input_data->file_opened = false;
	input_data->current_page = 1;
	
	program(input_data); 
	
	delete input_data;
}

/*
Funkcja g��wna programu
*/

void program(input_data_t* input_data)
{
	draw_menu(input_data);
	
		if(input_data->file_opened == false) 
		{
			open_file_dialog(input_data);
			program(input_data);
		}
		
	if(input_data->hex_mode) display_hex(input_data);
	else display_text(input_data);
		
	parse_keys(input_data);
}

/*
Funkcja rysuj�ca lini� znak�w w konsoli
*/

inline void draw_line(char character, short length)
{
	for(short i = 0; i < length; i++) cout << character; 
}

/*
Funkcja rysuj�ca pusty wiersz zako�czony znakami
*/

inline void draw_empty_line (short length)
{
	cout<<menu_char;
	draw_line(' ', length - 2);
	cout<<menu_char;
}

/*
Funkcja wy�wietlaj�ca menu w konsoli
*/

void draw_menu(input_data_t* input_data)
{
	system("cls");
	short name_length = input_data->file_name.length();
	short spaces = menu_length - name_length - 16; 		//obliczanie ilo�ci spacji do zako�czenia wiersza znakiem
	draw_line(menu_char, menu_length);
	cout<<endl;
	draw_empty_line(menu_length);
	cout<<endl;
		
	if (input_data->file_opened) 
		{
			
			cout<<menu_char<<" "<<"Nazwa pliku: "<<input_data->file_name;
			for(short i = 0; i < spaces; i++) cout<<" "; //uzupe�nienie linijki spacjami
			cout<<menu_char<<endl;
			draw_empty_line(menu_length);
			cout<<endl;
			if(input_data->hex_mode) cout<<menu_char<<"                      Tryb wyswietlania: Hex + tekst"<<"                         "<<menu_char<<endl;
			else 				 	 cout<<menu_char<<"                            Tryb wyswietlania: Tekst"<<"                         "<<menu_char<<endl;
		}    
	else
		{  
			cout<<menu_char<<"                            Brak otwartego pliku!                            "<<menu_char<<endl;
		}
	
	
	draw_empty_line(menu_length);
	cout<<endl;
	cout<<menu_char<<"          "<<"Tryb tekst: "<<text_mode_key<<"  Tryb hex + tekst: "<<hex_mode_key<<"  Dalej: "<<next_page_key<<"  Wstecz: "<<prev_page_key;
			   cout<<"            "<<menu_char<<endl;	
	cout<<menu_char<<"          "<<"Otworz nowy plik: "<<new_file_key<<"  Wczytaj plik ponownie: "<<reopen_key<<" Wyjscie: "<<exit_key<<"           "<<menu_char<<endl;
	draw_empty_line(menu_length);
	cout<<endl;
	draw_line(menu_char, menu_length);
	cout<<endl;
}

/*
Funkcja odpowiadaj�ca za obs�ug� otwierania pliku
*/

void open_file_dialog(input_data_t* input_data)
{	
	cout<<endl<<"Podaj nazwe pliku [z rozszerzeniem] >";
	cin>>input_data->file_name;
	if (input_data->file_name.compare("Q") == 0 || input_data->file_name.compare("q") == 0) exit(0);
	open_file(input_data);
	
	if(input_data->file_opened == false)
	{
		cerr<<"Blad otwarcia pliku!!!   Sprobuj ponownie"<<endl;
		open_file_dialog(input_data);
		
 	}
}

/*
Funkcja otwieraj�ca plik
*/

inline void open_file(input_data_t* input_data)
{
	input_data->file.open(input_data->file_name.c_str(), ios::in | ios::binary);

	if(input_data->file) input_data->file_opened = true;
	else input_data->file_opened = false;	
	
	input_data->file.seekg(0,ios::end);					//
	input_data->file_length = input_data->file.tellg();	// pobieranie d�ugo�ci pliku
	input_data->file.seekg(0);							//
}

/*
Funkcja zamykaj�ca plik
*/

inline void close_file(input_data_t* input_data)
{
	input_data->file.close();
	input_data->file.clear();
	input_data->file_opened = false;
}

/*
Funkcja wy�wietlaj�ca tre�� pliku w trybie tekstowym
*/

void display_text(input_data_t* input_data)
{
	char character;
	short chars = 0; //ilo�� znak�w w wierszu
	int lines = 0;   //liczba wy�wietlonych linii


	set_display_page_text(input_data); //ustawienie odpowiedniej strony do wy�wietlenia
	
	
	cout<<"Tresc pliku:"<<endl<<endl;
	while( input_data->file.get(character) )
	{
		cout<<character;
		chars++;
		
		if(chars >= menu_length) //prze�amanie wiersza
		{
			lines++;
			if(lines >= lines_per_page) break;
			cout<<endl;
			chars = 0;
			
		}else if ( character == 10 ) //obs�uga LF z pliku
		{
			chars = 0;
			lines++;		
		}
			
		if(lines >= lines_per_page) break; //ograniczenie wy�wietlanych linii
	}

	input_data->file.clear();
	input_data->file.seekg(0);
	cout<<endl<<endl<<"Strona "<<input_data->current_page<<"/"<<input_data->pages<<endl;
}

/*
Funkcja ustawiaj�ca aktualnie wy�wietlan� stron� w trybie tekstowym
*/

void set_display_page_text(input_data_t* input_data)
{
	int chars = 0;
	char character;
	input_data->lines = 0; //ustawienie pocz�tkowej ilo�ci linii
	
	
	input_data->file.seekg(0); 							
	while( input_data->file.get(character) ) 	//zliczanie ilo�ci linii
	{								
		chars++;
		if(chars >= menu_length || character == 10 ) 
		{
			chars = 0;
			input_data->lines++;
		}		
	}
	input_data->file.clear();
	input_data->file.seekg(0);
	

	input_data->pages = ceil(input_data->lines / (float)lines_per_page); //obliczanie ilo�ci stron w pliku
	
	if (input_data->current_page > input_data->pages) input_data->current_page = input_data->pages; // zabezpieczenie przez wyj�ciem poza zakres stron
	if (input_data->current_page < 1) input_data->current_page = 1;
	
	
	input_data->lines = 0;
	while( (input_data->current_page != 1) && input_data->file.get(character) ) //ustawienie odpowiedniej linii w tek�cie
	{
		chars++;
		if(chars >= menu_length || character == 10 ) 
		{
			chars = 0;
			input_data->lines++;
		}
		
		if( input_data->lines >= lines_per_page*(input_data->current_page - 1) ) break; //dop�ki nie zostan� pomini�te wszystkie linie przed lini� na danej stronie
	}
}

/*
Funkcja ustawiaj�ca aktualnie wy�wietlan� stron� w trybie tekstowym
*/

void set_display_page_hex(input_data_t* input_data)
{
	int chars_per_page = lines_per_page * 16; //ilo�� znak�w na stronie
	long page_pointer; // ilo�� znak�w przed aktualn� stron�
	
	input_data->lines = ceil(input_data->file_length / 16); 
	input_data->pages = ceil(input_data->lines / (float)lines_per_page);
	
	if (input_data->current_page > input_data->pages) input_data->current_page = input_data->pages;
	if (input_data->current_page < 1) input_data->current_page = 1;
	
	page_pointer = chars_per_page*(input_data->current_page - 1); 
	input_data->file.seekg(page_pointer*sizeof(char)); //ustawienie wska�nika odczytu z pliku na now� stron�
}

/*
Funkcja ustawiaj�ca aktualnie wy�wietlan� stron� w trybie szesnastkowym
*/

void display_hex(input_data_t* input_data)
{
	char character;
	char buffer[16]; //bufor do wy�wietlania napis�w w postaci ASCII
	bool end = false; //koniec znak�w we wczytywanym pliku
	int page;

	cout<<endl<<"Adres"<<setw(7);
	for(short i = 0; i < 16; i++) cout<<hex<<uppercase<<i<<setw(3); //wy�wietlenie kolejnych bajt�w adresu szesnastkowo w nag��wku
	cout<<setw(8)<<"Tekst"<<endl<<endl;

	set_display_page_hex(input_data); //ustawienie aktualnie wy�wietlanej strony 
	page = input_data->current_page; //aktualna strona
	
	
	for(int j = lines_per_page * (page - 1); j < lines_per_page * page; j++ ) //p�tla wy�wietlaj�ca kolejne linie z uwzgl�dnieniem adresu i aktualnej strony
	{
		if(end) break;
		cout<<setw(8)<<setfill('0')<<hex<<j*16<<' '; // adres szesnastkowo
		cout.fill(' ');
		
		for(short i = 0; i < 16; i++) //wy�wietlanie linii
		{
			if( input_data->file.read(&character,sizeof(char)) )
			{	
				if (character < 0) // obs�uga znak�w nie wyst�puj�cych w tablicy ASCII (np. UNICODE lub inne kodowania)
					cout<<setw(3)<<"FF";
				else if(character <= 16) // dodanie zera poprzedzaj�cego liczby 4-bitowe
					cout<<setw(2)<<'0'<<(int)character;
				else
					cout<<setw(3)<<(int)character;
					
				if(character < 32) character = ' '; //pomini�cie znak�w steruj�cych
				buffer[i] = character;			//zapis aktualnego znaku do bufora
			}
			else //gdy koniec pliku
			{
				cout<<setw(3)<<' ';
				buffer[i] = ' ';
				end = true;				
			}
		}
		
		buffer[16] = 0; //zrobienie z bufora C-stringa
		cout<<"   "<<buffer<<endl;	//wy�wietlenie bufora za znakami szesnastkowymi
	}
	
	input_data->file.clear();
	input_data->file.seekg(0);
	cout<<endl<<dec<<"Strona "<<input_data->current_page<<"/"<<input_data->pages<<endl;
}

/*
Funkcja-makro wczytuj�ca ponownie plik
*/

inline void file_reload(input_data_t* input_data)
{
	close_file(input_data);
	open_file(input_data);
}

/*
Funkcja parsuj�ca wpisywane komendy
*/

void parse_keys(input_data_t* input_data)
{
	char key, key_upper;
	cout<<"Wybierz opcje >";

	cin>>key;
	cin.sync();
	key_upper = toupper(key); // dla obs�ugi ma�ych i wielkich liter
	
	switch(key_upper)
	{
		case text_mode_key: input_data->hex_mode = false;
							input_data->current_page = 1;
							break;
							
		case hex_mode_key:  input_data->hex_mode = true;
							input_data->current_page = 1;
							break;
							
		case new_file_key:  close_file(input_data);							
							break;
							
		case reopen_key:    file_reload(input_data);
							break;
							
		case next_page_key: input_data->current_page++;
							break;
							
		case prev_page_key: input_data->current_page--;
							break;
																	
		case exit_key:		exit(0);
							break;
							
		default:			parse_keys(input_data);
							
	}
	
	program(input_data); //powr�t do programu - zap�tlenie
}
