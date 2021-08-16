#!/bin/bash

printf "\e[31;40m REMOVING TEMPORARY AND UNWANTED FILES"
printf "To remove GNU Emacs auto saves e.g <file>.c.~<nr>~\n"
printf "add the flag -e\n\n"

printf "\e[31m"

echo bin/*  
echo src/\#*
echo src/*~*
echo vgcore*

printf "\e[0m\n\n == Removing the files =="

rm bin/*  
rm src/\#*

rm vgcore*

if [[ $1 -eq "-e"]]
then rm src/*~*
fi     

printf "\n == Finished == \n == Removing them from git's \
added files =="

git rm bin/*
git rm src/\#*
git rm vgcore*

if [[ $1 -eq "-e"]]
then git rm src/*~*
fi     

printf "\n == Finished == \n
