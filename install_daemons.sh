#!/bin/bash

password="password"

# install postgres onto ix your ix server
postgresctl install

# set database password
# username is automatically your ix username

{
	/bin/expect << EOF
	expect "*?assword:*"  
	send -- "$password\n"
	expect "Enter it again:*" 
	send -- "$password\n"
	expect eof
EOF
}

cd ../

# run postgres configuration
. postgres-data/config.sh



# if any part of this doesn't work for you let me know and/or go to this link
# on how to do it by hand: https://systems.cs.uoregon.edu/wiki/index.php?n=Help.ToolsPostgresql

