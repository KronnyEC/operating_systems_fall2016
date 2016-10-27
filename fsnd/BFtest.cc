#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "blowfish.h"

using namespace std;

int main()
{
  Blowfish BF; // initialize

  string string;
  cout << "input: " << endl;
  getline(cin, string);

  BF.Encrypt(&string,8); // encrypt

  cout << "Encrypted" << endl;

  BF.Decrypt(&string,8); // decript

  cout << "Decrypted" << endl;

  cout << string << endl;
  return 0;
}
