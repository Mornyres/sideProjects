from Bio import ExPASy
from Bio import SwissProt
from urllib.request import urlopen
import requests


# TODO: figure out what's going on with uniprot ssl3 handshake verification -- i can't even use wget. 
result = requests.get("https://www.uniprot.org/uniprot/B5ZC00.txt",verify=False)

if result.ok:
    print(result.text)
else:
    print("URL not accessible", result.status_code)

handle = result
record = SwissProt.read(handle)
dir(record)

