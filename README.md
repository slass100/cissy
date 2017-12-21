cissy
=====
command-line csv processing and formatting utility/tool.

AWK, sed, cut and other popular/common utilities can handle a 
majority of tasks, but cissy is specifically written to for CSV
files where the quoted text contains the field separator.

See the [man page](https://github.com/slass100/cissy/wiki/man-page) in the 
Wiki for help.


Some basic examples are shown in the man page.

Things cissy can do easily:

##### Change delimiters without changing quoted text
```bash
$ echo '1,"2,3",4,5' | cissy -do :

1:"2,3":4:5
```

##### Move/transpose columns
```bash
$ echo 'a,b,c,d,e,f' | cissy -c 4-

d,e,f
```

```bash
$ echo 'a,b,c,d,e,f' | cissy -c 1-3,5-

a,b,c,e,f
```


```bash
$ echo 'a,b,c,d,e,f' | cissy -c 4

d
```

##### Use filenames or stdin/stdout

```bash
$ cat inputfile.txt | cissy -c 4

$ cissy -i inputfile.txt -c 4
```

```bash
$ cat inputfile.txt | cissy -c 4 > outputfile.txt

$ cissy -i inputfile.txt -o outputfile.txt -c 4
```



