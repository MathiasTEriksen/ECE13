
#########################
#			#
# Mathias Eriksen	#
# mterikse@ucsc.edu	#
# Lab 02		#
#			#
#########################

In this lab, I completed the entirety of the lab on my own, as it was quite simple to implement using concepts from class. A lot of issues I encountered were fixed by using google along with C libraries online. One main problem I ran into was the extra newline character being picked up by the scanf() function. I fixed this using the recommended approach of adding a space in the declaration in order to ignore whitespace. From this point on, my previous experience in coding made it quite simple to implement the required behavior using if else and else if statements. 

If scanf(" "); excecutes, the program will prompt the user for an input using a space, but if anything is input, it will not be read by the program since there is no variable in which the data should be stored. 

The no printf or scanf in the functions rule was helpful because it allows you to get the specific operands that you need before they are passed into the function. It would get complicated using the scanf and printf in the function, since the result from the scanf needs to be passed into that function. 