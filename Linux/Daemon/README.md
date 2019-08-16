# Brief
## simple_deamon
This program shows how to create a simple deamon program. Notice that if you run this deamon program,
you must kill it manually or it will keep doing stuffs in your background. By the way, this deamon will generate some syslog to the place like <code>/var/log/syslog</code>. To kill the deamon, please using 
<code>ps</code>with<code>grep</code> command to find out the <code>pid</code> of the deamon, and kill it by <code>kill `<pid>`</code> command directly or <code>kill -s SIGINT `<pid>`</code>

### Build
#### Enviroment
Ubuntu is recommanded. Other Linux platform should work ,too.
#### Steps
1. <code>cd</code> to current directory
2. Using makefile command:
```
make
```
### Run
Execute the syslog_example directly by command:
```
./simple_deamon
```
After the execution, you might want to checkout the ouput be placed at <code>/var/log/syslog</code>
(in Ubuntu).
### Termination
1. Using command <code>ps</code> with <code>grep</code> to find out the pid of the daemon:
```
ps -ed | grep simple_deamon
```
2. And you might see someting as below:
```
19547 ?         00:00:00 simple_daemon
```
3. Bingo! 19547 is the <code>pid</code> of the daemon.
4. Using <code>kill</code> command to terminate this deamon
```
kill 19547
```
or
```
kill -s SIGINT 19547
```
5. If you signal the daemon with SIGINT, it should write some syslog to the place <code>/var/log/syslog</code>. And you might see something like this:
```
Aug 16 10:52:43 jack-K55VM simple deamon[19547]: Receiving SIGINT, start exting the deamon...
```

