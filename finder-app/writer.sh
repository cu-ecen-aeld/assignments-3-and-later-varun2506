#!/bin/bash

#AESD Assignment 1
#File Name: writer.sh
#Author: Varun Milind Mehta
#References: https://www.halolinux.us/commands-for-ubuntu/searching-for-text-with-grep.html
#            https://stackoverflow.com/questions/25470652/linux-bash-redirect-a-string-to-a-file

#Store arguments in local variables
writefile=$1
writestr=$2

#check if the number of arguments are equal to two or not
if [ $# -ne 2 ] 
then
	echo " Invalid arguments"
	echo " Argument 1 <File path>"
	echo " Argument 2 String to be added "
	exit 1
fi

#get name of directory from the filepath
Directory="${writefile%/*}"

#Check if directory exist in the file system
#Create new directory is not found 
if [ ! -d "$Directory" ]  
then
	mkdir -p $Directory
	echo "Directory not found. New directory created $Directory"
fi

#Create the file in the directory or overwrite the file with updated timestamp
touch $writefile

#Check for valid file
if [ -f "$writefile" ] 
then
	#overwrite the string in the file
	echo "$writestr" >  "$writefile"
else
	echo "File not created"
	exit 1
fi

#end
