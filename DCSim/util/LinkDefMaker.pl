#!/usr/bin/perl -Wall
#
# A little script to generate linkdef files for each of the 
# classes which needs a ROOT dictionary.
# SPW

$argCount = $#ARGV + 1;

if($argCount < 1){
    print "You need an argument!";
}

foreach $argNum (0 .. $#ARGV) 
{
    my $arg = $ARGV[$argNum];
    
    if($arg =~ /Dict.cxx/){
        
        $arg =~ s/Dict.cxx//;
        
        my $fname = "Include/LinkDef.h";
        
#        print "linkdefmaker args:   $arg";
        if(1){
            open(OPF, ">$fname");
            print OPF "#ifdef __CINT__";
            print OPF "#pragma link off all globals;";
            print OPF "#pragma link off all classes;";
            print OPF "#pragma link off all functions;";
            print OPF "#pragma link C++ nestedclasses;";
            print OPF "#pragma link C++ nestedtypedefs;";
            print OPF "#pragma link C++ class $arg+;";
            print OPF "#endif";
            close(OPF);
        } else {
            ## Use this for debugging.
            print "#ifdef __CINT__";
            print "#pragma link off all globals;";
            print "#pragma link off all classes;";
            print "#pragma link off all functions;";
            print "#pragma link C++ nestedclasses;";
            print "#pragma link C++ nestedtypedefs;";
            print "#pragma link C++ class $arg+;";
            print "#endif";
        }
    }
}




