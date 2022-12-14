function data = importfile(filename, dataLines)
%IMPORTFILE Import data from a text file
%  UNTITLED = IMPORTFILE(FILENAME) reads data from text file FILENAME
%  for the default selection.  Returns the data as a table.
%
%  UNTITLED = IMPORTFILE(FILE, DATALINES) reads data for the specified
%  row interval(s) of text file FILENAME. Specify DATALINES as a
%  positive scalar integer or a N-by-2 array of positive scalar integers
%  for dis-contiguous row intervals.
%
%  Example:
%  Untitled = importfile("C:\Users\Louis\Desktop\Messung 18.11.22\Probe_1_241022_Oliver_12_rechts\01.txt", [2, Inf]);
%
%  See also READTABLE.
%
% Auto-generated by MATLAB on 14-Dec-2022 17:08:22

%% Input handling

% If dataLines is not specified, define defaults
if nargin < 2
    dataLines = [2, Inf];
end

%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 3);

% Specify range and delimiter
opts.DataLines = dataLines;
opts.Delimiter = "\t";

% Specify column names and types
opts.VariableNames = ["CoilA", "IA", "UV"];
opts.VariableTypes = ["double", "double", "double"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, ["CoilA", "IA", "UV"], "DecimalSeparator", ",");
opts = setvaropts(opts, ["CoilA", "IA", "UV"], "ThousandsSeparator", ".");

% Import the data
data = readtable(filename, opts);

end