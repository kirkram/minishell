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

< ./infile.txt ls -l | wc -w  > ./no_wr_perm.txt ; echo $? >> ./results.txt ;\
 cat ./no_wr_perm.txt >> ./results.txt
echo "-------------------" >> ./results.txt

../m_shell < ./infile.txt "ls -l" "wc -w" > ./no_wr_perm.txt ; echo $? >> ./results.txt ;\
 cat ./no_wr_perm.txt >> ./results.txt
#####################
#####################
#####################
# echo "" >> ./results.txt
# echo "Test of output redirection in bash" >> ./results.txt

# echo > echo "hello world" > test1.sh
# echo "echo file content: " > ./results.txt
# cat echo >> ./results.txt
# echo "-------------------" >> ./results.txt
# echo "test1.sh file content: " > ./results.txt
# cat test1.sh >> ./results.txt

# rm echo; rm test1.sh

##expected to createe echo empty, test1 sh will have hello world in it

#####################
#####################
#####################
echo "" >> ./results.txt
echo "End of tests" >> ./results.txt

cat ./results.txt
