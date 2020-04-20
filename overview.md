# Introduction
This repository contains the submission for my 4th year Bs.c Computing in Software Development module in GMIT, <b>Theory of Algorithms</b>. This module provides an introduction to the theory of algorithms, especially the areas of computation and computability. Students will also learn practical skills in the development of software using a selection of programming paradigms.

<br>

The following are the defined learning outcomes on completion of this module:

1. Compare different models of computation.

2. Write computer programs using a selection of programming paradigms.

3. Analyse the complexity of an algorithm.

4. Identify difficult computational problems in everyday programming.

## About the Repository
The contents of this repository was developed from the beginning of January to the end of April 2020. The main files that relate to the project specification for this module can be found in the <i>theory-of-algorithms-project</i> folder. The other folders contain code that was developed over the course of the module through a series of video/lab lectures that had been prepared for us by our lecturer for this module. The code that is contained in this repository was mainly developed using the <i>vi text editor</i> on a <i>Debian virtual machine</i> that was hosted by <i>Google cloud platform</i>. Any work done on my own machine was developed using <i>visual studio code</i> in conjunction with the <i>Windows sub-system for Linux</i> that used <i>Ubuntu 18.04</i>. 

<br>

The following is a brief description of the other folders in this repository that do not relate to the main project for this module:

* <b>hello-programs:</b>

The hello-programs folder contains two hello world programs, one written in C and the other in python. This was done as part of an introduction to this module where our lecturer took us through the steps to set up a virtual machine using the Google cloud platform. The virtual machine ran using a Debian OS image and after installing the necessary libraries needed to execute C and python programs these files were developed. 

* <b>c-bit-operations:</b>

The c-bit-operations folder contains C program files that were developed as part of the video lectures series. The purpose of these lectures was to further our understanding of how data is represented and manipulated the bit level using C. These videos were of great help in the lead up to the development of the main project for this module.

* <b>sha256:</b>

The sha256 folder contains code written in C that was developed over the last few videos in the series as a side project where we developed the sha256 hashing algorithm. In the README.md for this repository a link is provided to the main source used in the development of this algorithm along with some example testing output presented to show the reader the contents of a file after it has been processed by the program and comparing it with the output of openssl's version of the sha256 algorithm.  


## The Main Project 

The main project for the Theory of Algorithms module was to write a program in C to perform the MD5 message-digest algorithm. The main source used to develop this algorithm is specified in the [Request For Comments 1321 document supplied by the Internet Engineering Task Force.](https://tools.ietf.org/html/rfc1321)

The code developed to implement the MD5 message-digest algorithm for this project can be found in the <i>theory-of-algorithms-project</i> folder. The description of the algorithm and steps that are involved to successfully implement the MD5 message-digest algorithm are explained in the README.md file of this repository. The following will be a breakdown of the files contained within the project folder:

* <b>md5.h:</b>

The md5.h file of this folder is a header file that contains all the constants, pre-processor macro functions, global variables and function definitions for the md5.c file. Header files are commonly used when writing C programs which is evident when including files such as <b>stdio.h</b> or <b>stdlib.h</b>. In doing this you can leave the file in which your main function resides a lot more neat and tidy, another advantage in doing this is that by declaring global variables in this file your main program can use them throughout the program whereas otherwise these variables would have to be first defined in your main function.

* <b>md5.c:</b>

The md5.c file contains the logic and function implementations that went into successfully writing the MD5 message-digest algorithm. The program takes command line arguments on execution of the program and depending on the argument performs a different operation which will be explained in the next section. On execution of the program the user will have the option to either perform the MD5 algorithm on either a string input or a file path that is entered from a command line prompt. The md5.c file along with the functions written to perform the MD5 algorithm also contains testcase functions that were developed using the [cmocka](https://api.cmocka.org/) unit testing framework for C. This framework will be detailed further in the following sections.

* <b>empty:</b>

This an empty file that was created when testing the algorithm. When performing the MD5 operation on an input the empty string or file in this case is still considered a valid input and can be used as a good benchmark to assert your algorithm is correct.

* <b>inputText.txt:</b>

This file is used as part of the program when the user chooses to input a string. The program takes the user input writes it to this file and then proceeds to read the file contents as binary data and then perform the MD5 algorithm to output the correct hash value for a given input. 


# Run


# Test


# Algorithm


# Complexity


# References

