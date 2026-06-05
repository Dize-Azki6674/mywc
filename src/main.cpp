#include <iostream>
#include <fstream>
#include <algorithm>
#include <type_traits>
#include <vector>
#include <string>
#include <numeric>
#include <expected>

/* mywc ********************
*    version 5.1           *
*                          *
*    made by Azkey         *
****************************/

/* ToDo 
 * + Add help.
 * + Support operand mode('--').
 * + Reject '-' option.
 */

// Specify the type of error.
enum class ErrorType {
    Unexpected = 1,
    InvalidArgs = 2,
    IOError = 3,
};

enum class EchoOption : uint8_t {
    l    = 1 << 0,
     w   = 1 << 1,
      b  = 1 << 2,
    none = 0
};
EchoOption& operator|=(EchoOption& L, EchoOption R);
EchoOption operator|(EchoOption L, EchoOption R);
std::expected<EchoOption, ErrorType>
str2eop( const std::string& sop );
constexpr bool hasFlag(EchoOption value, EchoOption flag);

class FileProp {
    private:
        std::size_t lines = 0;
        std::size_t words = 0;
        std::size_t bytes = 0;
        std::string name;
    public:
        // FileProp();
        // ~FileProp();
        static FileProp create(
            std::istream& is, std::string specName = "<stdin>"
        );
        static std::expected<FileProp, ErrorType>
            tryCreate( const std::string& fileName );
        static FileProp aggregate( const std::vector<FileProp>& fpv );
        static void echo( const FileProp& fp, EchoOption eop );
        static void echo(
            const std::vector<FileProp>& fpv,
            EchoOption eop
        );
        std::size_t getLines() const;
        std::size_t getWords() const;
        std::size_t getBytes() const;
};

class Argument {
    private:
        std::vector<std::string> operands;
        std::vector<std::string> options;
    public:
        Argument( int argc, char* argv[] );
        const std::vector<std::string>& getOperands() const;
        const std::vector<std::string>& getOptions() const;
        bool hasOption() const;
        bool containsOption(std::string opt) const;
};

void printHelp();
void printVersion();


int main( int argc, char* argv[] ) {

    Argument args(argc, argv);

    if ( args.containsOption("--help") ) {
        printHelp();
        return 0;
    }

    if ( args.containsOption("--version") ) {
        printVersion();
        return 0;
    }

    EchoOption eop = EchoOption::l | EchoOption::w | EchoOption::b;

    if ( args.hasOption() ) {
        
        eop = EchoOption::none;

        for (const std::string& sop : args.getOptions()) {

            std::expected<EchoOption, ErrorType> op = str2eop( sop );

            if ( !op ) {
                return static_cast<int>(op.error());
            }

            eop |= *op;
        }
    }

    const std::vector<std::string>& opr = args.getOperands();

    std::size_t fileCnt = args.getOperands().size();

    if (fileCnt == 0) {
        FileProp fp = FileProp::create( std::cin );
        FileProp::echo( fp, eop );
        return 0;
    }

    std::vector<FileProp> fpv;
    
    for (const std::string& fileName : opr) {
        std::expected<FileProp, ErrorType> fp = FileProp::tryCreate(fileName);

        if ( !fp ) {
            return static_cast<int>(fp.error());
        }

        fpv.push_back( *fp );
    }

    FileProp::echo( fpv, eop );

    return 0;
}


EchoOption& operator|=( EchoOption& L, EchoOption R ){
    using UT = std::underlying_type_t<EchoOption>;

    L = static_cast<EchoOption>(
        static_cast<UT>(L) | static_cast<UT>(R)
    );
    return L;
}

EchoOption operator|( EchoOption L, EchoOption R){
    L|=R;
    return L;
}

std::expected<EchoOption, ErrorType>
str2eop( const std::string& sop ) {
    EchoOption eop = EchoOption::none;

    for( char c : std::string_view{sop}.substr(1) ) {
        switch (c) {
        case 'l':
            eop |= EchoOption::l;
            break;

        case 'w':
            eop |= EchoOption::w;
            break;

        case 'b':
            eop |= EchoOption::b;
            break;

        default:
            std::cerr << "Unknown option: " << sop << '\n';
            return std::unexpected{ErrorType::InvalidArgs};
        }
    }
    return eop;
}

constexpr bool hasFlag(EchoOption value, EchoOption flag) {
    return (static_cast<uint8_t>(value)
        & static_cast<uint8_t>(flag)) != 0;
}

FileProp FileProp::create( std::istream& is, std::string specName ) {
    char ch;
    FileProp fp;
    fp.name = specName;
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
    return std::move(fp);
}

std::expected<FileProp, ErrorType>
FileProp::tryCreate( const std::string& fileName ) {
    std::ifstream ifs(fileName, std::ios_base::binary);
    if ( !ifs.is_open() ) {
        std::cerr << "Invalid file name: " << fileName << std::endl;
        return std::unexpected{ ErrorType::IOError };
    }

    return FileProp::create(ifs, fileName);
}

FileProp FileProp::aggregate( const std::vector<FileProp>& fpv ) {
    FileProp totalFP;
    totalFP.name = "total";

    for ( const FileProp& fp :fpv ) {
        totalFP.lines += fp.getLines();
        totalFP.words += fp.getWords();
        totalFP.bytes += fp.getBytes();
    }

    return totalFP;
}

void FileProp::echo( const std::vector<FileProp>& fpv, EchoOption eop ) {
    
    for ( const FileProp& fp : fpv ) {
        echo( fp, eop );
    }

    if ( fpv.size() > 1 ) {        
        echo( FileProp::aggregate(fpv), eop );
    }
}

void FileProp::echo( const FileProp& fp, EchoOption eop )
{
    if ( hasFlag(eop, EchoOption::l) ) {
        std::cout << ' ' << fp.lines;
    }
    if ( hasFlag(eop, EchoOption::w) ) {
        std::cout << ' ' << fp.words;
    }
    if ( hasFlag(eop, EchoOption::b) ) {
        std::cout << ' ' << fp.bytes;
    }
    std::cout << ' ' << fp.name << std::endl;
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

const std::vector<std::string>& Argument::getOperands() const {
    return operands;
}

const std::vector<std::string>& Argument::getOptions() const {
    return options;
}

bool Argument::hasOption() const {
    return !options.empty();
}

bool Argument::containsOption(std::string opt) const {
    return std::ranges::contains(options, opt);
}

void printHelp(){

}

void printVersion(){

}