/*
 * main.cpp
 *
 * COMP 15 Project 2: Six Degrees of Collaboration
 * 
 * by [Jahansher Khan], [April 30 2021]
 * Purpose: Opens the necessary files and creates an instance of 
 * the sixdegrees class to run the program.
 */

#include <iostream>
#include <fstream>
#include "Artist.h"
#include "CollabGraph.h"
#include "SixDegrees.h"
using namespace std;


/* Open_or_die
*  Purpose: Checks if a file is valid 
*  Parameters: filestream, and argv[i]
*  Returns: N/A */
template <typename streamtype>
void open_or_die(streamtype &stream, string filename) {
    if (not stream.is_open()) {
            cerr << filename << " cannot be opened." << endl;
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
		cerr << "Usage: ./SixDegrees dataFile [commandFile] " <<
        "[outputFile]" << endl;
		exit(EXIT_FAILURE);
	}
    if (argc == 2) {
        ifstream infile;
        infile.open(argv[1]);
        open_or_die(infile, argv[1]);
        SixDegrees sixdegrees;
        sixdegrees.run(infile, cin, cout);
    }
    if (argc == 3) {
        ifstream infile, command;
        infile.open(argv[1]);
        open_or_die(infile, argv[1]);
        command.open(argv[2]);
        open_or_die(command, argv[2]);
        SixDegrees sixdegrees;
        sixdegrees.run(infile, command, cout);
    }
    if (argc == 4) {
        ifstream infile, command;
        ofstream output;
        infile.open(argv[1]);
        open_or_die(infile, argv[1]);
    	command.open(argv[2]);
    	open_or_die(command, argv[2]);
        output.open(argv[3]);
        open_or_die(output, argv[3]);
        SixDegrees sixdegrees;
        sixdegrees.run(infile, command, output);
    }
    return 0;
}
