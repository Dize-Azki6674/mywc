#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <expected>

/* mywc ********************
*    version 4.0           *
*                          *
*    made by Azkey         *
****************************/

/* ToDo 
 * + Add help.
 */

// Specify the type of error.
enum class ErrorType {
    Unexpected = 1,
    InvalidArgs = 2,
    IOError = 3,
};

enum class EchoOption : uint8_t {
    l    = 0b001,
     w   = 0b010,
    lw   = 0b011,
      b  = 0b100,
    lb   = 0b101,
     wb  = 0b110,
    lwb  = 0b111,
    none = 0b000
};
EchoOption& operator|=(EchoOption& L, EchoOption R);
EchoOption operator&(EchoOption L, EchoOption R);
EchoOption str2eop(std::string sop);

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
            create( const std::string fileName );
        std::size_t getLines() const;
        std::size_t getWords() const;
        std::size_t getBytes() const;
        FileProp& operator+=(const FileProp& fp);
        void echo( const std::string description, EchoOption eop ) const;
};

class Argument {
    private:
        std::vector<std::string> operands;
        std::vector<std::string> options;
    public:
        Argument( int argc, char* argv[] );
        std::vector<std::string> getOpr();
        std::vector<std::string> getOpt();
};


int main( int argc, char* argv[] ) {

    Argument args(argc, argv);

    int fileCnt = args.getOpr().size();

    EchoOption eop = EchoOption::lwb;

    if (args.getOpt().size() != 0) {
        eop = EchoOption::none;
        for (const std::string& sop : args.getOpt()) {
            eop |= str2eop(sop);
        }
    }
 
    switch (fileCnt) {
        case 0: {
            FileProp fp = FileProp::fromIst( std::cin );
            fp.echo("<stdin>", eop);

            break;
        }
        case 1: {
            const std::string fileName = args.getOpr()[0];
            
            std::expected<FileProp, ErrorType> fp
                = FileProp::create(fileName);

            if ( !fp ) {
                return static_cast<int>(fp.error());
            }
            
            fp->echo( fileName, eop );

            break;
        }
        default: {
            FileProp total;

            for (int i = 0; i < fileCnt; i++){
                
                std::string fileName = args.getOpr()[i];

                std::expected<FileProp, ErrorType> fp
                    = FileProp::create(fileName);

                if ( !fp ) {
                    return static_cast<int>(fp.error());
                }
                
                fp->echo( fileName, eop );

                total += *fp;
            }

            total.echo("total", eop);

            break;
        }
    }

    return 0;
}


EchoOption& operator|=( EchoOption& L, EchoOption R ){
    L = static_cast<EchoOption>(
        static_cast<uint8_t>(L) | static_cast<uint8_t>(R)
    );
    return L;
}

EchoOption operator&( EchoOption L, EchoOption R ){
    return static_cast<EchoOption>(
        static_cast<uint8_t>(L) & static_cast<uint8_t>(R)
    );
}

EchoOption str2eop( std::string sop ) {
    std::map<std::string, EchoOption> m;
    
    m["-l"  ] = EchoOption::l  ;
    m["-w"  ] = EchoOption::w  ;
    m["-b"  ] = EchoOption::b  ;
    m["-lw" ] = EchoOption::lw ;
    m["-lb" ] = EchoOption::lb ;
    m["-wb" ] = EchoOption::wb ;
    m["-lwb"] = EchoOption::lwb;

    return m.contains(sop) ? m[sop] : EchoOption::none;
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

std::expected<FileProp, ErrorType>
FileProp::create( const std::string fileName ) {
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

void FileProp::echo( const std::string description, EchoOption eop ) const {
    if ( static_cast<bool>(eop & EchoOption::l) ) {
        std::cout << std::setw(4) << lines;
    }
    if ( static_cast<bool>(eop & EchoOption::w) ) {
        std::cout << std::setw(6) << words;
    }
    if ( static_cast<bool>(eop & EchoOption::b) ) {
        std::cout << std::setw(6) << bytes;
    }
    std::cout << ' ' << description <<std::endl;
}

Argument::Argument( int argc, char* argv[] ) {
    for ( int i = 1; i < argc; i++ ) {
        std::string arg = argv[i];

        if (arg.starts_with('-')) {
            options.push_back(std::move(arg));
            continue;
        }

        operands.push_back(std::move(arg));
    }
}

std::vector<std::string> Argument::getOpr() {
    return operands;
}

std::vector<std::string> Argument::getOpt() {
    return options;
}