#!/bin/bash
#
# [Erik Johan Karlsson]  [GPLv2] 
#
# > Add all files commonly changed with git, that is,
# > associated with the assignment. This script is only
# > made for use when you are legally lazy. You should
# > add files manually to avoid large confusing commits.



help(){
    echo " 
Usage: ./git_add_src.sh [OPTION headers|files|others]

  OPTIONS:
     [-d | --disable] 
        - Disable the adding of one of these categories
 
     [-p | --print]   
        - Print the probided category

     [-h | --help]
        - Display this message

  CATEGORIES: 
     headers  - Header source files
     files    - Source files
     others   - Files not part of the program\n"
}


DIR=./src/

# Source code files
FILES="webstore.c   cart.c ui.c    utils.c merch.c \
      test_merch.c main.c debug.c backend.c"

# Source code header files
HEADERS="webstore.h   cart.h ui.h    utils.h merch.h \
        test_merch.h  ansi.h"

# Other files that is wanted to be tracked by git
OTHERS="README.md Makefile cleanup.sh empty_io.sh \
       git_add_src.sh cat.c doc.org TODO.org"


case $1 in
    --help|-h) help && exit
	       ;;
    --disable|-d) case $2 in
		      headers) echo "Excluding: ${HEADERS}";
			       HEADERS=""
			       ;;
		      files)   echo "Excluding: ${FILES}";
			       FILES=""
			       ;;
		      others)  echo "Excluding: ${OTHERS}";
			       OTHERS=""
		  esac
		  ;;
    --print|-p) echo
		"
Headers: ${HEADERS} 
Files:   ${FILES} 
Others:  ${OTHERS}"
		exit
esac


printf "\nCurrent modified non-added files:\n"
git status | grep modified
echo "Adding files!"
cd $DIR
for file in ${FILES};
do git add "${file}";
   printf "Added \e[32m ${file} \e[0m\n"
done   

for header in ${HEADERS};
do git add "${header}";
   printf "Added \e[32m ${header} \e[0m\n"
done   
cd ..

for other in ${OTHERS};
do git add ./$other;
   printf "Added \e[32m ${other} \e[0m\n"
done   

printf "\n Files remainging non-added and modified:\n"
git status | grep modified
