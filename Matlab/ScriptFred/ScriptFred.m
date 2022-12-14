% import the data
data = importfile("C:\Users\filepath\01.txt", [1, Inf]);

% initialize the counters
counter = 1;
coilA = data.CoilA(2);

% loop over the table and count the values of CoilA
for i = 2:height(data)

    % check if a new value is found
    if data.CoilA(i) ~= coilA
        
        % update the counter 
        counter = counter + 1;

        % replace the old value with the new value
        coilA = data.CoilA(i);
    end
end

% generate an array containg start and end index for a given value of CoilA
indices = zeros(counter,2);

% initialize the first index at i = 2
indices(1,1) = 2;

% reset the counters
counter = 1; 
coilA = data.CoilA(2);

% loop over the data again and get the start and end indices
for i = 2:height(data)
    
    % check for different value
    if data.CoilA(i) ~= coilA
        
        % update the counter and check value
        coilA = data.CoilA(i);
        counter = counter + 1;
        
        % store the indices
        indices(counter, 1) = i;
        indices(counter - 1, 2) = i - 1;
    end
end

% define fitting model
slopeFit = fittype('R*x');

% prepare an array for storing the values of the slopes
Rvals = zeros(counter,1);

% plot the data for the first 5 values of CoilA
for i = 1:5

    % create a new figure window
    figure(i) 
    
    % fit the data with a slope fit
    f = fit(data.IA(indices(i, 1):indices(i, 2)), data.UV(indices(i, 1):indices(i, 2)),slopeFit);
    
    % store the slope values
    Rvals(i) = coeffvalues(f);

    % plot the data and the fit together
    plot(f,data.IA(indices(i, 1):indices(i, 2)), data.UV(indices(i, 1):indices(i, 2)))
end

% send output to console
disp('The values for R are:')
disp(Rvals(1:counter))

