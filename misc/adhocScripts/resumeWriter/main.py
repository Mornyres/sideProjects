# options for making a new document:
#   - plaintext to pdf from scratch
#   - stitch 2 pdfs together

#from selenium.webdriver.chrome.options import Options
# we need javascript support
#from selenium import webdriver
#import selenium.webdriver.chrome.service as service

import requests
import re
import datetime
import sys
import PyPDF2 as pypdf

from time import strftime,localtime
from fpdf import FPDF
from bs4 import BeautifulSoup

# todo: make sure this works for crawlable sites
def pageParse(parenturl, logFile):
    print ("Parsing: " + parenturl)
    # from ://stackoverflow.com/questions/18408307/how-to-extract-and-download-all-images-from-a-website-using-beautifulsoup
    webpage = requests.get(parenturl)

    # extract server response as raw data

    webpage = webpage.content

    debugFile = open("rawpage.html", 'wb')
    debugFile.write(webpage)

    soup = BeautifulSoup(webpage, 'html.parser')

    debugFile = open("soup.html", 'wb')
    debugFile.write(soup.prettify("utf-8"))
    
    return debugFile


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
            logFile.write("\n")
    print ("-------------------------------------------------")
    return 0
    
def postingParse(fileName, reference, nSkills, verbose = True):
    # Find the most common entries occurring in dict from a given list
    
    # convert list to dict with 0 as default value
    entryCounts = dict.fromkeys(reference, 0)
    with open(fileName, 'r') as file:
        for line in file:
            for entry in reference:
                # TODO: special handling here for cases of C++, .NET, other with special chars
                query = "" + re.escape(entry.upper()) + ""

                entryCount = len(re.findall(query, line.upper()))

                # Debugging               
                if (entryCount != 10 and (verbose == True)):
                    print (entryCount)
                    print (query, " : ", line.upper())
                    print ("Found?", re.search(query, line.upper()))
                print ("Type:", type(entryCount))

                entryCounts[entry] = entryCounts[entry] + entryCount
               
    print ("Top ",nSkills, " Keywords | Occurrences")
    retString = []
    for elem in sorted(entryCounts.items(), key=lambda x: x[1], reverse=True)[:nSkills]:
        # make sure we only append skills that actually appear
        if elem[1] > 0:
            print (elem[0], " | ", elem[1])
            retString.append(elem[0])
        else:
            break
    return retString

# input: list of skills
# output: dict with skills and corresponding blurb to varying detail
def skillMap(skills, detailLevel):
    return None

# inputs: skill dict with name and blurb of each skill
#         target file with empty table to be populated
# output: none, file modified in-place
def dictUpdate(additions, targetFile):
    
    return None
    
def textToPDF(inputText, targetFile):
    
    pdf = FPDF()
    pdf.add_page()
    pdf.add_font('Bitter', '', 'helperFiles/fonts/Bitter-Regular.ttf', uni = True)
    pdf.add_font('Bitter', 'B', 'helperFiles/fonts/Bitter-Bold.ttf', uni = True)
    pdf.add_font('Bitter', 'I', 'helperFiles/fonts/Bitter-Italic.ttf', uni = True)
    pdf.add_font('Bitter', 'BI', 'helperFiles/fonts/Bitter-BoldItalic.ttf', uni = True)
    
    h1Color = [11,83,148]
    bodyColor = [67,67,67]
    
    h1Size = 14
    titleSize = 18
    byLineSize = 11
    bodySize = 10
    # style: B = bold, I = italic, U= underline
    pdf.set_y(-47.9) # 1.07 or .98 in... 27.178 mm or 24.892 mm
    pdf.set_x(37.6)
    pdf.set_text_color(bodyColor[0],bodyColor[1],bodyColor[2])
    pdf.set_font("Bitter", style='B', size = bodySize) 
    pdf.cell(200, 10, txt = inputText,  
         ln = 1, align = 'L')
         
    pdf.output(targetFile)
    return targetFile
    
def stitchPDF(victim, patch, outputFileName):
    with open(victim, "rb") as inFile, open(patch, "rb") as overlay:
        original = pypdf.PdfFileReader(inFile)
        background = original.getPage(0)
        foreground = pypdf.PdfFileReader(overlay).getPage(0)

        # merge the first two pages
        background.mergePage(foreground)

        # add all pages to a writer
        writer = pypdf.PdfFileWriter()
        for i in range(original.getNumPages()):
            page = original.getPage(i)
            writer.addPage(page)

        # write everything in the writer to a file
        with open(outputFileName, "wb") as outFile:
            writer.write(outFile)

# This is a placeholder from old driver code
def scratchToPDF():
    rawText = []
    with open('helperFiles/templateResume.txt', 'r') as templateFile:
        for line in templateFile:
            rawText.append(line + "\n")
            
    with open(logFileName, 'a') as logFile:
        logFile.write('-------------------Start template------------------------'+"\n")
        for line in rawText:
            logFile.write(line + " ")  

    textToPDF(rawText, pdfFileName)

    return None
####
# Driver code:
ref = {}

skillString = ""
with open('helperFiles/keywordDict.txt', 'r') as skillDB:
    for line in skillDB:
        skillString = skillString +        line
        
# split() for whitespace, strip() for commas
for skill in skillString.split(', '):
    if skill not in ref.keys():
        # hypothetically could have different entries, i.e. yrs experience, etc.
        ref[skill] = skill
###

logFileName = "logs/" + strftime("%Y-%m-%d_%H:%M:%S",localtime()) + ".txt"
pdfFileName = "logs/" + strftime("%Y-%m-%d_%H:%M:%S",localtime()) + ".pdf"

pasteInput(logFileName)
keywordList = postingParse(logFileName, ref, 5)

if keywordList == []:
    keywordList.append("N/A")
    print (keywordList)

addText = ""
for keyword in keywordList:
    addText = addText + keyword + ", "
addText = addText[:-2]
textToPDF(addText, pdfFileName)

stitchPDF('helperFiles/templateResume.pdf', pdfFileName, 'outputs/ForestLeBlanc_Resume.pdf')
