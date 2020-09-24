function [acc, prec, recall, fscore] = System_Design_scores(classMatrix)

assert (sum(size(classMatrix) == [2 2]) == 2, 'Enter a 2x2 matrix');

acc = 0;
prec = 0;
recall = 0;
fscore = 0;

classTN = classMatrix(2,2);
classFN = classMatrix(2,1);
classFP = classMatrix(1,2);
classTP = classMatrix(1,1);

acc = (classTN + classTP) / (classTN+classFN+classFP+classTP);
prec = classTP / (classTP + classFP);
recall = classTP / (classTP + classFN);

fscore = (2*prec*recall) / (prec+recall);

end