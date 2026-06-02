#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <expected>

/* mywc ********************
*    version 3.0           *
*                          *
*    made by Azkey         *
****************************/

/* ToDo 
 * + Add help.
 * + Support options.
 */

// Specify the type of error.
enum class ErrorType {
    Unexpected = 1,
    InvalidArgs = 2,
    IOError = 3,
};

class FileProp {
    private:
        std::size_t lines;
        std::size_t words;
        std::size_t bytes;
    public:
        FileProp();
        // ~FileProp();
        static FileProp fromIst( std::istream& is );
        static std::expected<FileProp, ErrorType>
            create( const char* fileName );
        std::size_t getLines() const;
        std::size_t getWords() const;
        std::size_t getBytes() const;
        FileProp& operator+=(const FileProp& fp);
        void echo( const char* description ) const;
};


int main( int argc, char* argv[] ) {

    int fileCnt = argc - 1;

    if ( fileCnt == 0 ) {

        FileProp fp = FileProp::fromIst( std::cin );
        fp.echo("<stdin>");

    } else if ( fileCnt == 1 ) {  // single file input

        const char* fileName = argv[1];
        
        std::expected<FileProp, ErrorType> fp
            = FileProp::create(fileName);

        if ( !fp ) {
            return static_cast<int>(fp.error());
        }
        
        fp->echo( fileName );

    } else if ( fileCnt > 1 ) {    // multiple file input

        FileProp total;

        for (int i = 0; i < fileCnt; i++){
            
            char* fileName = argv[i+1];

            std::expected<FileProp, ErrorType> fp
                = FileProp::create(fileName);

            if ( !fp ) {
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

FileProp FileProp::fromIst( std::istream& is ) {
    FileProp fp;
    char ch;
    bool inWord = false;
    while ( is.get(ch) ) {
        fp.bytes++;

        if ( std::isspace(static_cast<unsigned char>(ch)) ) {
            inWord = false;
        } else if ( !inWord ) {
            fp.words++;
            inWord = true;
        }

        if ( ch == '\n' ) {
            fp.lines++;
        }
    }
    return fp;
}

std::expected<FileProp, ErrorType> FileProp::create( const char* fileName ) {
    std::ifstream ifs(fileName, std::ios_base::binary);
    if ( !ifs.is_open() ) {
        std::cerr << "Invalid file name: " << fileName << std::endl;
        return std::unexpected{ ErrorType::IOError };
    }
    FileProp fp = FileProp::fromIst(ifs);

    return fp;
}

std::size_t FileProp::getLines() const {
    return lines;
}
std::size_t FileProp::getWords() const {
    return words;
}
std::size_t FileProp::getBytes() const {
    return bytes;
}

FileProp& FileProp::operator+=(const FileProp& fp){
    lines += fp.lines;
    words += fp.words;
    bytes += fp.bytes;
    return *this;
}

void FileProp::echo( const char* description ) const {
    std::cout
    << std::format("{:>4} {:>6} {:>6} {}", lines, words, bytes, description)
    << std::endl;
}