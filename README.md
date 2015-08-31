# cissy
command-line csv processing and formatting utility/tool

# NRFPT
* not ready for prime time.  

# TODO
* apt build

# MAN PAGE
    NAME
           cissy - csv delimited text processing utility/tool
    
    SYNOPSIS
           cissy [OPTIONS]
    
    DESCRIPTION
           Cissy  is  a  stream (file or pipeline) processor used to perform basic
           transformations on delimited text files.  It's primary goal is to  eas‐
           ily  process  delimited files for reformating and sorting (see EXAMPLES
           below).
    
    
       Input/Output
    
    
           -i <input filename>
    
                  reads from stdin, unless input file is specified
    
           -o <output filename>
    
                  reads from stdout, unless output file is specified
    
    
    EXAMPLES
       Selective Output
    
    
           cissy -c 3
    
                  output only the 3rd column
    
    
           cissy -c 3,1,2,4- cissy -c 3,1-2,4-
    
                  move the 3rd column to the front
    
    
       Sorting
    
    
           cissy -c 2,1- | sort | cissy -c 2-
    
                  sort by the 2nd column using sort(1).
    
    
    
    BUGS
           Report bugs to the Github bugtracker.
    
    
    AUTHOR
           Written by Steven Lass.
    
    COPYRIGHT
           Apache 2.0 License
    
    SEE ALSO
           awk(1), sed(1), sort(1), tr(1)
           
           

# Open Build Service Binaries

SRPM

[cissy-0.2alpha-1.1.src.rpm](https://api.opensuse.org/build/home:slass100/CentOS_6/i586/cissy/cissy-0.2alpha-1.1.src.rpm)

Centos 6 

[cissy-0.2alpha-1.1.i686.rpm](https://api.opensuse.org/build/home:slass100/CentOS_6/i586/cissy/cissy-0.2alpha-1.1.i686.rpm)

[cissy-0.2alpha-1.1.x86_64.rpm](http://download.opensuse.org/repositories/home:/slass100/CentOS_7/x86_64/cissy-0.2alpha-1.1.x86_64.rpm)
