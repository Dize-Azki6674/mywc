#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <expected>

/* mywc ********************
*    version 2.0           *
*                          *
*    made by Azkey         *
****************************/

/* ToDo 
 * + Add help.
 * + Support stdin.
 */

// Specify the type of error.
enum class ErrorType {
    Unexpected = 1,
    InvalidArgs = 2,
    IOError = 3,
};

// using FileResult = std::expected<std::ifstream, ErrorType>;

class FileProp {
    private:
        std::ifstream ifs;
        std::size_t lines;
        std::size_t words;
        std::size_t bytes;
    public:
        FileProp();
        // ~FileProp();
        static std::expected<FileProp, ErrorType>
            create( const char* fileName );
        std::size_t getLines();
        std::size_t getWords();
        std::size_t getBytes();
        FileProp& operator+=(const FileProp& fp);
        void echo( const char* description );
};

// Count lines ( i.e. number of "\n" + 1 ) of input file.
std::size_t countLine( std::istream& is );

// Count words of input file.
std::size_t countWord( std::istream& is );

// Count bytes of input file.
std::size_t countByte( std::istream& is );



int main( int argc, char* argv[] ) {

    if ( argc == 2 ) {  // single file input

        const char* fileName = argv[1];
        
        auto fp = FileProp::create(fileName);

        if ( !fp ) {
            std::cerr << "Invalid file name." << std::endl;
            return static_cast<int>(fp.error());
        }
        
        fp->echo( fileName );

    } else if ( argc > 2 ) {    // multiple file input

        FileProp total;

        for (int i = 1; i < argc; i++){
            
            char* fileName = argv[i];

            std::expected<FileProp, ErrorType> fp
                = FileProp::create(fileName);

            if ( !fp ) {
                std::cerr << "Invalid file name." << std::endl;
                return static_cast<int>(fp.error());
            }
            
            fp->echo( fileName );

            total += *fp;
        }

        total.echo("total");

    }

    return 0;
}

FileProp::FileProp():
    lines(0),
    words(0),
    bytes(0)
{
}

std::expected<FileProp, ErrorType> FileProp::create( const char* fileName ) {
    FileProp fp;
    std::ifstream ifs(fileName, std::ios_base::binary);
    if ( !ifs.is_open() ) {
        return std::unexpected{ ErrorType::IOError };
    }
    fp.ifs = std::move(ifs);
    fp.lines = countLine(fp.ifs);
    fp.words = countWord(fp.ifs);
    fp.bytes = countByte(fp.ifs);

    return fp;
}

std::size_t FileProp::getLines() {
    return lines;
}
std::size_t FileProp::getWords() {
    return words;
}
std::size_t FileProp::getBytes() {
    return bytes;
}

FileProp& FileProp::operator+=(const FileProp& fp){
    lines += fp.lines;
    words += fp.words;
    bytes += fp.bytes;
    return *this;
}

void FileProp::echo( const char* description ) {
    std::cout
    << std::format("{:>4} {:>6} {:>6} {}", lines, words, bytes, description)
    << std::endl;
}

// Count lines ( i.e. number of "\n" + 1 ) of input file.
std::size_t countLine( std::istream& is ) {
    std::size_t lineCounter = 0;
    std::string line;
    while ( std::getline(is, line) ) {
        ++lineCounter;
    }
    is.clear();
    is.seekg(0);
    return lineCounter;
};

// Count words of input file.
std::size_t countWord( std::istream& is ) {
    is.clear();
    is.seekg(0);

    std::size_t wordCounter = 0;
    std::string word;

    while ( is >> word ) {
        ++wordCounter;
    }
    
    is.clear();
    is.seekg(0);

    return wordCounter;
};

// Count bytes of input file.
std::size_t countByte( std::istream& is ) {
    is.clear();
    std::streamsize size = is.seekg(0, std::ios::end).tellg();
    
    is.clear();
    is.seekg(0);

    return static_cast<std::size_t>(size);
};