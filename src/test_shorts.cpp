#include <iostream>
#include "jpcre2.h"


int main(){
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
    
    return 0;
}