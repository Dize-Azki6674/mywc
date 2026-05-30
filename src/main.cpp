#include <iostream>
#include <fstream>
#include <string>
#include <expected>

/* mywc ********************
*    version 1.0           *
*                          *
*    made by Azkey         *
****************************/

/* ToDo 
 * + Support multiple file inputs.
 * + Add help.
 * + Support stdin.
 */

// Specify the type of error.
enum class ErrorType {
    Unexpected = 1,
    InvalidArgs = 2,
    IOError = 3,
};

using FileResult = std::expected<std::ifstream, ErrorType>;

// Try to open a file with its name.
// Throw error when the file cannot open.
FileResult openFile( const char* fileName );

// Count lines ( i.e. number of "\n" + 1 ) of input file.
int countLine( std::istream& is );

// Count words of input file.
int countWord( std::istream& is );

// Count bytes of input file.
int countByte( std::istream& is );



int main( int argc, char* argv[] ) {

    // single file input
    if ( argc == 2 ) {

        const char* fileName = argv[1];
        FileResult file = openFile(fileName);
        if ( !file ) {
            std::cerr << "Invalid file name." << std::endl;
            return static_cast<int>(file.error());
        }

        int lineCnt = countLine(*file);
        int wordCnt = countWord(*file);
        int byteCnt = countByte(*file);

        std::cout << lineCnt << " " << wordCnt << " " << byteCnt << " " << fileName << std::endl;
    }

    return 0;
}


// Try to open a file with its name.
// Throw error when the file cannot open.
FileResult openFile( const char* fileName ) {
    std::ifstream ifs(fileName, std::ios_base::binary);

    if (!ifs.is_open()) {
        return std::unexpected{ ErrorType::IOError };
    }

    return ifs;
};

// Count lines ( i.e. number of "\n" + 1 ) of input file.
int countLine( std::istream& is ) {
    int lineCounter = 0;
    std::string line;
    while ( std::getline(is, line) ) {
        ++lineCounter;
    }
    is.clear();
    is.seekg(0);
    return lineCounter;
};

// Count words of input file.
int countWord( std::istream& is ) {
    is.clear();
    is.seekg(0);

    int wordCounter = 0;
    std::string word;

    while ( is >> word ) {
        ++wordCounter;
    }
    
    is.clear();
    is.seekg(0);

    return wordCounter;
};

// Count bytes of input file.
int countByte( std::istream& is ) {
    is.clear();
    std::streamsize size = is.seekg(0, std::ios::end).tellg();
    
    is.clear();
    is.seekg(0);

    return static_cast<int>(size);
};