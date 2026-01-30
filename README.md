# vchmod

# Intro

v - visual
<br>
vchmod - visual chmod

No need to remember 700, 777, 000, 600, 644 anymore...

```
[user-c@**** vchmod]$ ./vchmod test.txt

q - Quit; Enter - Save; Arrows - Move/Switch; a - Allowed

     User      Group     Other
     R W X     R W X     R W X
     - - -     - - -     - - -     |     Oct:   0

Final executing command:

cd /home/user-c/vchmod/vchmod && chmod 0 test.txt
Press q to cancle. Press anything else to execute

[user-c@**** vchmod]$ cat test.txt
cat: test.txt: Permission denied
[user-c@**** vchmod]$ ./vchmod test.txt

q - Quit; Enter - Save; Arrows - Move/Switch; a - Allowed

     User      Group     Other
     R W X     R W X     R W X
     a a -     - - -     - - -     |     Oct: 600

Final executing command:

cd /home/user-c/vchmod/vchmod && chmod 600 test.txt
Press q to cancle. Press anything else to execute

[user-c@**** vchmod]$ echo "Hello, World!" > test.txt
[user-c@**** vchmod]$ cat test.txt
Hello, World!
[user-c@**** vchmod]$
```


# Quick start

* Clone
* `gcc -o vchmod vchmod *.c`
