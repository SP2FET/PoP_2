// POP 2017 01 20 projekt 2 Cichocki Bartosz AIR 2 165355 Srodowisko: Dev-C++ 5.10 Kompilator: TDM-GCC 4.8.1 64-bit Release
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>

using namespace std;

////////////////////////////////////// STA£E //////////////////////////////////////

const short menu_length = 79; //d³ugoœæ linii menu i wyœwietlanego tekstu
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
	double file_length; //d³ugoœæ pliku w znakach
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
Funkcja g³ówna programu
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
Funkcja rysuj¹ca liniê znaków w konsoli
*/

inline void draw_line(char character, short length)
{
	for(short i = 0; i < length; i++) cout << character; 
}

/*
Funkcja rysuj¹ca pusty wiersz zakoñczony znakami
*/

inline void draw_empty_line (short length)
{
	cout<<menu_char;
	draw_line(' ', length - 2);
	cout<<menu_char;
}

/*
Funkcja wyœwietlaj¹ca menu w konsoli
*/

void draw_menu(input_data_t* input_data)
{
	system("cls");
	short name_length = input_data->file_name.length();
	short spaces = menu_length - name_length - 16; 		//obliczanie iloœci spacji do zakoñczenia wiersza znakiem
	draw_line(menu_char, menu_length);
	cout<<endl;
	draw_empty_line(menu_length);
	cout<<endl;
		
	if (input_data->file_opened) 
		{
			
			cout<<menu_char<<" "<<"Nazwa pliku: "<<input_data->file_name;
			for(short i = 0; i < spaces; i++) cout<<" "; //uzupe³nienie linijki spacjami
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
Funkcja odpowiadaj¹ca za obs³ugê otwierania pliku
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
Funkcja otwieraj¹ca plik
*/

inline void open_file(input_data_t* input_data)
{
	input_data->file.open(input_data->file_name.c_str(), ios::in | ios::binary);

	if(input_data->file) input_data->file_opened = true;
	else input_data->file_opened = false;	
	
	input_data->file.seekg(0,ios::end);					//
	input_data->file_length = input_data->file.tellg();	// pobieranie d³ugoœci pliku
	input_data->file.seekg(0);							//
}

/*
Funkcja zamykaj¹ca plik
*/

inline void close_file(input_data_t* input_data)
{
	input_data->file.close();
	input_data->file.clear();
	input_data->file_opened = false;
}

/*
Funkcja wyœwietlaj¹ca treœæ pliku w trybie tekstowym
*/

void display_text(input_data_t* input_data)
{
	char character;
	short chars = 0; //iloœæ znaków w wierszu
	int lines = 0;   //liczba wyœwietlonych linii


	set_display_page_text(input_data); //ustawienie odpowiedniej strony do wyœwietlenia
	
	
	cout<<"Tresc pliku:"<<endl<<endl;
	while( input_data->file.get(character) )
	{
		cout<<character;
		chars++;
		
		if(chars >= menu_length) //prze³amanie wiersza
		{
			lines++;
			if(lines >= lines_per_page) break;
			cout<<endl;
			chars = 0;
			
		}else if ( character == 10 ) //obs³uga LF z pliku
		{
			chars = 0;
			lines++;		
		}
			
		if(lines >= lines_per_page) break; //ograniczenie wyœwietlanych linii
	}

	input_data->file.clear();
	input_data->file.seekg(0);
	cout<<endl<<endl<<"Strona "<<input_data->current_page<<"/"<<input_data->pages<<endl;
}

/*
Funkcja ustawiaj¹ca aktualnie wyœwietlan¹ stronê w trybie tekstowym
*/

void set_display_page_text(input_data_t* input_data)
{
	int chars = 0;
	char character;
	input_data->lines = 0; //ustawienie pocz¹tkowej iloœci linii
	
	
	input_data->file.seekg(0); 							
	while( input_data->file.get(character) ) 	//zliczanie iloœci linii
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
	

	input_data->pages = ceil(input_data->lines / (float)lines_per_page); //obliczanie iloœci stron w pliku
	
	if (input_data->current_page > input_data->pages) input_data->current_page = input_data->pages; // zabezpieczenie przez wyjœciem poza zakres stron
	if (input_data->current_page < 1) input_data->current_page = 1;
	
	
	input_data->lines = 0;
	while( (input_data->current_page != 1) && input_data->file.get(character) ) //ustawienie odpowiedniej linii w tekœcie
	{
		chars++;
		if(chars >= menu_length || character == 10 ) 
		{
			chars = 0;
			input_data->lines++;
		}
		
		if( input_data->lines >= lines_per_page*(input_data->current_page - 1) ) break; //dopóki nie zostan¹ pominiête wszystkie linie przed lini¹ na danej stronie
	}
}

/*
Funkcja ustawiaj¹ca aktualnie wyœwietlan¹ stronê w trybie tekstowym
*/

void set_display_page_hex(input_data_t* input_data)
{
	int chars_per_page = lines_per_page * 16; //iloœæ znaków na stronie
	long page_pointer; // iloœæ znaków przed aktualn¹ stron¹
	
	input_data->lines = ceil(input_data->file_length / 16); 
	input_data->pages = ceil(input_data->lines / (float)lines_per_page);
	
	if (input_data->current_page > input_data->pages) input_data->current_page = input_data->pages;
	if (input_data->current_page < 1) input_data->current_page = 1;
	
	page_pointer = chars_per_page*(input_data->current_page - 1); 
	input_data->file.seekg(page_pointer*sizeof(char)); //ustawienie wskaŸnika odczytu z pliku na now¹ stronê
}

/*
Funkcja ustawiaj¹ca aktualnie wyœwietlan¹ stronê w trybie szesnastkowym
*/

void display_hex(input_data_t* input_data)
{
	char character;
	char buffer[16]; //bufor do wyœwietlania napisów w postaci ASCII
	bool end = false; //koniec znaków we wczytywanym pliku
	int page;

	cout<<endl<<"Adres"<<setw(7);
	for(short i = 0; i < 16; i++) cout<<hex<<uppercase<<i<<setw(3); //wyœwietlenie kolejnych bajtów adresu szesnastkowo w nag³ówku
	cout<<setw(8)<<"Tekst"<<endl<<endl;

	set_display_page_hex(input_data); //ustawienie aktualnie wyœwietlanej strony 
	page = input_data->current_page; //aktualna strona
	
	
	for(int j = lines_per_page * (page - 1); j < lines_per_page * page; j++ ) //pêtla wyœwietlaj¹ca kolejne linie z uwzglêdnieniem adresu i aktualnej strony
	{
		if(end) break;
		cout<<setw(8)<<setfill('0')<<hex<<j*16<<' '; // adres szesnastkowo
		cout.fill(' ');
		
		for(short i = 0; i < 16; i++) //wyœwietlanie linii
		{
			if( input_data->file.read(&character,sizeof(char)) )
			{	
				if (character < 0) // obs³uga znaków nie wystêpuj¹cych w tablicy ASCII (np. UNICODE lub inne kodowania)
					cout<<setw(3)<<"FF";
				else if(character <= 16) // dodanie zera poprzedzaj¹cego liczby 4-bitowe
					cout<<setw(2)<<'0'<<(int)character;
				else
					cout<<setw(3)<<(int)character;
					
				if(character < 32) character = ' '; //pominiêcie znaków steruj¹cych
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
		cout<<"   "<<buffer<<endl;	//wyœwietlenie bufora za znakami szesnastkowymi
	}
	
	input_data->file.clear();
	input_data->file.seekg(0);
	cout<<endl<<dec<<"Strona "<<input_data->current_page<<"/"<<input_data->pages<<endl;
}

/*
Funkcja-makro wczytuj¹ca ponownie plik
*/

inline void file_reload(input_data_t* input_data)
{
	close_file(input_data);
	open_file(input_data);
}

/*
Funkcja parsuj¹ca wpisywane komendy
*/

void parse_keys(input_data_t* input_data)
{
	char key, key_upper;
	cout<<"Wybierz opcje >";

	cin>>key;
	cin.sync();
	key_upper = toupper(key); // dla obs³ugi ma³ych i wielkich liter
	
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
	
	program(input_data); //powrót do programu - zapêtlenie
}
