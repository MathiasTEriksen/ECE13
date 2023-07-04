----------------------------------------------------------------------

			Mathias Eriksen
			mterikse@ucsc.edu
			Lab 09 : BattleBoats
			December 2 2022
            
----------------------------------------------------------------------

MY REPO CONTAINS THE FINAL VERSION OF THE PROJECT

I WROTE:

Field.c
Message.c
AgentTest.c
NegotiationTest.c

__Summary:__

This lab was a culmiation of all the labs so far, and many different concepts were covered within it. I wrote Field and Message, so most of my work was centered around using arrays and string operaters, as well as being competent in struct usage.

__Methods:__
 
This lab was quite difficul, because it was hard for me to find time to work with my partner in the weeks leading up to the due date. The result was that it was quite difficult to get our project working together properly, since we could not work together most of the time. 

In my lab, I had more trouble with writing message, since I was not that familiar with how strings work in C. I had sometrouble using strtok properly, and making sure messages were getting parsed and broken up correctly. Most of my debugging in this lab was done using the test files, and incorporating print statements within my code to visually check that my output was what I expected. The main difficulty I had was that I had some trouble with my message decode function, where the checksum it was getting from decoding the characters was not the value I was expecting. I was unable to resolve this issue. Fortunately, my Field.c file worked quite well, and I enjoyed putting it together. I also found that having the partner write the otehr partners test file was quite annoying, since I had to write my own test files anyway, and I have a much better understanding of how to write a test file for code that I wrote myself. It was quite difficult to dig through my partners code and write a meaningful test file for it when they were ready.

We also encountered a problem at the last day of the lab, when we were trying to run the lab on the microcontroller. The agent file my partner wrote was not properly updating the OLED, so we couldn't really test how everything really worked together properly. 

If I were to do this lab again, I would allocate a lot more of my time to work on it, since I kind of ran out of time towards the end of the lab and had a lot to do in a short time frame towards the end. I think my main mistake was not working on the project over thanksgiving break, and leaving too much to do in all of my classes this past week.

__Results:__

The result for this lab was not quite what we hoped for. We would have hoped to get a fully working final project, but out limited time working together probably hindered that. Either way, I think the functions I wrote work quite well, and it was rewarding to get the field file working correctly.

__Feedback:__
 
I believe that the lab doc for this lab could have been done a bit better, with some more in depth discussion of how to approach each file on its own. All we had here was a paragraph outlining the general usage of the files, and not a step by step guide like we had in the other labs. 