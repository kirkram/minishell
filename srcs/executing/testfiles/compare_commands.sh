#!/bin/bash

rm ./results.txt
rm ./outfile.txt;
touch ./outfile.txt;

echo "" >> ./results.txt
echo "/////////////////" >> ./results.txt
echo "CommandCompare. Version 0.1 " >> ./results.txt
echo "The file m_shell should be in a folder above current" >> ./results.txt
echo "Check the results of stderr in the terminal" >> ./results.txt
#####################
#####################
#####################
echo "" >> ./results.txt
echo "Test with a mix of existing and nonexisting commands" >> ./results.txt

rm ./outfile.txt; ../m_shell ./infile.txt "lsss -l" "ls" "pipe" \
"hesshe" "afe" "eccho" "jump" "wc"  "wc -w" \
 ./outfile.txt ; echo $? >> ./results.txt ; cat ./outfile.txt >> \
 ./results.txt

echo "-------------------" >> ./results.txt

rm ./outfile.txt; < ./infile.txt lsss -l | ls | pipe | \
 hesshe | afe | eccho | jump | wc | wc -w > \
 ./outfile.txt ; echo $? >> ./results.txt ; cat ./outfile.txt >> \
 ./results.txt
#####################
#####################
#####################
echo "" >> ./results.txt
echo "Test with no_wr outfile permissions" >> ./results.txt
chmod 111 no_wr_perm.txt

< ./infile.txt ls -l | wc -w | echo "xxx" > ./no_wr_perm.txt ; echo $? >> ./results.txt ;\
 cat ./no_wr_perm.txt >> ./results.txt
echo "-------------------" >> ./results.txt

../m_shell < ./infile.txt "ls -l" "wc -w" "echo "xxx"" > ./no_wr_perm.txt ; echo $? >> ./results.txt ;\
 cat ./no_wr_perm.txt >> ./results.txt
<<<<<<< HEAD

chmod 777 no_wr_perm.txt
####################
####################
####################
echo "" >> ./results.txt
echo "Test of output redirection in bash" >> ./results.txt

echo > echo "hello world" > test1.sh
echo "echo file content: " >> ./results.txt
cat -e echo >> ./results.txt
echo "" >> ./results.txt
echo "test1.sh file content: " >> ./results.txt
cat -e test1.sh >> ./results.txt
echo "" >> ./results.txt
echo "-------------------" >> ./results.txt
echo "REDIRECTION NOT YET IMPLEMENTED" >> ./results.txt

rm echo; rm test1.sh
#expected to createe echo empty, test1 sh will have hello world in it


####################
####################
####################
echo "" >> ./results.txt
echo "Test of export behaviour" >> ./results.txt

export A="echo hi"
export B="-n hi"
echo $A >> ./results.txt; echo "" >> ./results.txt
echo $B >> ./results.txt; echo "" >> ./results.txt
echo $B"$A" >> ./results.txt ; echo "" >> ./results.txt
echo $B"$A"'$A' >> ./results.txt ; echo "" >> ./results.txt
echo $B"$A"'$A' >> ./results.txt ; echo "" >> ./results.txt
export A="hellow" | echo $A >> results.txt ; echo "" >> ./results.txt
echo "-------------------" >> ./results.txt
echo "EXPORT NOT YET IMPLEMENTED" >> ./results.txt
####################
####################
####################
=======
#####################
#####################
#####################
>>>>>>> executing
echo "" >> ./results.txt
echo "End of tests" >> ./results.txt

cat ./results.txt

#########!!!!!
# Test ideas 
# input file is not a file but a directory that exists <./testfiles/ ls -l
# should just ignore the redirection of input?
#
#
# 2:  bash echo <./testfiles "hello world!!"
# it outputs something like bash echo <./testfiles "hello worldbash " ""
# but with three exclamation marks bash echo <./testfiles "hello world!!!"   
# it waits for a dquote