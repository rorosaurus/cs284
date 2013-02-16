#!/usr/bin/env bash

ancestry(){
current_pid=$$
ps -ef > temp
echo $current_pid
while [ $current_pid != 1 ]
do
	echo "  |"
	current_pid=`awk '{if($2=="'"$current_pid"'")print;}' temp | awk '{print $3}'`
	echo $current_pid
done
rm temp
}

proccess(){
ps -ef > tempPS
who | awk '{print $1}' > tempWHO
echo Select a User to View their Running Processes
#Echo out all users, with unqie identifiers
index=1
while read line
do
	echo $index")" $line
	index=`expr $index + 1`
done < tempWHO

read user_selection

user="null"
while read line
do
	user_selection=`expr $user_selection - 1`
	
	if [ $user_selection -eq 0 ]
		then 
			user=$line
	fi
done < tempWHO

if [ $user != "null" ]
	then
		echo
		echo Here are the Current Processes of User \"$user\":
		awk '{if($1=="'"$user"'")print;}' tempPS
else
	echo
	echo You selected an invalid index.
fi

rm tempPS
rm tempWHO

}

#Script begins here
echo Welcome to Rory Hayes\'s Assignment 1
echo CS 284 Section 1A
echo -----------------

#continute to function until user selects 'exit'
while true
do

echo Please Enter a Menu Option:
echo 1- Process Ancestry
echo 2- See Who is Online
echo 3- View Processes by User
echo 4- Exit

read selection

case $selection in
	1)
		echo 
		echo "The Ancestry Tree for the current process is:"
		ancestry
		echo
		;;
	2)
		echo
		echo The following users are currently online:		
		#who | cut -f1 -d' '
		who | awk '{print $1}'
		echo
		;;
	3)
		echo
		proccess
		echo
		;;
	4)
		echo
		echo Exiting script..
		exit
		;;
	*)
		echo '\007'		
		echo Invalid selection.  Please try again.
		echo
		;;
esac

done
