#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;


struct row {
  bool* __data;
  unsigned long long __size;

  void resize(unsigned long long _size){
    bool* tmp = new bool[_size];
    copy(__data,__data+__size,tmp);
    tmp[_size-1] = 0;
    delete[] __data;
    __data = tmp;
    __size = _size;
  }

  unsigned long long size(){
    return __size;
  }

  bool& operator[](unsigned long long index){
    while (index+1 > size()){
      resize(index+1);
    }
    return __data[index];
  }

  row () {
    // cout << "initialize array (row)" << endl;
    __data = NULL;
    __size = 0;
  }

  ~row () {
    // cout << "unload array (row)" << endl;
  }
};


struct array {
  row* __data;
  unsigned long long __size;

  void resize(unsigned long long _size){
		row* tmp = new row[_size];
		copy(__data,__data+__size,tmp);
		delete[] __data;
		__data = tmp;
		__size = _size;
	}

  unsigned long long size(){
		return __size;
	}

	row& operator[](unsigned long long index){
		while (index+1 > __size){
			resize(index+1);
		}
		return __data[index];
	}

  array () {
    // cout << "initialize array (map)" << endl;
    __data = NULL;
    __size = 0;
  }

  ~array () {
    // cout << "unload array (map)" << endl;
  }
};

void drawSyntaxError (int line, bool &error) {
  error = true;
  cout << "Syntax error!" << endl << "Line: " << line + 1 << endl;
  system ("pause");
}


char optionAndValidate (string input, unsigned int line, bool &error) {
  char option;
  char inputC[10];
  strcpy (inputC, input.c_str());
  option = inputC[0];

  switch (option) {

    case 'D':
      if (!(inputC[1] == 'O' && inputC[2] == 'W' && inputC[3] == 'N') || inputC[4] != 0) {
        drawSyntaxError (line, error);
      }
      break;

    case 'U':
      if (!(inputC[1] == 'P') || inputC[2] != 0) {
        drawSyntaxError (line, error);
      }
      break;

    case 'X':
      if (inputC[1] == ' ') {
        for (size_t i = 2; inputC[i] && !error; i++) {
          if (!(inputC[i] - '0' >= 0 && inputC[i] - '0' <= 9) && !(i == 2 && inputC[i] == '-')) {
            drawSyntaxError (line, error);
          }
        }
      } else {
        drawSyntaxError (line, error);
      }
      break;

    case 'Y':
      if (inputC[1] == ' ') {
        for (size_t i = 2; inputC[i] && !error; i++) {
          if (!(inputC[i] - '0' >= 0 && inputC[i] - '0' <= 9) && !(i == 2 && inputC[i] == '-')) {
            drawSyntaxError (line, error);
          }
        }
      } else {
        drawSyntaxError (line, error);
      }

      break;

    default :
      cout << line+1;
      drawSyntaxError (line, error);
      break;
  }

  return option;
}


void loadDataFromFile (array &map) {
  ifstream file("test1.txt");
  if (!file) {
    cout << "error while opening input file" << endl;
    system ("pause");
    return;
  }

  string input;
  int x = 0;
  int y = 0;
  char action;
  bool isPenDown = false;
  bool error = false;


  for (size_t line = 0; getline(file, input) && !error; line++) {
    action = optionAndValidate (input, line, error);
    switch (action) {
      case 'D':
        isPenDown = true;
        break;

      case 'U':
        isPenDown = false;
        break;

      case 'X': {
        char inputC[10];
        strcpy (inputC, input.c_str());
        double distance = 0;
        bool sign = false;  // + false, - true
        for (int i = 2; inputC[i]; i++) {
          if (inputC[i] == '-') {
            sign = true;
          } else {
            distance *= 10;
            distance += (inputC[i]-'0');
          }
        }
        distance *= pow(-1.0, sign);

        cout << "ispendown: " << isPenDown << " x: " << x << " y: " << y << endl;
        if (isPenDown) {
          if (distance >= 0) {
            for (size_t i = 0; i <= distance; i++) {
              map[y][x+i] = true;
            }
          } else {
            for (size_t i = 0; i <= distance*(-1); i++) {
              map[y][x-i] = true;
            }
          }
        }
        x += (int)distance;

        cout << "move x: " << distance << endl;
        break;
      }


      case 'Y': {
        char inputC[10];
        strcpy (inputC, input.c_str());
        double distance = 0;
        bool sign = false;  // + false, - true
        for (int i = 2; inputC[i]; i++) {
          if (inputC[i] == '-') {
            sign = true;
          } else {
            distance *= 10;
            distance += (inputC[i]-'0');
          }
        }
        distance *= pow(-1.0, sign);

        cout << "ispendown: " << isPenDown << " x: " << x << " y: " << y << endl;
        if (isPenDown) {
          if (distance >= 0) {
            for (size_t i = 0; i <= distance; i++) {
              map[y+i][x] = true;
            }
          } else {
            for (size_t i = 0; i <= distance*(-1); i++) {
              map[y-i][x] = true;
            }
          }
        }
        y += (int)distance;

        cout << "move y: " << distance << endl;
        break;
      }

    }
  }

  file.close();
}


int main(int argc, char const *argv[]) {
  array map;
  // z tym dziala :
   for (size_t y = 0; y < 20; y++) {
     for (size_t x = 0; x < 20; x++) {
       map[y][x] = 0;
     }
   }
  loadDataFromFile(map);

  for (size_t y = 0; y < map.size(); y++) {
    for (size_t x = 0; x < map[y].size(); x++) {
      cout << map[y][x] << " ";
    }
    cout << endl;
  }

  return 0;
}
