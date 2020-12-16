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

# https://www.poetryfoundation.org/poems/browse#page=1&sort_by=recently_added&forms=279

def pageParse(parenturl, debugMode="Verbose"):
    print ("Parsing: " + parenturl),
    # from ://stackoverflow.com/questions/18408307/how-to-extract-and-download-all-images-from-a-website-using-beautifulsoup
    webpage = requests.get(parenturl)
    
    # extract server response as raw data
    webpage = webpage.content
    if (debugMode == "Verbose"):
        with open("logs/rawpage.html", 'wb') as logFile:
            logFile.write(webpage)
    
    soup = BeautifulSoup(webpage, 'html.parser')
    
    # make human-readable
    if (debugMode == "Verbose"):
        with open("logs/souped.html", 'wb') as logFile:
            logFile.write(soup.prettify("utf-8"))
            
    #<div class="o-poem" data-view="PoemView">
    poemBody = soup.find("div", {"class": "o-poem"})
    
    if (debugMode == "Verbose"):
        with open("logs/poemBody.txt", 'w') as logFile:
            logFile.write(poemBody.text)
            
    with open("scrapedPoems/poems.txt", 'a') as logFile:
        logFile.write(poemBody.text)
    
    print ("Done!")
    
    return 0
    
if len(sys.argv) == 2:
    pageParse(sys.argv[1])