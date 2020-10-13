#from selenium.webdriver.chrome.options import Options
# we need javascript support
#from selenium import webdriver
#import selenium.webdriver.chrome.service as service
from bs4 import BeautifulSoup
import requests
import re
import datetime
from time import strftime,localtime

import sys
'''
def pageParse(parenturl, logFile):
    print ("Parsing: " + parenturl)
    # from ://stackoverflow.com/questions/18408307/how-to-extract-and-download-all-images-from-a-website-using-beautifulsoup
    webpage = requests.get(parenturl)

    # extract server response as raw data

    webpage = webpage.content

    debugFile = open("rawpage.html", 'wb')
    debugFile.write(webpage)

    soup = BeautifulSoup(webpage, 'html.parser')

    # format for printing to a debug file (which we wound up using a lot)
    debugFile = open("soup.html", 'wb')
    debugFile.write(soup.prettify("utf-8"))
    
    return debugFile
'''

def pasteInput(fileName):
    print("Enter/Paste your content. Ctrl-D to save it.") 
    contents = [] 
    while True: 
        try: 
            line = input() 
        except EOFError: 
            break 
        contents.append(line)
        #contents.append("\n")
        
    with open(fileName, 'w') as logFile:
        for line in contents:
            logFile.write(line)
    print ("-------------------------------------------------")
    return 0
    
def postingParse(fileName, reference, nSkills):
    # Find the most common entries occurring in FILE from a given list
    entryCounts = dict.fromkeys(reference)
    with open(fileName, 'r') as file:
        for line in file:
            for entry in reference:
                # TODO: special handling here for cases of C++, .NET, other with special chars
                query = "" + re.escape(entry.upper()) + ""
                entryCounts[entry] = 0
                # Debugging
                #print (query, "\t", line.upper())
                #print ("Found?", re.search(query, line.upper()))
                #
                entryCount = len(re.findall(query, line.upper()))
                entryCounts[entry] += entryCount
                
    print ("Top ",nSkills, " Keywords | Occurrences")
    for elem in sorted(entryCounts.items(), key=lambda x: x[1], reverse=True)[:nSkills]:
        print (elem[0], " | ", elem[1])
    return 0

# input: list of skills
# output: dict with skills and corresponding blurb to varying detail
def skillMap(skills, detailLevel):
    return 0

# inputs: skill dict with name and blurb of each skill
#         target file with empty table to be populated
# output: none, file modified in-place
def dictPaste(skillDict, targetFile)


# Driver code:
ref = ["Jenkins","Excel","powerpoint","word","java","javascript","python","agile","dsp","scrum","golang","c++","cpp","go","c#","sql","SAS","Ruby","Scala","objective","tableau","tensorflow","keras","mathematics","biology","physics","kinematics","robotics","machine learning",".net"]


logFile = "logs/" + strftime("%Y-%m-%d_%H:%M:%S",localtime()) + ".txt"
pasteInput(logFile)
postingParse(logFile, ref, 5)