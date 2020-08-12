from Bio.Seq import Seq

#my_seq = Seq("AGCTTTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGTGGATTAAAAAAAGAGTGTCTGATAGCAGC")

with open('rosalind_ini','r') as f_open:
    my_seq = f_open.read()

string = ""

for letter in "ACGT":
    string = string + str((my_seq.count(letter))) + " "

# drop last space
string = string[:-1]

print (string)

