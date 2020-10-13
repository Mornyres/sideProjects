function idx = findClosestCentroids(X, centroids)
%FINDCLOSESTCENTROIDS computes the centroid memberships for every example
%   idx = FINDCLOSESTCENTROIDS (X, centroids) returns the closest centroids
%   in idx for a dataset X where each row is a single example. idx = m x 1 
%   vector of centroid assignments (i.e. each entry in range [1..K])
%

% Set K
K = size(centroids, 1);

% You need to return the following variables correctly.
idx = zeros(size(X,1), 1);

% ====================== YOUR CODE HERE ======================
% Instructions: Go over every example, find its closest centroid, and store
%               the index inside idx at the appropriate location.
%               Concretely, idx(i) should contain the index of the centroid
%               closest to example i. Hence, it should be a value in the 
%               range 1..K
%
% Note: You can use a for-loop over the examples to compute this.
%

for (example = 1:size(X,1))
    distance_min = Inf;
    min_index = 1;
    
    for (c = 1:K)

        % failed past attempts for posterity ... i forgot to
        % reference the rest of the array.
            %distance = dist(X(example), centroids(c));
        distance = norm(X(example,:) - centroids(c,:)); 
            %distance = sqrt(X(example).^2) - sqrt(centroids(c).^2); 
        distance = distance^2;
        % getting wrong answer on example 2
        %if (example<=3)
        if (0)
            fprintf("E%d | C%d | NEW<%d>| MIN<%d>\n",example,c,distance,distance_min);
            fprintf("X=");
            disp(X(example,:));
            fprintf("C=");
            disp(centroids(c,:));
            
        end
        if (distance < distance_min)
            distance_min = distance;
            min_index = c;
        end
    end
    % index of, not value
    idx(example) = min_index;
end







% =============================================================

end

