/* *****************************************************************************
 * ******************* C++ wrapper of PCRE2 Library ****************************
 * *****************************************************************************
 *            Copyright (c) 2015-2016 Md. Jahidul Hamid
 * 
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 *     * The names of its contributors may be used to endorse or promote 
 *       products derived from this software without specific prior written
 *       permission.
 *       
 * Disclaimer:
 * 
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *     IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *     ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *     CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *     SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *     INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *     ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 * */

/** @file jpcre2.hpp
 * @brief Main header file for JPCRE2 library to be included by programs that uses its functionalities.
 * It includes the pcre2.h header, therefore you shouldn't include pcre2.h
 * separately in your program. Make sure to link both JPCRE2 and PCRE2 library when compiling.
 *
 * If you are using JPCRE2 with all of its source files, you won't need to link it with JPCRE2 library, but do remember that you
 * still need to link with  PCRE2 library
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 */

#ifndef JPCRE2_HPP
#define JPCRE2_HPP

#ifndef PCRE2_CODE_UNIT_WIDTH
/// Code unit width 8 is used by default
#define PCRE2_CODE_UNIT_WIDTH 8
#endif

#include <pcre2.h> 
//#include <stdint.h> 	// uint32_t		//pcre2 itself includes this
#include <cstddef>  	// std::size_t
#include <string>  		// std::string
#include <vector>   	// std::vector
#include <map>      	// std::map
#include <exception>    // std::exception

/** Top level namespace of JPCRE2.
 *
 * All functions, classes, constants, enums that are provided by JPCRE2 belong to this namespace while
 * **PCRE2** functions, constants remain outside of its scope.
 *
 * If you want to use any PCRE2 functions or constants,
 *  remember that they are in the global scope and should be used as such.
 * */
namespace jpcre2 {

typedef std::size_t SIZE_T;                     ///< Used for match count and vector size
typedef uint32_t Uint;                          ///< Used for options (bitwise operation)
typedef std::string String;                     ///< Used as std::string
typedef std::map<String, String> MapNas;        ///< Map for Named substrings
typedef std::map<SIZE_T, String> MapNum;        ///< Map for Numbered substrings
typedef std::map<String, SIZE_T> MapNtN;        ///< Substring name to Substring number map
typedef MapNtN MapNtn;                          ///< Allow spelling mistake of MapNtN as MapNtn
typedef std::vector<MapNas> VecNas;             ///< Vector of matches with named substrings
typedef std::vector<MapNtN> VecNtN;             ///< Vector of substring name to Substring number map
typedef VecNtN VecNtn;                          ///< Allow spelling mistake of VecNtN as VecNtn
typedef std::vector<MapNum> VecNum;             ///< Vector of matches with numbered substrings

/**Namespace to provide information about JPCRE2 library itself.
 * Contains constant Strings with version info.
 * */
namespace INFO{
extern const String NAME;					///< Name of the project
extern const String FULL_VERSION;			///< Full version string
extern const String VERSION_GENRE;			///< Generation, depends on original PCRE2 version
extern const String VERSION_MAJOR;			///< Major version, updated when API change is made
extern const String VERSION_MINOR;			///< Minor version, includes bug fix or minor feature upgrade
extern const String VERSION_PRE_RELEASE;	///< Alpha or beta (testing) release version
}
/// Namespace for error codes
namespace ERROR {

/**ERROR numbers.
 *  JPCRE2 error numbers are positive integers while
 *  PCRE2 error numbers are negative integers.
 * */
enum {
	INVALID_MODIFIER        = 2,        ///< Error number implying that invalid modifier was detected
	JIT_COMPILE_FAILED      = 3         ///< Error number implying that JIT compile failed
};
}

extern const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE;    ///< Used by default to provide big enough initial buffer for replaced string
extern const String LOCALE_NONE;                    ///< Don't do anything about locale if it is set to #LOCALE_NONE
extern const String LOCALE_DEFAULT;                 ///< Default locale
extern const String JIT_ERROR_MESSAGE_PREFIX;       ///< Prefix to be added to JIT error message

/** These constants provide JPCRE2 options.
 * */
enum {
	NONE                    = 0x0000000u,   ///< Option 0 (zero)
	VALIDATE_MODIFIER       = 0x0000001u,   ///< Perform validation check on modifiers and throw
											///< jpcre2::ERROR::INVALID_MODIFIER if any wrong modifier is passed
	FIND_ALL                = 0x0000002u,   ///< Find all during match (global match)
	JIT_COMPILE             = 0x0000004u,   ///< Perform JIT compilation for optimization
	ERROR_ALL               = 0x0000008u    ///< Treat warnings as error and throw exception (warnings don't throw exception)
};

/** Namespace for modifier constants.
 *  For each modifier constant there is a jpcre2::Uint option value.
 *  Some modifiers may have multiple values set together (ORed in bitwise operation) and
 *  thus they may include other modifiers. Such an example is the 'n' modifier. It is combined together with 'u'.
 * */
namespace MOD {
extern const String C_N;        ///< String of compile modifier characters for PCRE2 options
extern const Uint C_V[];        ///< Array of compile modifier values for PCRE2 options
extern const String CJ_N;       ///< String of compile modifier characters for JPCRE2 options
extern const Uint CJ_V[];       ///< Array of compile modifier values for JPCRE2 options
extern const String M_N;        ///< String of action (match) modifier characters for PCRE2 options
extern const Uint M_V[];        ///< Array of action (match) modifier values for PCRE2 options
extern const String MJ_N;       ///< String of action (match) modifier characters for JPCRE2 options
extern const Uint MJ_V[];       ///< Array of action (match) modifier values for JPCRE2 options
extern const String R_N;        ///< String of action (replace) modifier characters for PCRE2 options
extern const Uint R_V[];        ///< Array of action (replace) modifier values for PCRE2 options
extern const String RJ_N;       ///< String of action (replace) modifier characters for JPCRE2 options
extern const Uint RJ_V[];       ///< Array of action (replace) modifier values for JPCRE2 options
}

/// Namespace for some utility functions
namespace utils {
extern String toString(int);                      ///< Converts an integer to String
extern String toString(char);                     ///< Converts a char to String
extern String toString(const char*);              ///< Converts const char* to String
extern String toString(PCRE2_UCHAR*);             ///< Converts a PCRE2_UCHAR* to String
extern std::string getPcre2ErrorMessage(int);     ///< Get PCRE2 error message for an error number
extern std::string getErrorMessage(int, int);            ///< Get error message from error number and error offset

/// Used throughout JPCRE2 to throw exceptions.
extern void throwException(int, int);
}

/**Class to handle exception.
 * Provides public functions to get the error number,
 * error offset and error message.
 * 
 * Any exception should be caught by reference, e.g
 * ```
 * try {
 *     throw Except("Error", 1, 2);
 * }
 * catch( Except& e){
 *     std::cout<<e.what();
 * }
 * ```
 * */
class Except: public std::exception {
    
protected:

    int error_number;               ///< Error number
    int error_offset;               ///< Error offset
    std::string error_message;      ///< Error message
    
public:

    /** Constructor (C++ STL strings).
     *  @param msg The error message.
     *  @param err_num Error number
     *  @param err_off Error offset
     */
    explicit 
    Except(const std::string& msg, int err_num, int err_off):
        error_number(err_num),
        error_offset(err_off),
        error_message(msg)
        {}

    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~Except() throw () {}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in posession of the Except object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw () {
       return error_message.c_str();
    }
    
    /**Returns error number
     * @return #error_number
     * */
    virtual int getErrorNumber() const throw() {
        return error_number;
    }
    
    /**Returns error offset
     * @return #error_offset
     * */
    virtual int getErrorOffset() const throw() {
        return error_offset;
    }
    

    /** Just another name for what() for convenience
     *  Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* getErrorMessage() const throw() {
        return error_message.c_str();
    }

};

/// Forward declaration of Regex class
class Regex;



/** @class RegexMatch
 *  Provides the RegexMatch::match() function to perform regex matching.
 * 
 *  Provides chained methods to set various options.
 *
 *  All constructors of this class are private.
 */
class RegexMatch {

private:

	Regex* re;              ///< This is used to access private members in Regex.

	String m_subject;       ///< Subject string for match
	Uint match_opts;        ///< PCRE2 options for pcre2_match() (PCRE2 internal function)
	Uint jpcre2_match_opts; ///< JPCRE2 options for match

	VecNum* vec_num;        ///< Pointer to vector that will store the numbered substring maps
	VecNas* vec_nas;        ///< Pointer to vector that will store the named substring maps
	VecNtN* vec_ntn;        ///< Pointer to vector that will store the name to number maps

	MapNum* num_map0;       ///< Pointer to map that will store numbered substrings temporarily
	MapNas* nas_map0;       ///< Pointer to map that will store named substrings temporarily
	MapNtN* ntn_map0;       ///< Pointer to map that will store name to number mapping temporarily

	void getNumberedSubstrings(int, pcre2_match_data *);   ///< Populate #num_map0 with numbered substrings

	void getNamedSubstrings(int, int, PCRE2_SPTR, pcre2_match_data *);   ///< Populate #nas_map0 and/or #ntn_map0
                                                                         ///< with named substring and/or name to number mapping
    
    /// Push maps into the vectors pointed by #vec_num, #vec_nas and #vec_ntn and thus store match results in them.
    void pushMapsIntoVectors(void){
        if (vec_num)
            vec_num->push_back(*num_map0);
        if (vec_nas)
            vec_nas->push_back(*nas_map0);
        if (vec_ntn)
            vec_ntn->push_back(*ntn_map0);
    }
    
    /// Initialize class variables.
    /// Do not call this without releasing vector/map memory.
	void init_vars() {
		vec_num = 0;
		vec_nas = 0;
		vec_ntn = 0;
		num_map0 = 0;
		nas_map0 = 0;
		ntn_map0 = 0;
		match_opts = 0;
		jpcre2_match_opts = 0;
	}

	/// Default constructor.
	/// Initialize class variables.
	RegexMatch() {
		init_vars();
	}

	/// This is a copy constructor which is only used to prevent public object creation.
	/// No need to implement it completely.
	RegexMatch(const RegexMatch&) {
		init_vars();
	}

	/// Destructor.
	/// Deletes the temporary maps that were created to store substrings
	~RegexMatch() {
		delete num_map0;
		delete nas_map0;
		delete ntn_map0;
	}

	/// Define class Regex as friend and thus allow Regex to create object of this class
	friend class Regex;

public:

	/// Set a pointer to the numbered substring vector of type jpcre2::VecNum
	/// @param v #vec_num
	/// @return RegexMatch&
	RegexMatch& setNumberedSubstringVector(VecNum* v) {
		vec_num = v;
		return *this;
	}

	/// Set a pointer to the named substring vector of type jpcre2::VecNas
	/// @param v #vec_nas
	/// @return RegexMatch&
	RegexMatch& setNamedSubstringVector(VecNas* v) {
		vec_nas = v;
		return *this;
	}

	/// Set a pointer to the name to number map vector of type jpcre2::VecNtN
	/// @param v #vec_ntn
	/// @return RegexMatch&
	RegexMatch& setNameToNumberMapVector(VecNtN* v) {
		vec_ntn = v;
		return *this;
	}

	/// Set the subject string #m_subject
	/// @param s Subject string
	/// @return RegexMatch&
	/// @see RegexReplace::setSubject()
	RegexMatch& setSubject(const String& s) {
		m_subject = s;
		return *this;
	}

	/// Set the modifier (overwrites existing JPCRE2 and PCRE2 option).
	/// Re-initializes the option bits for PCRE2 and JPCRE2 options, then parses the modifier to set their equivalent options.
	///
	/// **Note:** If speed of operation is very crucial, use RegexMatch::setJpcre2Option() and
	/// RegexMatch::setPcre2Option() with equivalent options. It will be faster that way.
	/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER as error number
    ///        if jpcre2::VALIDATE_MODIFIER or jpcre2::ERROR_ALL is set and a wrong modifier was encountered.
	/// @param s Modifier string
	/// @return RegexMatch&
	/// @see RegexReplace::setModifier()
	/// @see Regex::setModifier()
	RegexMatch& setModifier(const String& s) {
		match_opts = 0;
        jpcre2_match_opts = 0;
        changeModifier(s, true);
		return *this;
	}

	/// Set JPCRE2 option #jpcre2_match_opts (overwrite existing option)
	/// @param x Option value
	/// @return RegexMatch&
	/// @see RegexReplace::setJpcre2Option()
	/// @see Regex::setJpcre2Option()
	RegexMatch& setJpcre2Option(Uint x) {
		jpcre2_match_opts = x;
		return *this;
	}

	/// Set PCRE2 option #match_opts (overwrite existing option)
	/// @param x Option value
	/// @return RegexMatch&
	/// @see RegexReplace::setPcre2Option()
	/// @see Regex::setPcre2Option()
	RegexMatch& setPcre2Option(Uint x) {
		match_opts = x;
		return *this;
	}
    
	/// Set whether to perform global match
	/// @param x True or False
	/// @return RegexMatch&
	RegexMatch& setFindAll(bool x) {
		if (x)
			jpcre2_match_opts |= FIND_ALL;
		else
			jpcre2_match_opts &= ~FIND_ALL;
		return *this;
	}

	///@overload
	///
	///This function just calls RegexMatch::setFindAll(bool x) with `true` as the parameter
	///@return RegexMatch&
	RegexMatch& setFindAll() {
		return setFindAll(true);
	}

	/// Parse modifier and add/remove equivalent PCRE2 and JPCRE2 options.
	RegexMatch& changeModifier(const String&, bool);

    /// Add or remove a JPCRE2 option
    /// @param opt JPCRE2 option value
    /// @param x Add the option if it's true, remove otherwise.
    /// @return Regex&
	/// @see RegexReplace::changeJpcre2Option()
	/// @see Regex::changeJpcre2Option()
    RegexMatch& changeJpcre2Option(Uint opt, bool x) {
        if(x)
            jpcre2_match_opts |= opt;
        else
            jpcre2_match_opts &= ~opt;
        return *this;
    }

    /// Add or remove a PCRE2 option
    /// @param opt PCRE2 option value
    /// @param x Add the option if it's true, remove otherwise.
    /// @return Regex&
	/// @see RegexReplace::changePcre2Option()
	/// @see Regex::changePcre2Option()
    RegexMatch& changePcre2Option(Uint opt, bool x) {
        if(x)
            match_opts |= opt;
        else
            match_opts &= ~opt;
        return *this;
    }
    
    /// Parse modifier string and add equivalent PCRE2 and JPCRE2 options.
    /// This is just a wrapper of the original function RegexMatch::changeModifier()
    /// provided for convenience.
	///
	/// **Note:** If speed of operation is very crucial, use RegexMatch::addJpcre2Option() and RegexMatch::addPcre2Option()
    /// with equivalent options. It will be faster that way.
	/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER as error number
    ///        if jpcre2::VALIDATE_MODIFIER or jpcre2::ERROR_ALL is set and a wrong modifier was encountered.
    /// @param mod Modifier string
    /// @return RegexMatch&
    /// @see RegexReplace::addModifier()
    /// @see Regex::addModifier()
    RegexMatch& addModifier(const String& mod){
        return changeModifier(mod, true);
    }

	/// Add option to existing JPCRE2 options #jpcre2_match_opts
	/// @param x Option value
	/// @return RegexMatch&
	/// @see RegexReplace::addJpcre2Option()
	/// @see Regex::addJpcre2Option()
	RegexMatch& addJpcre2Option(Uint x) {
		jpcre2_match_opts |= x;
		return *this;
	}

	/// Add option to existing PCRE2 options #match_opts
	/// @param x Option value
	/// @return RegexMatch&
	/// @see RegexReplace::addPcre2Option()
	/// @see Regex::addPcre2Option()
	RegexMatch& addPcre2Option(Uint x) {
		match_opts |= x;
		return *this;
	}

	/// Store match result in vectors and return the number of matches.
	SIZE_T match(void);
};

/** @class RegexReplace
 *  Provides the RegexReplace::replace() function to perform regex replace on a string.
 * 
 *  Provides chained methods to set various options.
 *
 *  All constructors of this class are private.
 */
class RegexReplace {

private:

	Regex* re;                  ///< This is used to access private members in Regex.

	String r_subject;           ///< Subject string for replace.
	String r_replw;             ///< Replacement string i.e string to replace with
	Uint replace_opts;          ///< PCRE2 options for pcre2_substitute() (PCRE2 internal function)
	Uint jpcre2_replace_opts;   ///< JPCRE2 options
	PCRE2_SIZE buffer_size;     ///< Size of the resultant string after replacement.
								///< Used to allocate enough memory for replaced string by PCRE2
								///< internal function pcre2_substitute.
								///< Initialized to #SUBSTITUTE_RESULT_INIT_SIZE.

	/// Initialize class variables
	void init_vars() {
        ///This enables returning the required length of string
        replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH;
        ///in case substitute fails due to insufficient memory. It is required to try again with the correct amount of
        ///memory allocation.
        jpcre2_replace_opts = 0;
		buffer_size = SUBSTITUTE_RESULT_INIT_SIZE;
	}

	/// Default constructor.
	/// Initialize class variables
	RegexReplace() {
		init_vars();
	}

	/// This is a copy constructor which is only used to prevent public object creation.
	/// No need to implement it completely
	RegexReplace(const RegexReplace&) {
		init_vars();
	}

	/// Destructor.
	/// Nothing to be done here.
	~RegexReplace() {}

	/// Define Regex as a friend so that it can create object of this class.
	friend class Regex;

public:

	/** Set the subject string #r_subject
	 * @param s Subject string
	 * @return RegexReplace&
	 * @see RegexMatch::setSubject()
	 * */
	RegexReplace& setSubject(const String& s) {
		r_subject = s;
		return *this;
	}

	/** Set the replacement string #r_replw
	 * @param s String to replace with
	 * @return RegexReplace&
	 * */
	RegexReplace& setReplaceWith(const String& s) {
		r_replw = s;
		return *this;
	}

	/** Set the modifier string (overwrites existing JPCRE2 and PCRE2 option).
	 *
	 * **Note:** If speed of operation is very crucial, use RegexReplace::setJpcre2Option() and RegexReplace::setPcre2Option()
	 * with equivalent options. It will be faster that way.
	/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER as error number
    ///        if jpcre2::VALIDATE_MODIFIER or jpcre2::ERROR_ALL is set and a wrong modifier was encountered.
	 * @param s Modifier string
	 * @return RegexReplace&
	 * @see RegexMatch::setModifier()
	 * @see Regex::setModifier()
	 * */
	RegexReplace& setModifier(const String& s) {
		replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; //must not be initialized to 0
        jpcre2_replace_opts = 0;
        return changeModifier(s, true);
	}

	/** Set the initial buffer size (#buffer_size) to be allocated for replaced string (used by PCRE2)
	 * @param x Buffer size
	 * @return RegexReplace&
	 * */
	RegexReplace& setBufferSize(PCRE2_SIZE x) {
		buffer_size = x;
		return *this;
	}

	/** Set JPCRE2 option #jpcre2_replace_opts (overwrite existing option)
	 * @param x Option value
	 * @return RegexReplace&
	 * @see RegexMatch::setJpcre2Option()
	 * @see Regex::setJpcre2Option()
	 * */
	RegexReplace& setJpcre2Option(Uint x) {
		jpcre2_replace_opts = x;
		return *this;
	}

	/** Set PCRE2 option #replace_opts (overwrite existing option)
	 * @param x Option value
	 * @return RegexReplace&
	 * @see RegexMatch::setPcre2Option()
	 * @see Regex::setPcre2Option()
	 * */
	RegexReplace& setPcre2Option(Uint x) {
		replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH | x;
		return *this;
	}

	/// Parse modifier and add/remove equivalent PCRE2 and JPCRE2 options.
    RegexReplace& changeModifier(const String&, bool);
    
    /// Add or remove a JPCRE2 option
    /// @param opt JPCRE2 option value
    /// @param x Add the option if it's true, remove otherwise.
    /// @return Regex&
	/// @see RegexMatch::changeJpcre2Option()
	/// @see Regex::changeJpcre2Option()
    RegexReplace& changeJpcre2Option(Uint opt, bool x) {
        if(x)
            jpcre2_replace_opts |= opt;
        else
            jpcre2_replace_opts &= ~opt;
        return *this;
    }

    /// Add or remove a PCRE2 option
    /// @param opt PCRE2 option value
    /// @param x Add the option if it's true, remove otherwise.
    /// @return Regex&
	/// @see RegexMatch::changePcre2Option()
	/// @see Regex::changePcre2Option()
    RegexReplace& changePcre2Option(Uint opt, bool x) {
        if(x)
            replace_opts |= opt;
        else
            replace_opts &= ~opt;
            replace_opts |= PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; //It's important, this option must not be removed
        return *this;
    }
    
    /// Parse modifier string and add equivalent PCRE2 and JPCRE2 options.
    /// This is just a wrapper of the original function RegexReplace::changeModifier()
    /// provided for convenience.
	///
	/// **Note:** If speed of operation is very crucial, use RegexReplace::addJpcre2Option() and
	/// RegexReplace::addPcre2Option() with equivalent options. It will be faster that way.
	/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER as error number
    ///        if jpcre2::VALIDATE_MODIFIER or jpcre2::ERROR_ALL is set and a wrong modifier was encountered.
    /// @param mod Modifier string
    /// @return RegexReplace&
    /// @see RegexMatch::addModifier()
    /// @see Regex::addModifier()
    RegexReplace& addModifier(const String& mod){
        return changeModifier(mod, true);
    }

	/** Add specified JPCRE2 option to existing options #jpcre2_replace_opts
	 * @param x Option value
	 * @return RegexReplace&
	 * @see RegexMatch::addJpcre2Option()
	 * @see Regex::addJpcre2Option()
	 * */
	RegexReplace& addJpcre2Option(Uint x) {
		jpcre2_replace_opts |= x;
		return *this;
	}

	/** Add specified PCRE2 option to existing options #replace_opts
	 * @param x Option value
	 * @return RegexReplace&
	 * @see RegexMatch::addPcre2Option()
	 * @see Regex::addPcre2Option()
	 * */
	RegexReplace& addPcre2Option(Uint x) {
		replace_opts |= x;
		return *this;
	}

	/** Returns the resultant string after performing regex replace.
     * */
	String replace(void);
};

/** @class Regex
 * Implements public overloaded and copy constructors, provides functions to set/unset various options and perform regex match
 * and replace against a compiled pattern.
 *
 * Each regex pattern needs an object of this class.
 *
 * A pattern must be compiled either by explicitly calling the compile function or using one of the parameterized constructors.
 *
 * */
class Regex {

private:

	RegexMatch *rm;		        ///< Pointer to RegexMatch object
	RegexReplace *rr;	        ///< Pointer to RegexReplace object

	String pat_str;		        ///< Pattern string
	pcre2_code *code;	        ///< Pointer to compiled pattern
	Uint compile_opts;          ///< Compile options for PCRE2 (used by PCRE2 internal function pcre2_compile())
	Uint jpcre2_compile_opts;	///< Compile options specific to JPCRE2
	String mylocale;	        ///< Locale as a string
	String current_warning_msg;	///< Current warning message

	/// Initialize class variables
	void init_vars() {
		mylocale = LOCALE_DEFAULT;
		jpcre2_compile_opts = 0;
		compile_opts = 0;
		code = 0;
		rr = 0;
		rm = 0;
	}

	/// Call Regex::init_vars() and initialize class variables.
	/// This function should not be attempted to call after creating object.
	/// To re-initialize class variables at a later stage after
	/// creating object, use the Regex::reset() function.
	/// This function is private and should remain as such.
	void init() {
		init_vars();
	}

	/// @overload
	/// @param re Regex pattern
	/// @param mod Modifier string
	void init(const String& re, const String& mod) {
		init_vars();
		pat_str = re;
        changeModifier(mod, true);
	}

	/// @overload
	/// @param re Regex pattern
	/// @param po PCRE2 options
	/// @param jo JPCRE2 options
	void init(const String& re, Uint po, Uint jo) {
		init_vars();
		pat_str = re;
		compile_opts = po;
		jpcre2_compile_opts = jo;
	}

	/// Free #code if it's non-NULL
	void freeRegexMemory(void) {
		if (code)
			pcre2_code_free(code);
	}

	/// Define RegexMatch as friends. It needs to access the compiled pattern which is a private property of this class.
	friend class RegexMatch;
	/// Define RegexReplace as friends. It needs to access the compiled pattern which is a private property of this class.
	friend class RegexReplace;

	/// Do a shallow copy of class variables
	void shallowCopy(const Regex& r) {
		pat_str = r.pat_str;
		mylocale = r.mylocale;
		compile_opts = r.compile_opts;
		jpcre2_compile_opts = r.jpcre2_compile_opts;
		current_warning_msg = r.current_warning_msg;
	}

	/// Do a deep copy of #rm, #rr and #code
	void deepCopy(const Regex&);

public:

	/// Default Constructor.
	/// Initializes all class variables to defaults.
	/// Does not perform any compilation, and thus not expected to throw any exception.
	Regex() {
		init();
	}

	/** Compile pattern with initialization.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 * */
	Regex(const String& re) {
		compile(re, 0, 0);
	}

	/** @overload
	 *
	 *
	 *  Compile pattern.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 *  @param mod Modifier string
	 * */
	Regex(const String& re, const String& mod) {
		compile(re, mod);
	}

	/** @overload
	 *
	 *
	 *  Compile pattern.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 *  @param pcre2_opts PCRE2 option value
	 * */
	Regex(const String& re, Uint pcre2_opts) {
		compile(re, pcre2_opts, 0);
	}

	/** @overload
	 *
	 *
	 *  Compiles pattern.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 *  @param pcre2_opts	PCRE2 option value
	 *  @param opt_bits		JPCRE2 option value
	 * */
	Regex(const String& re, Uint pcre2_opts, Uint opt_bits) {
		compile(re, pcre2_opts, opt_bits);
	}

	/// @overload
	/// Copy constructor\. Performs a deep copy.
    /// @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	/// @param r const Regex&
	Regex(const Regex& r) {
		///shallow copy must be performed **before** deep copy
		shallowCopy(r);
		deepCopy(r);
	}

	/// Overloaded assignment operator.
	/// Performs a deep copy.
	///
	/// Allows assigning objects like this:
	/// ```cpp
	/// Regex re;
	/// re = Regex("new pattern");
	/// ```
	/// However, use of this method is discouraged (Use Regex::compile() instead), because a call to this function
	/// requires an additional call to PCRE2 internal function pcre2_code_copy().
	/// If the pattern was JIT compiled, it requires another additional JIT compilation because
	/// JIT memory was not copied by pcre2_code_copy().
	///
	/// **Memory management:** Old JIT memory will be released along with the old compiled code.
    /// @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	/// @param r const Regex&
	/// @return *this
	Regex& operator=(const Regex& r) {
		if (this == &r)
			return *this;
		///shallow copy must be performed before deep copy
		shallowCopy(r);
		deepCopy(r);
		return *this;
	}

	/// Destructor
	/// Deletes memory used by #rm an #rr
	~Regex() {
		freeRegexMemory();
		delete rm;
		delete rr;
	}

	/** Reset all class variables to its default (initial) state.
     * Release any memory used by existing compiled pattern.
	 * @return *this
	 * */
	Regex& reset() {
		freeRegexMemory();
		init();
		return *this;
	}

	/** Get modifier string calculated from JPCRE2 and PCRE2 options.
	 **/
	String getModifier();

	/** Get pattern string
	 *  @return #pat_str
	 * */
	String getPattern() {
		return pat_str;
	}

	/** Get locale as a string
	 *  @return #mylocale
	 * */
	String getLocale() {
		return mylocale;
	}

	/** Get PCRE2 option
	 *  @return #compile_opts
	 * */
	Uint getPcre2Option() {
		return compile_opts;
	}

	/** Get JPCRE2 option
	 *  @return #jpcre2_compile_opts
	 * */
	Uint getJpcre2Option() {
		return jpcre2_compile_opts;
	}


	/// Get current warning message
	/// @return #current_warning_msg
	String getWarningMessage() {
		return current_warning_msg;
	}

	/// Set the Pattern string #pat_str
	/// @param re Pattern string
	/// @return Regex&
	Regex& setPattern(const String& re) {
		pat_str = re;
		return *this;
	}

	/// Set the modifier (overwrite existing JPCRE2 and PCRE2 option).
	/// Re-initializes the option bits for PCRE2 and JPCRE2 options, then parses the modifier and sets
	/// equivalent PCRE2 and JPCRE2 options.
	///
	/// **Note:** If speed of operation is very crucial, use Regex::setJpcre2Option() and
	/// Regex::setPcre2Option() with equivalent options. It will be faster that way.
	/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER as error number
    ///        if jpcre2::VALIDATE_MODIFIER or jpcre2::ERROR_ALL is set and a wrong modifier was encountered.
	/// @param x Modifier string
	/// @return Regex&
	/// @see RegexMatch::setModifier()
	/// @see RegexReplace::setModifier()
	Regex& setModifier(const String& x) {
		compile_opts = 0;
        jpcre2_compile_opts = 0;
		return changeModifier(x, true);
	}

	/// Set the locale #mylocale.
	/// @param x Locale string
	/// @return Regex&
	Regex& setLocale(const String& x) {
		mylocale = x;
		return *this;
	}

	/// Set JPCRE2 option #jpcre2_compile_opts (overwrites existing option)
	/// @param x Option value
	/// @return Regex&
	/// @see RegexMatch::setJpcre2Option()
	/// @see RegexReplace::setJpcre2Option()
	Regex& setJpcre2Option(Uint x) {
		jpcre2_compile_opts = x;
		return *this;
	}
    
	/// Set PCRE2 option #compile_opts (overwrites existing option)
	/// @param x Option value
	/// @return Regex&
	/// @see RegexMatch::setPcre2Option()
	/// @see RegexReplace::setPcre2Option()
	Regex& setPcre2Option(Uint x) {
		compile_opts = x;
		return *this;
	}

	/// Parse modifier and add/remove equivalent PCRE2 and JPCRE2 options.
    Regex& changeModifier(const String&, bool);

    /// Add or remove a JPCRE2 option
    /// @param opt JPCRE2 option value
    /// @param x Add the option if it's true, remove otherwise.
    /// @return Regex&
	/// @see RegexMatch::changeJpcre2Option()
	/// @see RegexReplace::changeJpcre2Option()
    Regex& changeJpcre2Option(Uint opt, bool x) {
        if(x)
            jpcre2_compile_opts |= opt;
        else
            jpcre2_compile_opts &= ~opt;
        return *this;
    }

    /// Add or remove a PCRE2 option
    /// @param opt PCRE2 option value
    /// @param x Add the option if it's true, remove otherwise.
    /// @return Regex&
	/// @see RegexMatch::changePcre2Option()
	/// @see RegexReplace::changePcre2Option()
    Regex& changePcre2Option(Uint opt, bool x) {
        if(x)
            compile_opts |= opt;
        else
            compile_opts &= ~opt;
        return *this;
    }

    /// Parse modifier string and add equivalent PCRE2 and JPCRE2 options.
    /// This is just a wrapper of the original function Regex::changeModifier()
    /// provided for convenience.
	///
	/// **Note:** If speed of operation is very crucial, use Regex::addJpcre2Option() and
	/// Regex::addPcre2Option() with equivalent options. It will be faster that way.
	/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER as error number
    ///        if jpcre2::VALIDATE_MODIFIER or jpcre2::ERROR_ALL is set and a wrong modifier was encountered.
	/// is set and a wrong modifier was encountered.
    /// @param mod Modifier string
    /// @return Regex&
    /// @see RegexMatch::addModifier()
    /// @see RegexReplace::addModifier()
    Regex& addModifier(const String& mod){
        return changeModifier(mod, true);
    }

	/// Add option to existing JPCRE2 options #jpcre2_compile_opts
	/// @param x Option value
	/// @return Regex&
	/// @see RegexMatch::addJpcre2Option()
	/// @see RegexReplace::addJpcre2Option()
	Regex& addJpcre2Option(Uint x) {
		jpcre2_compile_opts |= x;
		return *this;
	}

	/// Add option to existing PCRE2 options #compile_opts
	/// @param x Option value
	/// @return Regex&
	/// @see RegexMatch::addPcre2Option()
	/// @see RegexReplace::addPcre2Option()
	Regex& addPcre2Option(Uint x) {
		compile_opts |= x;
		return *this;
	}

	/** Compile the regex pattern from class variable #pat_str.
	 * */
	void compile(void);

	/** @overload
	 *
	 *
	 *  Set the specified parameters, then compile the pattern using information from class variables.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 *  @param po PCRE2 option
	 *  @param jo JPCRE2 option
	 * */
	void compile(const String& re, Uint po, Uint jo) {
		init(re, po, jo);
		compile();
	}

	/** @overload
	 *
	 *
	 *  Set the specified parameters, then compile the pattern using options from class variables.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 *  @param po PCRE2 option
	 * */
	void compile(const String& re, Uint po) {
		init(re, po, 0);
		compile();
	}

	/** @overload
	 *
	 *
	 *  Set the specified parameters, then compile the pattern using options from class variables.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 *  @param mod Modifier string
	 * */
	void compile(const String& re, const String& mod) {
		init(re, mod);
		compile();
	}

	/** @overload
	 *
	 *
	 *  Set the specified parameters, then compile the pattern using options from class variables.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param re Pattern string
	 * */
	void compile(const String& re) {
		init(re, 0, 0);
		compile();
	}

	/** Perform regex match and return match count.
	 *  This function takes the parameters, then sets the parameters to RegexMatch class and calls
	 *  RegexMatch::match() which returns the result
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param s Subject string
	 *  @param mod Modifier string
	 *  @return Match count
	 *  @see RegexMatch::match()
	 * */
	SIZE_T match(const String& s, const String& mod) {
        //rm is either occupied or NULL, double deletion won't happen
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		return rm->setSubject(s).setModifier(mod).match();
	}

	/** @overload
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param s Subject string
	 *  @return Match count
	 * */
	SIZE_T match(const String& s) {
        //rm is either occupied or NULL, double deletion won't happen
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		return rm->setSubject(s).match();
	}

	/** Prepare to call RegexMatch::match().
	 * Other options can be set with the setter functions of RegexMatch class
	 * in-between the Regex::initMatch() and RegexMatch::match() call.
	 * @return RegexMatch&
	 * @see RegexMatch::match()
	 * @see RegexMatch::setSubject(const String& s)
	 * @see RegexMatch::setModifier(const String& mod)
	 * @see RegexMatch::setNumberedSubstringVector(VecNum* vec_num)
	 * @see RegexMatch::setNamedSubstringVector(VecNas* vec_nas)
	 * @see RegexMatch::setNameToNumberMapVector(VecNtN* vec_ntn)
	 * */
	RegexMatch& initMatch() {
        //rm is either occupied or NULL, double deletion won't happen
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		return *rm;
	}

	/** Perform regex replace and return the replaced string.
	 *  This function takes the parameters, then sets the parameters to RegexReplace class and calls
	 *  RegexReplace::replace() which returns the result.
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param mains Subject string
	 *  @param repl String to replace with
	 *  @param mod Modifier string
	 *  @return Resultant string after regex replace
	 *  @see RegexReplace::replace()
	 * */
	String replace(const String& mains, const String& repl, const String& mod) {
        //rr is either occupied or NULL, double deletion won't happen
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		return rr->setSubject(mains).setReplaceWith(repl).setModifier(mod).replace();
	}

	/** @overload
     *  @throw jpcre2::Except Throws exception with PCRE2 error number and error offset
	 *  @param mains Subject string
	 *  @param repl String to replace with
	 *  @return Resultant string after regex replace
	 *  @see RegexReplace::replace()
	 * */
	String replace(const String& mains, const String& repl) {
        //rr is either occupied or NULL, double deletion won't happen
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		return rr->setSubject(mains).setReplaceWith(repl).replace();
	}

	/** Prepare to call RegexReplace::replace().
	 * Other options can be set with the setter functions of RegexReplace class
	 * in-between the Regex::initReplace() and RegexReplace::replace() call.
	 * @return RegexReplace&
	 * @see RegexReplace::replace()
	 * @see RegexReplace::setSubject(const String& s)
	 * @see RegexReplace::setModifier(const String& mod)
	 * @see RegexReplace::setReplaceWith(const String& s)
	 * @see RegexReplace::setBufferSize(PCRE2_SIZE x)
	 * */
	RegexReplace& initReplace() {
        //rr is either occupied or NULL, double deletion won't happen
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		return *rr;
	}
};

}// jpcre2 namespace

#endif
