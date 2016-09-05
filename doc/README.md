JPCRE2                         {#mainpage}
======

C++ wrapper of PCRE2 library

[![Build status image](https://travis-ci.org/jpcre2/jpcre2.svg?branch=release)](https://travis-ci.org/jpcre2/jpcre2/)


PCRE2 is the name used for a revised API for the PCRE library, which is a set of functions, written in C, that implement regular expression pattern matching using the same syntax and semantics as Perl, with just a few differences. Some features that appeared in Python and the original PCRE before they appeared in Perl are also available using the Python syntax.

This provides some C++ wrapper functions to provide some useful utilities like regex match and regex replace.


# Dependency {#dependency}

1. PCRE2 library (`version >=10.21`).


If the required PCRE2 version is not available in the official channel, download <a href="https://github.com/jpcre2/pcre2">my fork of the library</a>.


# Install or Include {#install-or-include}

The `jpcre2.hpp` header should be included in the source file that uses JPCRE2 functionalities.

## Use with sources {#use-with-sources}

After including the header you can compile your source either by installing and linking with JPCRE2 library or providing the following sources to your compiler:

1. **jpcre2.hpp**
2. **jpcre2.cpp**

An example compile/build command with GCC would be:

```sh
g++ mycpp.cpp jpcre2.cpp jpcre2.hpp -lpcre2-8
```

If your PCRE2 library is not in the standard library path, then add the path:

```sh
g++ mycpp.cpp ... -L/path/to/your/pcre2/library -lpcre2-8
```

**Note that** it requires the PCRE2 library installed in your system. If it is not already installed and linked in your compiler, you will need to link it with appropriate path and options.

## Use as a library {#install-as-a-library}

To install it as a library in a Unix based system, run:

```sh
./configure
make
make install # or sudo make install
```
Now `#include <jpcre2.hpp>` in your code and build/compile by linking with both JPCRE2 and PCRE2 library.

An example command for GCC would be:

```sh
g++  mycpp.cpp -ljpcre2-8 -lpcre2-8 #sequence is important
```

If you are in a non-Unix system (e.g Windows), build a library from the JPCRE2 sources with your favorite IDE or use it as it is.

**Notes:**

1. `PCRE2_CODE_UNIT_WIDTH` other than 8 is not supported in this version.
2. To use the `PCRE2 POSIX` compatible library, add the `-lpcre2-posix` along with the others.


# How to code with JPCRE2 {#how-to-code-with-jpcre2}

Performing a match or replacement against regex pattern involves two steps: 

1. Compiling the pattern
2. Performing the match or replacement operation

## Compile a pattern {#compile-a-pattern}

**First create a `jpcre2::Regex` object**

(You can use temporary object too, see [short examples](#short-examples)).

This object will hold the pattern, modifiers, compiled pattern, error and warning codes.

```cpp
jpcre2::Regex re;   //Create object, it's not supposed to throw exception
```
Each object for each regex pattern.

**Compile the pattern** and catch any error exception:

```cpp
try{re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")  //set pattern
      .setModifier("nJ")                                                    //set modifier
      .addJpcre2Option(jpcre2::VALIDATE_MODIFIER                            //modifier goes through validation check
                        | jpcre2::JIT_COMPILE                               //perform JIT compile
                        | jpcre2::ERROR_ALL)                                //treat warnings as errors
      .addPcre2Option(0)                                                    //add pcre2 option
      .compile();                                                           //Finally compile it.
    
    //Another way is to use constructor to initialize and compile at the same time:
    jpcre2::Regex re2("pattern2","mSi");  //S is an optimization mod.
    jpcre2::Regex re3("pattern3", PCRE2_ANCHORED);
    jpcre2::Regex re4("pattern4", PCRE2_ANCHORED, jpcre2::JIT_COMPILE);
}
catch(int e){
    /*Handle error*/
    std::cerr<<re.getErrorMessage(e)<<std::endl;
}
```

Now you can perform match or replace against the pattern. Use the `match()` member function to preform regex match and the `replace()` member function to perform regex replace.


## Match {#match}

The `jpcre2::Regex::match(const String& s)` member function can take two arguments (subject & modifier) and returns the number of matches found against the compiled pattern.


To get the match result (captured groups) however, you need to call the `jpcre2::RegexMatch::match()` function. Point be noted that, you can not call this function directly or create any object of the class `jpcre2::RegexMatch`. To call this function, first invoke the `jpcre2::Regex::initMatch()` function. It will give you a temporary `jpcre2::RegexMatch` object. Now you can chain function calls of `jpcre2::RegexMatch::setNumberedSubstringVector(VecNum* vec_num)` and such functions from `jpcre2::RegexMatch` class to pass various parameters. After you are done passing all the parameter that you need, the `jpcre2::RegexMatch::match()` function should be called to perform the actual match and return the match count. The match results will be stored in vectors (vectors of maps) whose pointers were passed as parameters.

*You should catch any error exception that may be thrown in case error occurs.*

### Get match count {#simple-match-count}

```cpp
//If you want to match all and get the match count, use the action modifier 'g':
size_t count = jpcre2::Regex("(\\d)|(\\w)","m").match("I am the subject","g");
```

### Get match result {#do-match}

To get the match results, you need to pass appropriate vector pointers. This is an example of how you can get the numbered substrings/captured groups from a match:


```cpp
jpcre2::VecNum vec_num;
try{
    size_t count=re.initMatch()									//prepare for match() call
    			   .setSubject(subject)                         //set subject string
                   .setModifier(ac_mod)                         //set modifier string
                   .setNumberedSubstringVector(&vec_num)        //pass VecNum vector to store maps of numbered substrings
                   .addJpcre2Option(jpcre2::VALIDATE_MODIFIER)  //add jpcre2 option
                   .match();                                    //Finally perform the match.
    //vec_num will be populated with maps of numbered substrings.
    //count is the total number of matches found
}
catch(int e){
    /*Handle error*/
    std::cerr<<re.getErrorMessage(e)<<std::endl;
}
```
### Access a substring {#access-substring}

You can access a substring/captured group by specifying their index (position):

```cpp
std::cout<<vec_num[0][0]; // group 0 in first match
std::cout<<vec_num[0][1]; // group 1 in first match
std::cout<<vec_num[1][0]; // group 0 in second match
```
### Get named substrings {#get-named-substrings}

To get named substring and/or name to number mapping, pass pointer to the appropriate vectors with `jpcre2::RegexMatch::setNamedSubstringVector()` and/or `jpcre2::RegexMatch::setNameToNumberMapVector()` before doing the match.

```cpp
jpcre2::VecNum vec_num;   ///Vector to store numbured substring Map.
jpcre2::VecNas vec_nas;   ///Vector to store named substring Map.
jpcre2::VecNtN vec_ntn;   ///Vector to store Named substring to Number Map.
std::string ac_mod="g";   // g is for global match. Equivalent to using setFindAll() or FIND_ALL in addJpcre2Options()
try{
    re.initMatch()
      .setSubject(subject)                         //set subject string
      .setModifier(ac_mod)                         //set modifier string
      .setNumberedSubstringVector(&vec_num)        //pass pointer to vector of numbered substring maps
      .setNamedSubstringVector(&vec_nas)           //pass pointer to vector of named substring maps
      .setNameToNumberMapVector(&vec_ntn)          //pass pointer to vector of name to number maps
      .addJpcre2Option(jpcre2::VALIDATE_MODIFIER)  //add jpcre2 option
      .addPcre2Option(PCRE2_ANCHORED)              //add pcre2 option
      .match();                                    //Finally perform the match()
}
catch(int e){
    /*Handle error*/
    std::cerr<<re.getErrorMessage(e)<<std::endl;
}
```

### Accesing a substring by name {#access-substring-by-name}

```cpp
std::cout<<vec_nas[0]["name"]; // captured group by name in first match
std::cout<<vec_nas[1]["name"]; // captured group by name in second match
```

### Get the position of a capture group name {#get-number-to-name}

If you need this information, you should have passed a `jpcre2::VecNtN` pointer to `jpcre2::RegexMatch::setNameToNumberMapVector()` function before doing the match ([see above](#get-named-substrings)).

```cpp
std::cout<<vec_ntn[0]["name"]; // position of captured group 'name' in first match
```

### Iterate through match result {#iterate}

You can iterate through the matches and their substrings like this:

```cpp
for(size_t i=0;i<vec_num.size();++i){
    //i=0 is the first match found, i=1 is the second and so forth
    for(jpcre2::MapNum::iterator ent=vec_num[i].begin();ent!=vec_num[i].end();++ent){
	    //ent.first is the number/position of substring found
	    //ent.second is the substring itself
	    //when ent->first is 0, ent->second is the total match.
        std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
    }
}
```

If you are using `>=C++11`, you can make the loop a lot simpler:

<!-- if version [gte C++11] -->
```cpp
for(size_t i=0;i<vec_num.size();++i){
	for(auto const& ent : vec_num[i]){
	    std::cout<<"\n\t"<<ent.first<<": "<<ent.second<<"\n";
	}
}
```
<!-- end version if -->

*The process of iterating through the vectors and associated maps are the same for all three. The size of those vectors are the same and can be accessed in the same way.*

##Replace or Substitute {#replace}

The `jpcre2::Regex::replace(const String& s, const String& r)` member function can take up-to three arguments (subject, replacement string, modifier) and returns the resultant replaced string.

If you want to pass more options or prefer a named parameter idiom, you will have to use the `jpcre2::RegexReplace::replace()` function instead. Point be noted that, all constructors of the `jpcre2::RegexReplace` class are private and thus you can't create any object of this class or call the mentioned function directly. In this case you need to call `jpcre2::Regex::initReplace()` function which will give you a temporary object that you can use to chain method calls to pass various options to be used by `jpcre2::RegexReplace::replace()` before calling it.

*You should catch any error exception that may be thrown in case error occurs.*

### Simple replacement {#simple-replace}


```cpp
//Using a temporary regex object
std::cout<<jpcre2::Regex("\\d+").replace("I am digits 1234","5678", "g");
//'g' modifier is for global replacement
```

### Using named parameter idiom {#replace-with-named-parameter}

```cpp
try{
    std::cout<<
    re.initReplace()       //Prepare to call jpcre2::RegexReplace::replace()
      .setSubject(s)       //Set various parameters
      .setReplaceWith(s2)  //...
      .setModifier("gE")   //...
      .addJpcre2Option(0)  //...
      .addPcre2Option(0)   //...
      .replace();          //Finally do the replacement.
    //gE is the modifier passed (global and unknown-unset-empty).
    //Access substrings/captured groups with ${1234},$1234 (for numbered substrings)
    // or ${name} (for named substrings) in the replacement part i.e in setReplaceWith()
}
catch(int e){
    /*Handle error*/
    std::cerr<<re.getErrorMessage(e)<<std::endl;
}
```
If you pass the size of the resultant string with `jpcre2::RegexReplace::setBufferSize()` function, make sure it will be enough to store the whole resultant replaced string; otherwise the internal replace function (`pcre2_substitute()`) will be called *twice* to adjust the size of the buffer to hold the whole resultant string in order to avoid `PCRE2_ERROR_NOMEMORY` error.


# Modifiers {#modifiers}

**JPCRE2** uses modifiers to control various options, type, behavior of the regex and its' interactions with different functions that uses it. Two types of modifiers are available: *compile modifiers* and *action modifiers*:


## Compile modifiers {#compile-modifier}

These modifiers define the behavior of a regex pattern. They have more or less the same meaning as the [PHP regex modifiers](https://php.net/manual/en/reference.pcre.pattern.modifiers.php) except for `e, j and n` (marked with <sup>\*</sup>). 

Modifier | Details
-------- | -------
`e`<sup>\*</sup> | Unset back-references in the pattern will match to empty strings. Equivalent to `PCRE2_MATCH_UNSET_BACKREF`.
`i` | Case-insensitive. Equivalent to `PCRE2_CASELESS` option.
`j`<sup>\*</sup> | `\u \U \x` and unset back-references will act as JavaScript standard. <ul><li><code>\U</code> matches an upper case "U" character (by default it causes a compile time error if this option is not set).</li><li><code>\u</code> matches a lower case "u" character unless it is followed by four hexadecimal digits, in which case the hexadecimal number defines the code point to match (by default it causes a compile time error if this option is not set).</li><li><code>\x</code> matches a lower case "x" character unless it is followed by two hexadecimal digits, in which case the hexadecimal number defines the code point to match (By default, as in Perl, a hexadecimal number is always expected after <code>\x</code>, but it may have zero, one, or two digits (so, for example, <code>\xz</code> matches a binary zero character followed by z) ).</li><li>Unset back-references in the pattern will match to empty strings.</li></ul>
`m` | Multi-line regex. Equivalent to `PCRE2_MULTILINE` option.
`n`<sup>\*</sup> | Enable Unicode support for `\w \d` etc... in pattern. Equivalent to PCRE2_UTF \| PCRE2_UCP.
`s` | If this modifier is set, a dot meta-character in the pattern matches all characters, including newlines. Equivalent to `PCRE2_DOTALL` option.
`u` | Enable UTF support.Treat pattern and subjects as UTF strings. It is equivalent to `PCRE2_UTF` option.
`x` | Whitespace data characters in the pattern are totally ignored except when escaped or inside a character class, enables commentary in pattern. Equivalent to `PCRE2_EXTENDED` option.
`A` | Match only at the first position. It is equivalent to `PCRE2_ANCHORED` option.
`D` | A dollar meta-character in the pattern matches only at the end of the subject string. Without this modifier, a dollar also matches immediately before the final character if it is a newline (but not before any other newlines). This modifier is ignored if `m` modifier is set. Equivalent to `PCRE2_DOLLAR_ENDONLY` option.
`J` | Allow duplicate names for sub-patterns. Equivalent to `PCRE2_DUPNAMES` option.
`S` | When a pattern is going to be used several times, it is worth spending more time analyzing it in order to speed up the time taken for matching/replacing. It may also be beneficial for a very long subject string or pattern. Equivalent to an extra compilation with JIT\_COMPILER with the option `PCRE2_JIT_COMPLETE`.
`U` | This modifier inverts the "greediness" of the quantifiers so that they are not greedy by default, but become greedy if followed by `?`. Equivalent to `PCRE2_UNGREEDY` option.

## Action modifiers {#action-modifiers}

These modifiers are not compiled in the regex itself, rather they are used per call of each match or replace function.

Modifier | Details
------ | ------
`A` | Match at start. Equivalent to `PCRE2_ANCHORED`. Can be used in match operation. Setting this option only at match time (i.e regex was not compiled with this option) will disable optimization during match time.
`e` | Replaces unset group with empty string. Equivalent to `PCRE2_SUBSTITUTE_UNSET_EMPTY`. Can be used in replace operation.
`E` | Extension of `e` modifier. Sets even unknown groups to empty string. Equivalent to PCRE2_SUBSTITUTE_UNSET_EMPTY \| PCRE2_SUBSTITUTE_UNKNOWN_UNSET.
`g` | Global. Will perform global matching or replacement if passed.
`x` | Extended replacement operation. It enables some Bash like features:<br>`${<n>:-<string>}`<br>`${<n>:+<string1>:<string2>}`<br>`<n>` may be a group number or a name. The first form specifies a default value. If group `<n>` is set, its value is inserted; if not, `<string>` is expanded and the result is inserted. The second form specifies strings that are expanded and inserted when group `<n>` is set or unset, respectively. The first form is just a convenient shorthand for `${<n>:+${<n>}:<string>}`.

<div id="jpcre2-options"></div>

# Options {#options}

JPCRE2 allows both PCRE2 and native JPCRE2 options to be passed. PCRE2 options are recognized by the PCPRE2 library itself.

## JPCRE2 options {#jpcre-options}

These options are meaningful only for the **JPCRE2** library itself not the original **PCRE2** library. We use the `addJpcre2Options()` function to pass these options.

Option | Details
------ | ------
`jpcre2::NONE` | This is the default option. Equivalent to 0 (zero).
`jpcre2::VALIDATE_MODIFIER` | If this option is passed, modifiers will be subject to validation check. If any of them is invalid, a `jpcre2::ERROR::INVALID_MODIFIER` error exception will be thrown. You can get the error message with `jpcre2::Regex::getErrorMessage(error_code)` member function.
`jpcre2::FIND_ALL` | This option will do a global matching if passed during matching. The same can be achieved by passing the 'g' modifier with `jpcre2::RegexMatch::setModifier()` function.
`jpcre2::ERROR_ALL` | Treat warnings as errors and throw exception.
`jpcre2::JIT_COMPILE` | This is same as passing the `S` modifier during pattern compilation.

## PCRE2 options {#pcre2-options}

While having its own way of doing things, JPCRE2 also supports the traditional PCRE2 options to be passed. We use the `addPcre2Option()` functions to pass the PCRE2 options. These options are the same as the PCRE2 library and have the same meaning. For example instead of passing the 'g' modifier to the replacement operation we can also pass its PCRE2 equivalent `PCRE2_SUBSTITUTE_GLOBAL` to have the same effect.

# Short examples {#short-examples}

```cpp
size_t count;
///Check if string matches the pattern
/**
 * The following uses a temporary Regex object.
 * */
if(jpcre2::Regex("(\\d)|(\\w)").match("I am the subject")) 
    std::cout<<"\nmatched";
else
    std::cout<<"\nno match";
/**
 * The above is a good example of using temporary objects to perform match (or replace)
 * 
 * Using the modifier S (i.e jpcre2::JIT_COMPILE) with temporary object may or may not give you
 * any performance boost (depends on the complexity of the pattern). The more complex 
 * the pattern gets, the more sense the S modifier makes.
 * */
 
///If you want to match all and get the match count, use the action modifier 'g':
std::cout<<"\n"<<
    jpcre2::Regex("(\\d)|(\\w)","m").match("I am the subject","g");

/**
 * Modifiers passed to the Regex constructor or with compile() function are compile modifiers
 * Modifiers passed with the match() or replace() functions are action modifiers
 * */

/// Substrings/Captured groups:

/**
 * *** Getting captured groups/substring ***
 * 
 * captured groups or substrings are stored in maps for each match,
 * and each match is stored in a vector. 
 * Thus captured groups are in a vector of maps.
 * 
 * PCRE2 provides two types of substrings:
 *  1. numbered (index) substring
 *  2. named substring
 * 
 * For the above two, we have two vectors respectively:
 *  1. jpcre2::VecNum (Corresponding map: jpcre2::MapNum)
 *  2. jpcre2::VecNas (Corresponding map: jpcre2::MapNas)
 * 
 * Another additional vector is available to get the substring position/number
 * for a particular captured group by name. It's a vector of name to number maps
 *  * jpcre2::VecNtN (Corresponding map: jpcre2:MapNtN)
 * */

/// ***** Get numbered substring ***** ///
jpcre2::VecNum vec_num;
count = 
jpcre2::Regex("(\\w+)\\s*(\\d+)","m")
        .initMatch()
        .setSubject("I am 23, I am digits 10")
        .setModifier("g")
        .setNumberedSubstringVector(&vec_num)
        .match();
/**
* count (the return value) is guaranteed to give you the correct number of matches,
* while vec_num.size() may give you wrong result if any match result
* was failed to be inserted in the vector. This should not happen
* i.e count and vec_num.size() should always be equal.
* */
std::cout<<"\nNumber of matches: "<<count/* or vec_num.size()*/;

///Now vec_num is populated with numbered substrings for each match
///The size of vec_num is the total match count
///vec_num[0] is the first match
///The type of vec_num[0] is jpcre2::MapNum
std::cout<<"\nTotal match of first match: "<<vec_num[0][0];      ///Total match (group 0) from first match
std::cout<<"\nCaptrued group 1 of frist match: "<<vec_num[0][1]; ///captured group 1 from first match 
std::cout<<"\nCaptrued group 2 of frist match: "<<vec_num[0][2]; ///captured group 2 from first match
std::cout<<"\nCaptrued group 3 of frist match: "<<vec_num[0][3]; ///captured group 3 doesn't exist, it will give you empty string
///Using the [] operator with jpcre2::MapNum will create new element if it doesn't exist
/// i.e vec_num[0][3] were created in the above example.
///This should be ok, if existence of a particular substring is not important

///If the existence of a substring is important, use the std::map::find() or std::map::at() (>=C++11) function to access map elements
/* //>=C++11
try{
    ///This will throw exception, because substring 4 doesn't exist
    std::cout<<"\nCaptrued group 4 of frist match: "<<vec_num[0].at(4);
} catch (std::logic_error e){
    std::cout<<"\nCaptrued group 4 doesn't exist";
}*/

///There were two matches found (vec_num.size() == 2) in the above example
std::cout<<"\nTotal match of second match: "<<vec_num[1][0];      ///Total match (group 0) from second match
std::cout<<"\nCaptrued group 1 of second match: "<<vec_num[1][1]; ///captured group 1 from second match 
std::cout<<"\nCaptrued group 2 of second match: "<<vec_num[1][2]; ///captured group 2 from second match


/// ***** Get named substring ***** ///

jpcre2::VecNas vec_nas;
jpcre2::VecNtN vec_ntn; /// We will get name to number map vector too
count = 
jpcre2::Regex("(?<word>\\w+)\\s*(?<digit>\\d+)","m")
        .initMatch()
        .setSubject("I am 23, I am digits 10")
        .setModifier("g")
        ///.setNumberedSubstringVector(vec_num) /// We don't need it in this example
        .setNamedSubstringVector(&vec_nas)
        .setNameToNumberMapVector(&vec_ntn) /// Additional (name to number maps)
        .match();
std::cout<<"\nNumber of matches: "<<vec_nas.size()/* or count */;
///Now vec_nas is populated with named substrings for each match
///The size of vec_nas is the total match count
///vec_nas[0] is the first match
///The type of vec_nas[0] is jpcre2::MapNas
std::cout<<"\nCaptured group (word) of first match: "<<vec_nas[0]["word"];
std::cout<<"\nCaptured group (digit) of first match: "<<vec_nas[0]["digit"];

///If the existence of a substring is important, use the std::map::find() or std::map::at() (>=C++11) function to access map elements
/* //>=C++11
try{
    ///This will throw exception becasue the substring name 'name' doesn't exist
    std::cout<<"\nCaptured group (name) of first match: "<<vec_nas[0].at("name");
} catch(std::logic_error e){
    std::cout<<"\nCaptured group (name) doesn't exist";
}*/

///There were two matches found (vec_nas.size() == 2) in the above example
std::cout<<"\nCaptured group (word) of second match: "<<vec_nas[1]["word"];
std::cout<<"\nCaptured group (digit) of second match: "<<vec_nas[1]["digit"];

///Get the position (number) of a captured group name (that was found in match)
std::cout<<"\nPosition of captured group (word) in first match: "<<vec_ntn[0]["word"];
std::cout<<"\nPosition of captured group (digit) in first match: "<<vec_ntn[0]["digit"];

/**
 * Replacement Examples
 * Replace pattern in a string with a replacement string
 * 
 * The initReplace() function can take a subject and replacement string as argument.
 * You can also pass the subject with setSubject() function in method chain,
 * replacement string with setReplaceWith() function in method chain, etc ...
 * 
 * A call to replace() will return the resultant string
 * */

std::cout<<"\n"<<
///replace first occurrence of a digit with @
jpcre2::Regex("\\d").replace("I am the subject string 44", "@");

std::cout<<"\n"<<
///replace all occrrences of a digit with @
jpcre2::Regex("\\d").replace("I am the subject string 44", "@", "g");

///swap two parts of a string
std::cout<<"\n"<<
jpcre2::Regex("^([^\t]+)\t([^\t]+)$")
        .replace("I am the subject\tTo be swapped according to tab", "$2 $1");
        
```


