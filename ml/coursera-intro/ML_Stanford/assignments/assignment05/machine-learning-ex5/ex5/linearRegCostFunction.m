function [J, grad] = linearRegCostFunction(X, y, theta, lambda)
%LINEARREGCOSTFUNCTION Compute cost and gradient for regularized linear 
%regression with multiple variables
%   [J, grad] = LINEARREGCOSTFUNCTION(X, y, theta, lambda) computes the 
%   cost of using theta as the parameter for linear regression to fit the 
%   data points in X and y. Returns the cost in J and the gradient in grad

% Initialize some useful values
m = length(y); % number of training examples

% You need to return the following variables correctly 
J = 0;
grad = zeros(size(theta));

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the cost and gradient of regularized linear 
%               regression for a particular choice of theta.
%
%               You should set J to the cost and grad to the gradient.
%


h = (X*theta);

% remove first theta term for each 
%theta_reg = theta(:,2:end);
% gotcha: was removing the wrong dimension! 
theta_reg = theta(2:end,:);


% replace with 0
theta_reg = [0; theta_reg];

delta = h - y;

J = sum( delta.^2 / (2*m));
%J = ((delta * delta') / (2*m));
% apply regularization
J = J + (lambda/(2*m)) * sum(theta_reg.^2);

% the regularization term shouldn't matter and can just be added in for all
% cases since the regularized theta matrix has the zeroth theta = 0

%tic
%grad = sum(delta .* X) / m;
%grad = grad + ((lambda / m) * theta_reg(2:end));
%toc

% turns out this one is faster, not surprisingly
%tic
grad = (X'*delta+lambda*theta_reg)/m;
%toc
% =========================================================================

grad = grad(:);

end
