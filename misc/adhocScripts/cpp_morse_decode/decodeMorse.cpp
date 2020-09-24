// no matter what the regex pattern is, it always matches leading and trailing whitespace in the pattern:
//       ...---... -.-.--   - .... .   --.- ..- .. -.-. -.-   -... .-. --- .-- -.   ..-. --- -..-   .--- ..- -- .--. ...   --- ...- . .-.   - .... .   .-.. .- --.. -.--   -.. --- --. .-.-.-     
// i.e. the areas in brackets are matched and should not be
//[       ]...---... -.-.--   - .... .   --.- ..- .. -.-. -.-   -... .-. --- .-- -.   ..-. --- -..-   .--- ..- -- .--. ...   --- ...- . .-.   - .... .   .-.. .- --.. -.--   -.. --- --. .-.-.- [     ]
// ...and I have no clue why

#include <regex>
using namespace std;


string decodeMorse(string morseCode) {
    string decoded;
    vector<string> splitStrings;
    cout << "Processing: {" << morseCode << "}" << endl;
    // this is a little clunky and could probably be done by fiddling with the stringstream, but i realized i haven't used regex in c++ yet and wanted to try
    // yes, i now regret this
    regex e ("(\-|\.)( {3})(\-|\.)");
    string alteredMorse;
    regex_replace (back_inserter(alteredMorse), morseCode.begin(), morseCode.end(), e, "$1 _ $3");
    
    stringstream lineStream(alteredMorse);
    
    copy(istream_iterator<string>(lineStream), istream_iterator<string>(),
    back_inserter(splitStrings));
    string value;
    cout << "Altered: {" << alteredMorse << "}" << endl;

    
    // dear lord this took me forever to figure out:
    // (before adding support for spaces it was nice and simple)
    for (auto i = splitStrings.begin(); i != splitStrings.end(); i++) {
      // something something vectors random indexed so we need the right index
      int index = distance(splitStrings.begin(), i);
      value = splitStrings.at(index);
      if (value.compare("_") == 0 
        && (i != splitStrings.end()) 
        && (i != splitStrings.begin())
        ){
        cout << "Space detected at " << index << endl;
        decoded.append(" ");
      }
      else{
        decoded.append(MORSE_CODE[value]);
      }
    }

    cout << decoded << endl;
    return decoded;
}