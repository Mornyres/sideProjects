# text file format:
#   "Quote quote quote" -- Author
# useful common regexes depending on quote source (with vim commands): 
#  g/   ^\#.*   /d      for hashtags 
#  %s/  \n--    /--     for linebreaks before author

import os, random

# this is inefficient for large source files as it reads in the whole thing. for this really basic use it's fine
def randomline(text):
    lines = text.readlines()
    line = random.choice(lines)
    return line

filename = "quotes.txt"
myFile = open(filename, 'r')


print(randomline(myFile))
