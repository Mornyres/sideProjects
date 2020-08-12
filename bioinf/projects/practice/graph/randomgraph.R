

makerandomgraph <- function(numvertices,numedges)
{
	# Function to make a random graph
    	require("graph")
	require("Rgraphviz")
     	# Make a vector with the names of the vertices
     	mynames <- sapply(seq(1,numvertices),toString)
     	myrandomgraph <- randomEGraph(mynames, edges = numedges)
     	return(myrandomgraph)
}

getClusters <- function(mygraph, minVertices)
{
	# Get clusters/communities from given graph
	# each cluster must have at least minVertices to be counted

}

makeGraph <- function(inputData)
{
	# Convert given structure to graph
}

library("Rgraphviz")

myrandomgraph <- makerandomgraph(15,30)
myrandomgraph
myrandomgraphplot <- layoutGraph(myrandomgraph, layoutType="neato")

png(filename="bareGraph.png")
renderGraph(myrandomgraphplot)
dev.off()

#png(filename="groupedGraph.png")
#renderGraph(myrandomgraphplot)
#dev.off()
