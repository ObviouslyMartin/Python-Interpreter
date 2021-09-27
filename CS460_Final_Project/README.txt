CS460 - Programming Languages
Created By: Denver Gregory, Martin Plut, Ryan Romero -- May 11, 2019
Professor: Ali Kooshesh

HOW TO RUN:
    This was created and tested using CLION.
    We used input2.txt and inputExpressions.txt to test our interpreter.

What works: Everything in the provided Test files
What does not work:
    -Array Subscripts: print a[5] fails.
    -Assigning a value to an array entry like this: a[2] = 5
    -Assigning a function call to variable like => a = main()
    -return statements.
        - We tried to capture the return statement in the function that handles function parsing. We wanted to read the
         function header in, then bring in all of the statements in that function, then from that array of statements we
         wanted to grab the return statement so we then have access it's test(). The challenge is getting the return
         statement out of the list of statements. We wanted to grab the return statement and set its value to the function
         name in the global SymTab.
            For Instance, if we had a function def main(): <statements> return 5. we wanted to read in def main():
            then we wanted to read all of the statements and add them to a vector. In that vector, we want to then
            somehow grab the return statement, grab it's test, evaluate it and set its value to the function name "main"
            in the global SymTab.

Slip days: We are submitting at this time as we planned to use Martin's remaining 2 slip days.
