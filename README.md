# Tetris

A *simple* C++ string calculator for big numbers. It uses minimal amount of libraries, hence works really fast and doesn't require any additional measures for deployment. In order for this calculator to function properly, I wrote my own class BigInt, containing required methods and operations, realizing simple Object Oriented Programming principles.

***

## Deployment

Simply use the Git Bash console:

    $ git clone https://github.com/Danilx8/Tetris.git
    
## Options

First enter the number. Initially, the number is set to 0.

    enter value:
    
Next, you can do some simple mathematical operations, listed in the following line, on it:
    
    Enter operator (+ - * / < > = (! to quit)):
   
Most operators are obvious and return results of those. However, the comparison operators return either true or false, depending on the result of comparison

**Example of the code:**

    enter value: 200000000000000000000000000000000000000
    --> 200000000000000000000000000000000000000
    Enter operator (+ - * / < > = (! to quit)): /
    enter value: 9
    --> 22222222222222222222222222222222222222
    Enter operator (+ - * / < > = (! to quit)): * 12000000000000000000000000
    enter value: --> 266666666666666666666666666666666666664000000000000000000000000
    Enter operator (+ - * / < > = (! to quit)): <
    enter value: 2
    false
    --> 266666666666666666666666666666666666664000000000000000000000000
    Enter operator (+ - * / < > = (! to quit)): 1
    enter value:
    e
    error with operator
    --> 266666666666666666666666666666666666664000000000000000000000000
    Enter operator (+ - * / < > = (! to quit)): !
    266666666666666666666666666666666666664000000000000000000000000
    
As you can see, the program is fully working with numbers with many digits, even with such difficult operations as division. Also, the consolee program is robust, i.e. wrong input won't crush the program and just trigger the catch function.
