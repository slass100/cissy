cissy
=====
command-line csv processing and formatting utility/tool.

See the [man page](https://github.com/slass100/cissy/wiki/man-page) in the 
Wiki for help.


Some basic examples are shown in the man page.

Things cissy can do easily:

##### Change delimiters without changing quoted text
```bash
echo '1,"2,3",4,5' | cissy -do :
1:"2,3":4:5
```

##### Move/transpose columns
```bash
echo 'a,b,c,d,e,f' | cissy -c 4-
d,e,f
```

```bash
echo 'a,b,c,d,e,f' | cissy -c 1-3,5-
a,b,c,e,f
```


```bash
echo 'a,b,c,d,e,f' | cissy -c 4
d
```

##### Use filenames or stdin/stdout

```bash
cat inputfile.txt | cissy -c 4

cissy -i inputfile.txt -c 4
```

```bash
cat inputfile.txt | cissy -c 4 > outputfile.txt

cissy -i inputfile.txt -o outputfile.txt -c 4
```


Binaries/Packages
===================

Binaries for many Linux distros are available from the [Open Build Service](https://build.opensuse.org/).

[OBS Cissy](https://build.opensuse.org/package/show/home:slass100/cissy)

Here are some of the OBS built binaries


Centos 5
[SRPM](http://download.opensuse.org/repositories/home:/slass100/CentOS_5/src/cissy-0.4-16.1.src.rpm)
[i586](http://download.opensuse.org/repositories/home:/slass100/CentOS_5/i386/cissy-0.4-16.1.i386.rpm)
[x86_64](http://download.opensuse.org/repositories/home:/slass100/CentOS_5/x86_64/cissy-0.4-16.1.x86_64.rpm)


Centos 6
[SRPM](http://download.opensuse.org/repositories/home:/slass100/CentOS_6/src/cissy-0.4-16.1.src.rpm)
[i586](http://download.opensuse.org/repositories/home:/slass100/CentOS_6/i686/cissy-0.4-16.1.i686.rpm)
[x86_64](http://download.opensuse.org/repositories/home:/slass100/CentOS_6/x86_64/cissy-0.4-16.1.x86_64.rpm)


Centos 7
[SRPM](http://download.opensuse.org/repositories/home:/slass100/CentOS_7/src/cissy-0.4-16.1.src.rpm)
[broken](http://download.opensuse.org/repositories/home:/slass100/CentOS_7/i686/cissy-0.4-16.1.i686.rpm)
[x86_64](http://download.opensuse.org/repositories/home:/slass100/CentOS_7/x86_64/cissy-0.4-16.1.x86_64.rpm)


Debian 7.0 
[i386](http://download.opensuse.org/repositories/home:/slass100/Debian_7.0/i386/cissy_0.4_i386.deb)
[amd64](http://download.opensuse.org/repositories/home:/slass100/Debian_7.0/amd64/cissy_0.4_amd64.deb)


Debian 8.0 
[i386](http://download.opensuse.org/repositories/home:/slass100/Debian_8.0/i386/cissy_0.4_i386.deb)
[amd64](http://download.opensuse.org/repositories/home:/slass100/Debian_8.0/amd64/cissy_0.4_amd64.deb)

Fedora / Redhat / Ubuntu / Others

[Start Here](https://build.opensuse.org/package/show/home:slass100/cissy)
Click on the distro, then click on the package.

