import requests
from bs4 import BeautifulSoup as bs
import sys
import os
import webbrowser
import re

# open the tabs for matched jobs, up to n
def newTabs(matches, n=10):
    # determine list
        # ..
    # iterate over list
    
    openTab()
    maxLen = min(n,len(matches))
    
    for i in range(maxLen):
        openTab(matches[i])
    return
    
    
# apply filter based on job search platform
# (may or may not be applicable)
def resultFilter(platform, query):
    # call pageLoop
    return
    
    
# apply additional ctrl+F style search given query
# parse with bs and return corresponding hyperlinks
def searchPage(page, matches, query):
    # for each match in page append to matches
    divs = page.findAll('div', {'class': 'jobsearch-SerpJobCard unifiedRow row result'})
    
    with open("logs/parsedJobs.html", 'w') as debugFile:
        for div in divs:
            job = div.findChildren()
            # apply some search term and if it matches...
            if (query in (str(job))):
                url =  'https://www.indeed.com/viewjob?jk=' + div.attrs['data-jk']
                debugFile.write(str(url))
                matches.append(url)
                    
    return matches

# given string url, parse page and write data to log file(s) 
def pageParse(url):
    print ("DEBUG: Parsing {}\n".format(url))

    webpage = requests.get(url)

    # extract server response as raw data

    webpage = webpage.content

    with open("logs/rawpage.html", 'wb') as debugFile:
        debugFile.write(webpage)

    soup = bs(webpage, 'html.parser')

    with open("logs/soup.html", 'wb') as debugFile:
        debugFile.write(soup.prettify("utf-8"))
    
    return soup
    
# loop over n pages and append to matches
def pageLoop(parentUrl, query, n=5):
    
    matches=[]
    for i in range(n):
        # modify parentUrl and query next page data
        nextUrl = pageUrl(parentUrl, i, site='Indeed')
        pageData = pageParse(nextUrl) 
        matches = searchPage(pageData, matches, query)
    return matches
    
def pageUrl(parentUrl, n, site='Indeed'):
    # parse url for page number, return modified url
    if site=='Indeed':
        re_pagesize= re.match(r"(.*limit=)([0-9]+)(.*)",parentUrl)
        pagesize = int(re_pagesize.group(2))
        # start=0 corresponds to page 0, start=50 corresponds to page 1 with pagesize 50, etc.
        n_offset = n * pagesize
        nextUrl = re_pagesize.group(1) + re_pagesize.group(2) + '&start=' + str(n_offset) + re_pagesize.group(3)
    else:
        # todo: try to intuit next page from beautifulsoup query
        nextUrl = None
    return nextUrl
    
def openTab(url='', method='OS'):
    # todo: generalize this further, allow for more modular input
    if method=='OS':
        browser_cmd = '\'/cygdrive/c/Program Files (x86)/Google/Chrome/Application/new_chrome.exe\''
        os.system(browser_cmd + ' ' + url)
        return 0
    else:
        return 1
 
# pseudocode 
    # call pageLoop
        # optionally call resultFilter
        # call searchPage

    # use output:  
    # call newTabs
    
try:
    parentUrl = sys.argv[1]
except:
    parentUrl = 'https://www.indeed.com/jobs?q=engineer%20-combat%20-missile%20-weapon%20-weapons%20-threat%20-threats&jt=fulltime&explvl=entry_level&limit=50&sr=directhire&st=employer&fromage=3&vjk=ad4b15ef2c5185c4'

try:
    myQuery = sys.argv[2]
except:
    myQuery = 'Urgently'

# checkpoint 1: return & view raw page data for page n -- DONE

# checkpoint 2: return all jobs found in page -- DONE
# checkpoint 3: return all following pages for 0..n -- DONE
matches = pageLoop(parentUrl, 'Urgently', 2)
# checkpoint 4: open all the pages
newTabs(matches)