# Brief
## syslog_example
This program is for demonstartion that how to use syslog. by default, 
the generated syslog would be placed at the path like <code>/var/log/syslog</code>.
You could use <code>cat</code>command to check out the output.

The output might look like this:

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
./syslog_example
```
After the execution, you might want to checkout the ouput be placed at <code>/var/log/syslog</code>
(in Ubuntu).


