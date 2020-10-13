function [C, sigma] = dataset3Params(X, y, Xval, yval)
%DATASET3PARAMS returns your choice of C and sigma for Part 3 of the exercise
%where you select the optimal (C, sigma) learning parameters to use for SVM
%with RBF kernel
%   [C, sigma] = DATASET3PARAMS(X, y, Xval, yval) returns your choice of C and 
%   sigma. You should complete this function to return the optimal C and 
%   sigma based on a cross-validation set.
%

% You need to return the following variables correctly.
C = 1;
sigma = 0.3;

% ====================== YOUR CODE HERE ======================
% Instructions: Fill in this function to return the optimal C and sigma
%               learning parameters found using the cross validation set.
%               You can use svmPredict to predict the labels on the cross
%               validation set. For example, 
%                   predictions = svmPredict(model, Xval);
%               will return the predictions on the cross validation set.
%
%  Note: You can compute the prediction error using 
%        mean(double(predictions ~= yval))
%

Cvals = [0.01 0.03 0.1 0.3 1 3 10 30];
sigmavals = [0.01 0.03 0.1 0.3 1 3 10 30];

smallest_error = Inf;

for i=1:length(Cvals)
    for j=1:length(sigmavals)
        current_sigma = sigmavals(j);
        current_C = Cvals(i);
        % try each pair
        %function [model] = svmTrain(X, Y, C, kernelFunction, ...
        %                    tol, max_passes)
        
        % where do x1,x2 come in?
            % https://www.coursera.org/learn/machine-learning/programming/e4hZk/support-vector-machines/discussions/threads/lxbgh2_FEemrEArKyDxDRA
            % https://www.coursera.org/learn/machine-learning/programming/e4hZk/support-vector-machines/discussions/threads/Xwn4BixSEembew4tDjUTcg
        model = svmTrain(X, y, current_C, @(x1, x2) gaussianKernel(x1, x2, current_sigma));
        predictions = svmPredict(model, Xval);
        error = mean(double(predictions ~= yval));
        if (error < smallest_error)
            smallest_error = error;
            C = current_C;
            sigma = current_sigma;
        end
        
    end
    
end

% =========================================================================

end
