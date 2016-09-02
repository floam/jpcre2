[![Build status image](https://travis-ci.org/jpcre2/jpcre2.svg?branch=release)](https://travis-ci.org/jpcre2/jpcre2/)

<div id="description"></div>
PCRE2 is the name used for a revised API for the PCRE library, which is a set of functions, written in C, that implement regular expression pattern matching using the same syntax and semantics as Perl, with just a few differences. Some features that appeared in Python and the original PCRE before they appeared in Perl are also available using the Python syntax.

This provides some C++ wrapper functions to provide some useful utilities like regex match and regex replace.

<div id="requires"></div>
#Requirements:

1. PCRE2 library (`version >=10.21`).
2. C++ compiler with C++11 support.

If the required PCRE2 version is not available in the official channel, download <a href="https://github.com/jpcre2/pcre2">my fork of the library</a> which will always be kept compatible with JPCRE2.

<div id="how-to"></div>
#Install/Include:

It can be installed as a separate library or can be used directly in a project by including the appropriate sources:

1. **jpcre2.h**
2. **jpcre2.cpp**
3. **jpcre2_match.cpp**
4. **jpcre2_replace.cpp**

An example compile/build command with GCC would be:

```sh
g++ mycpp.cpp jpcre2_match.cpp jpcre2_replace.cpp jpcre2.cpp jpcre2.h -lpcre2-8
```

If your PCRE2 library is not in the standard library path, then add the path:

```sh
g++ -std=c++11 mycpp.cpp ... -L/path/to/your/pcre2/library -lpcre2-8
```

**Note that** it requires the PCRE2 library installed in your system. If it is not already installed and linked in your compiler, you will need to link it with appropriate path and options.

**Installing JPCRE2 as a library:**

To install it in a Unix based system, run:
```sh
./configure
make
sudo make install
```
Now to use it:

1. `#include <jpcre2.h>` in your code.
2. Build/compile by linking with JPCRE2 and PCRE2 library.

An example command for GCC would be:

```sh
g++  mycpp.cpp -ljpcre2-8 -lpcre2-8 #sequence is important
```

If you are in a non-Unix system (e.g Windows), build a library from the JPCRE2 sources with your favourite IDE or use it as it is.

**Note:**

<ol>
<li>PCRE2_CODE_UNIT_WIDTH other than 8 is not supported in this version.
</li>
<li>To use the PCRE2 POSIX compatible library, add the <code>-lpcre2-posix</code> along with the others.
</ol>

#How to code with JPCRE2:

<ol>
<li>
First create a <code>jpcre2::Regex</code> object (You can use temporary object too, see <a href="#short-examples">short examples</a>). This object will hold the pattern, modifiers, compiled pattern, error and warning codes.
<pre class="highlight"><code class="highlight-source-c++ cpp">jpcre2::Regex re;   //Create object</code></pre>
Each object for each regex pattern.
</li>
<li>
Compile the pattern and catch any error exception:
<pre class="highlight"><code class="highlight-source-c++ cpp">
try{
    re.compile()            //Invoke the compile() function
      .pattern(pat)         //set various parameters
      .modifiers("Jin")     //Always sets the modifier
      .jpcre2Options(0)     //Adds the option
      .pcre2Options(0)      //Adds the option
      .execute();           //Finaly execute it.
    
    //Another way is to use constructor to initialize and compile at the same time:
    jpcre2::Regex re2("pattern2","mSi");  //S is an optimization mod.
    jpcre2::Regex re3("pattern3", PCRE2_ANCHORED);
    jpcre2::Regex re4("pattern4", PCRE2_ANCHORED, jpcre2::JIT_COMPILE);
    // Using constructor to compile the regex is faster than invoking compile() and execute() explicitly
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}
</code></pre>
</li>
<li>
Now you can perform match or replace against the pattern. Use the <code>match()</code> member function to preform regex match and the <code>replace()</code> member function to perform regex replace.
</li>
  <ol>
<li>
<b>Match:</b> The <code>match()</code> member function can take an optional argument (subject) and returns an object of the class <i>RegexMatch</i> which then in turn can be used to pass various parameters using  available member functions (method chaining) of <i>RegexMatch</i> class. The end function in the method chain should always be the <code>execute()</code> function which returns the result (number of matches found).
</li>
    <ul>
<li>
Perform match and catch any error exception:
<pre class="highlight"><code class="highlight-source-c++ cpp">
jpcre2::VecNum vec_num;
try{
    size_t count=re.match(subject)                            //Invoke the match() function
                   .modifiers(ac_mod)                         //Set various options
                   .numberedSubstringVector(vec_num)          //...
                   .jpcre2Options(jpcre2::VALIDATE_MODIFIER)  //...
                   .execute();                                //Finally execute it.
    //vec_num will be populated with maps of numbered substrings.
    //count is the total number of matches found
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}
</code></pre>
Access the substrings like this:
<pre class="highlight"><code class="highlight-source-c++ cpp">
for(size_t i=0;i&lt;vec_num.size();++i){
    //This loop will iterate only once if find_all is false.
    //i=0 is the first match found, i=1 is the second and so forth
    for(auto const&amp; ent : vec_num[i]){
        //ent.first is the number/position of substring found
        //ent.second is the substring itself
        //when ent.first is 0, ent.second is the total match.
    }
}
</code></pre>
</li>
<li>
To get named substrings or name to number mapping, simply pass the appropriate vectors with <code>numberedSubstringVector()</code> and/or <code>namedSubstringVector()</code> and/or <code>nameToNumberMapVector()</code>:
<pre class="highlight"><code class="highlight-source-c++ cpp">
jpcre2::VecNum vec_num;   ///Vector to store numbured substring Map.
jpcre2::VecNas vec_nas;   ///Vector to store named substring Map.
jpcre2::VecNtN vec_ntn;    ///Vector to store Named substring to Number Map.
std::string ac_mod="g"; // g is for global match. Equivalent to using findAll() or FIND_ALL in jpcre2Options()
try{
    re.match(subject)                            //Invoke the match() function
      .modifiers(ac_mod)                         //Set various options
      .numberedSubstringVector(vec_num)          //...
      .namedSubstringVector(vec_nas)             //...
      .nameToNumberMapVector(vec_ntn)            //...
      .jpcre2Options(jpcre2::VALIDATE_MODIFIER)  //...
      .pcre2Options(PCRE2_ANCHORED)              //...
      .execute();                                //Finally execute it.
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}
</code></pre>
And access the substrings by looping through the vectors and associated maps. The size of all three vectors are the same and can be accessed in the same way.
</li>
    </ul>
<li>
<b>Replace:</b> The <code>replace()</code> member function can take upto two optional arguments (subject and replacement string) and returns an object of the class <i>RegexReplace</i> which then in turn can be used to pass various parameters using  available member functions (method chaining) of <i>RegexReplace</i> class. The end function in the method chain should always be the <code>execute()</code> function which returns the result (replaced string).
</li>
    <ul>
<li>
Perform replace and catch any error exception:
<pre class="highlight prettyprint"><code class="highlight-source-c++ cpp">
try{
    std::cout&lt;&lt;
    re.replace()        //Invoke the replace() function
      .subject(s)       //Set various parameters
      .replaceWith(s2)  //...
      .modifiers("gE")  //...
      .jpcre2Options(0) //...
      .pcre2Options(0)  //...
      .execute();       //Finally execute it.
    //gE is the modifier passed (global and unknown-unset-empty).
    //Access substrings/captured groups with ${1234},$1234 (for numbered substrings)
    // or ${name} (for named substrings) in the replacement part i.e in replaceWith()
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}

</code></pre>
</li>
<li>
If you pass the size of the resultant string with <code>bufferSize()</code> function, then make sure it will be enough to store the whole resultant replaced string, otherwise the internal replace function (<code>pcre2_substitute()</code>) will be called <i>twice</i> to adjust the size of the buffer to hold the whole resultant string in order to avoid <code>PCRE2_ERROR_NOMEMORY</code> error.
</li>
    </ul>
  </ol>
</ol>

#Insight:

Let's take a quick look what's inside and how things are working here:

###Namespaces:

1. **jpcre2::utils :** Some utility functions used by JPCRE2.
2. **jpcre2 :** This is the namespace you will be using in your code to access JPCRE2 classes and functions.

###Classes:

1. **Regex :** This is the main class which holds the key utilities of JPCRE2. Every regex needs an object of this class.
2. **RegexMatch:** This is the class that holds all the useful functions to perform regex match according to the compiled pattern.
3. **RegexReplace:** This is the class that holds all the useful functions to perform replacement according to the compiled pattern.

###Functions at a glance:

```cpp
////Class Regex

//Constructors
Regex()
Regex(const String& re, const String& mod="")
Regex(const String& re, uint32_t pcre2_opts, uint32_t jpcre2_opts=0)

//Getters
String     getModifier()
String     getPattern()
String     getLocale()       ///Gets LC_CTYPE
uint32_t   getCompileOpts()  ///Returns the compile opts used for compilation

///Error handling
String     getErrorMessage(int err_num)
String     getErrorMessage()
String     getWarningMessage()
int        getErrorNumber()
int        getErrorCode()
PCRE2_SIZE getErrorOffset()

Regex&              compile(const String& re,const String& mod)
Regex&              compile(const String& re="")
Regex&              pattern(const String& re)
Regex&              modifiers(const String& x)
Regex&              locale(const String& x)
Regex&              jpcre2Options(uint32_t x)
Regex&              pcre2Options(uint32_t x)
void                execute()  //executes the compile operation.

RegexMatch&         match()
RegexReplace&       replace()

////Class RegexMatch

RegexMatch&         numberedSubstringVector(VecNum& vec_num)
RegexMatch&         namedSubstringVector(VecNas& vec_nas)
RegexMatch&         nameToNumberMapVector(VecNtN& vec_ntn)
RegexMatch&         subject(const String& s)
RegexMatch&         modifiers(const String& s)
RegexMatch&         jpcre2Options(uint32_t x=NONE)
RegexMatch&         pcre2Options(uint32_t x=NONE)
RegexMatch&         findAll()
SIZE_T              execute()  //executes the match operation


////Class RegexReplace

RegexReplace&       subject(const String& s)
RegexReplace&       replaceWith(const String& s)
RegexReplace&       modifiers(const String& s)
RegexReplace&       jpcre2Options(uint32_t x=NONE)
RegexReplace&       pcre2Options(uint32_t x=NONE)
RegexReplace&       bufferSize(PCRE2_SIZE x)
String              execute() //executes the replacement operation

```

<div id="modifiers"></div>
###Modifiers:

JPCRE2 uses modifiers to control various options, type, behavior of the regex and its' interactions with different functions that uses it. Two types of modifiers are available: **compile modifiers** and **action modifiers**:

<div id="compile-modifiers"></div>

1. **Compile modifiers:** Modifiers that are used to compile a regex. They define the behavior of a regex pattern. The modifiers have more or less the same meaning as the [PHP regex modifiers](https://php.net/manual/en/reference.pcre.pattern.modifiers.php) except for `e, j and n` (marked with <sup>*</sup>). The available compile modifiers are:
  * **e**<sup>\*</sup> : Unset back-references in the pattern will match to empty strings. Equivalent to *PCRE2_MATCH_UNSET_BACKREF*.
  * **i** : Case-insensitive. Equivalent to *PCRE2_CASELESS* option.
  * **j**<sup>\*</sup> : `\u \U \x` and unset back-referencees will act as JavaScript standard.
     * `\U` matches an upper case "U" character (by default it causes a compile time error if this option is not set).
     * `\u` matches a lower case "u" character unless it is followed by four hexadecimal digits, in which case the hexadecimal number defines the code point to match (by default it causes a compile time error if this option is not set).
     * `\x` matches a lower case "x" character unless it is followed by two hexadecimal digits, in which case the hexadecimal number defines the code point to match (By default, as in Perl, a hexadecimal number is always expected after `\x`, but it may have zero, one, or two digits (so, for example, `\xz` matches a binary zero character followed by z) ).
     * Unset back-references in the pattern will match to empty strings.
  * **m** : Multi-line regex. Equivalent to *PCRE2_MULTILINE* option.
  * **n**<sup>\*</sup> : Enable Unicode support for `\w \d` etc... in pattern. Equivalent to *PCRE2_UTF | PCRE2_UCP*.
  * **s** : If this modifier is set, a dot meta-character in the pattern matches all characters, including newlines. Equivalent to *PCRE2_DOTALL* option.
  * **u** : Enable UTF support.Treat pattern and subjects as UTF strings. It is equivalent to *PCRE2_UTF* option.
  * **x** : Whitespace data characters in the pattern are totally ignored except when escaped or inside a character class, enables commentary in pattern. Equivalent to *PCRE2_EXTENDED* option.
  * **A** : Match only at the first position. It is equivalent to *PCRE2_ANCHORED* option.
  * **D** : A dollar meta-character in the pattern matches only at the end of the subject string. Without this modifier, a dollar also matches immediately before the final character if it is a newline (but not before any other newlines). This modifier is ignored if *m* modifier is set. Equivalent to *PCRE2_DOLLAR_ENDONLY* option.
  * **J** : Allow duplicate names for subpatterns. Equivalent to *PCRE2_DUPNAMES* option.
  * **S** : When a pattern is going to be used several times, it is worth spending more time analyzing it in order to speed up the time taken for matching/replacing. It may also be beneficial for a very long subject string or pattern. Equivalent to an extra compilation with JIT_COMPILER with the option *PCRE2_JIT_COMPLETE*.
  * **U** : This modifier inverts the "greediness" of the quantifiers so that they are not greedy by default, but become greedy if followed by `?`. Equivalent to *PCRE2_UNGREEDY* option.
2. **Action modifiers:** Modifiers that are used per action i.e match or replace. These modifiers are not compiled in the regex itself, rather it is used per call of each function. Available action modifiers are:
  * **A** : Match at start. Equivalent to *PCRE2_ANCHORED*. Can be used in match operation. Setting this option only at match time (i.e regex was not compiled with this option) will disable optimization during match time.
  * **e** : Replaces unset group with empty string. Equivalent to *PCRE2_SUBSTITUTE_UNSET_EMPTY*. Can be used in replace operation.
  * **E** : Extension of *e* modifier. Sets even unknown groups to empty string. Equivalent to *PCRE2_SUBSTITUTE_UNSET_EMPTY | PCRE2_SUBSTITUTE_UNKNOWN_UNSET*.
  * **g** : Global. Will perform global matching or replacement if passed.
  * **x** : Extended replacement operation. It enables some Bash like features:
     1. `${<n>:-<string>}`
     2. `${<n>:+<string1>:<string2>}`

  `<n>` may be a group number or a name. The first form specifies a default value. If group `<n>` is set, its value is inserted; if not, `<string>` is expanded and the result is inserted. The second form specifies strings that are expanded and inserted when group `<n>` is set or unset, respectively. The first form is just a convenient shorthand for `${<n>:+${<n>}:<string>}`.

<div id="jpcre2-options"></div>

###JPCRE2 options:

These options are meaningful only for the JPCRE2 library itself not the original PCRE2 library. We use the `jpcre2Options()` function to pass these options.

1. **jpcre2::NONE**: This is the default option. Equivalent to 0 (zero).
2. **jpcre2::VALIDATE_MODIFIER**: If this option is passed, modifiers will be subject to validation check. If any of them is invalid then a `jpcre2::ERROR::INVALID_MODIFIER` error exception will be thrown. You can get the error message with `getErrorMessage(error_code)` member function.
3. **jpcre2::FIND_ALL**: This option will do a global matching if passed during matching. The same can be achieved by passing the 'g' modifier with `modifiers()` function.
4. **jpcre2::ERROR_ALL**: Treat warnings as errors and throw exception.
5. **jpcre2::JIT_COMPILE**: This is same as passing the **S** modifier during pattern compilation.

###PCRE2 options:

While having its own way of doing things, JPCRE2 also supports the traditional PCRE2 options to be passed. We use the `pcre2Options()` function to pass the PCRE2 options. These options are the same as the PCRE2 library and have the same meaning. For example instead of passing the 'g' modifier to the replacement operation we can also pass its PCRE2 equivalent *PCRE2_SUBSTITUTE_GLOBAL* to have the same effect.

###Common properties of several functions:

1. All other functions in method chain except `jpcre2Options()` and `pcre2Options()` re-initiates the corresponding value/s. Multiple call will overwrite existing value
2. Multiple call to `jpcre2Options()` and `pcre2Options()` in the method chain will add further specified options.
2. All values get re-initiated during the three function calls: `compile()`, `match()` and `replace()`.


<div id="short-examples"></div>
#Short examples

```cpp
size_t count;
///Check if string matches the pattern
/**
 * The following can be used to check if a string matches a pattern. 
 * If match is found 1 will be returned, 0 otherwise.
 * */
if(jpcre2::Regex("(\\d)|(\\w)").match("I am the subject").execute()) 
    std::cout<<"\nmatched";
else
    std::cout<<"\nno match";
/**
 * The above is a good example of using temporary objects to perform match (or replace)
 * 
 * Using the modifier S (i.e jpcre2::JIT_COMPILE) with temporary object may not give you
 * any performance boost.
 * */
 
///If you want to match all and get the match count, use the action modifier 'g':
std::cout<<"\n"<<
    jpcre2::Regex("(\\d)|(\\w)","m").match("I am the subject").modifiers("g").execute();

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
jpcre2::Regex("(\\w+)\\s*(\\d+)","m").match("I am 23, I am digits 10")
                                     .modifiers("g")
                                     .numberedSubstringVector(vec_num)
                                     .execute();
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

///If the existence of a substring is important, use the std::map::at() function to access map elements
try{
    ///This will throw exception, because substring 4 doesn't exist
    std::cout<<"\nCaptrued group 4 of frist match: "<<vec_num[0].at(4);
} catch (std::logic_error e){
    std::cout<<"\nCaptrued group 4 doesn't exist";
}

///There were two matches found (vec_num.size() == 2) in the above example
std::cout<<"\nTotal match of second match: "<<vec_num[1][0];      ///Total match (group 0) from second match
std::cout<<"\nCaptrued group 1 of second match: "<<vec_num[1][1]; ///captured group 1 from second match 
std::cout<<"\nCaptrued group 2 of second match: "<<vec_num[1][2]; ///captured group 2 from second match


/// ***** Get named substring ***** ///

jpcre2::VecNas vec_nas;
jpcre2::VecNtN vec_ntn; /// We will get name to number map vector too
count = 
jpcre2::Regex("(?<word>\\w+)\\s*(?<digit>\\d+)","m").match("I am 23, I am digits 10")
                                                    .modifiers("g")
                                                    ///.numberedSubstringVector(vec_num) /// We don't need it in this example
                                                    .namedSubstringVector(vec_nas)
                                                    .nameToNumberMapVector(vec_ntn) /// Additional (name to number maps)
                                                    .execute();
std::cout<<"\nNumber of matches: "<<vec_nas.size()/* or count */;
///Now vec_nas is populated with named substrings for each match
///The size of vec_nas is the total match count
///vec_nas[0] is the first match
///The type of vec_nas[0] is jpcre2::MapNas
std::cout<<"\nCaptured group (word) of first match: "<<vec_nas[0]["word"];
std::cout<<"\nCaptured group (digit) of first match: "<<vec_nas[0]["digit"];

///If the existence of the particular name in the pattern is important use std::map::at() function
try{
    ///This will throw exception becasue the substring name 'name' doesn't exist
    std::cout<<"\nCaptured group (name) of first match: "<<vec_nas[0].at("name");
} catch(std::logic_error e){
    std::cout<<"\nCaptured group (name) doesn't exist";
}

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
 * The replace() function can take a subject and replacement string as argument.
 * You can also pass the subject with subject() function in method chain,
 * replacement string with replaceWith() function in method chain, etc ...
 * 
 * A call to replace() must end with the call to execute()
 * */

std::cout<<"\n"<<
///replace first occurrence of a digit with @
jpcre2::Regex("\\d").replace("I am the subject string 44","@").execute();

std::cout<<"\n"<<
///replace all occrrences of a digit with @
jpcre2::Regex("\\d").replace("I am the subject string 44","@").modifiers("g").execute();

///swap two parts of a string
std::cout<<"\n"<<
jpcre2::Regex("^([^\t]+)\t([^\t]+)$").replace()
                                     .subject("I am the subject\tTo be swapped according to tab")
                                     .replaceWith("$2 $1")
                                     .execute();
```

#Testing:

2. **test_match.cpp**: Contains an example code for match function.
3. **test_replace.cpp**: Contains an example code for replace function.
4. **test_match2.cpp**: Another matching example. The makefile creates a binary of this (jpcre2match).
5. **test_replace2.cpp**: Another replacement example. The makefile creates a binary of this (jpcre2replace).

#Screenshots of some test outputs:

test_match:
----------

```
subject = "(I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56)"
pattern = "(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)"
```

![jpcre2 screenshot1](img/s1_1.png)
![jpcre2 screenshot2](img/s1_2.png)
![jpcre2 screenshot3](img/s1_3.png)


