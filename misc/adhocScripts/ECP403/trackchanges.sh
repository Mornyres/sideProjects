# note: in this example the new report is unzipped to a directory finalreport_NEW and the old one is unzipped to finalreport_OLD

latexdiff-so finalreport_OLD/Sections/1_abstract.tex finalreport_NEW/Sections/1_abstract.tex > output/abstract.tex

latexdiff-so finalreport_OLD/Sections/2_intro.tex finalreport_NEW/Sections/2_intro.tex > output/intro.tex

latexdiff-so finalreport_OLD/Sections/3_breakdown.tex finalreport_NEW/Sections/3_breakdown.tex > output/breakdown.tex

latexdiff-so finalreport_OLD/Sections/4_details.tex finalreport_NEW/Sections/4_details.tex > output/details.tex

latexdiff-so finalreport_OLD/Sections/5_results.tex finalreport_NEW/Sections/5_results.tex > output/results.tex

latexdiff-so finalreport_OLD/Sections/6_conclusion.tex finalreport_NEW/Sections/6_conclusion.tex > output/conclusion.tex
