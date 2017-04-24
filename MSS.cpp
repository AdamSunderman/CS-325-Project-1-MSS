#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#define maxProblem 100
using namespace std;

typedef struct{
    std::vector<int> array;
    int sum;
}ResultPair;

ResultPair mssDivConq(std::vector<int> array);
ResultPair mssDivConq(std::vector<int> array, int & start, int & end);
ResultPair mssEnum(std::vector<int> array, int length);
int runAlgorithms(string fileName);
void printVector(std::vector<int> vec, int vecLength, std::ofstream & outfile);
ResultPair mssLinear(std::vector<int> array, int length);
ResultPair BetterEnum(int * array, int length);
int subarraysum (int * array, int start, int end);
ResultPair basicenum(int * array, int length);

int main(){

    string fileName;
    cout << "What file should I read?" << endl;
    cin >> fileName;
    //We can undo this later


    int results = runAlgorithms(fileName);


    if(results == 1){
        cout << "Bad file read attempt" << endl;
    }

    return 0;
}




//Function to read input file and run all algorithms and tests
int runAlgorithms(string fileName){

    //The vector with the problem
    std::vector<int> problem;
    //The vectors number of elements (not really needed but leftover and still used
    //from the switch to vector from array)
    int problemLength = 0;

    //Open the input file and verify it opened
    const char* file = fileName.c_str();
    std::ifstream input;
    input.open(file, std::ifstream::in);
    if(input.fail()){
        cout << "File does not exist, Exiting" << endl;
        return 1;
    }

    //Open the output file
    std::ofstream output;
    output.open("MSS_Results.txt");

    //Variables for file parsing
    string line;
    string delimiter = " ";
    size_t pos = 0;
    string token;
    int problemNumber = 0;

    //1. Get/Parse one problem store it in vector<int> problem.
    //2. Run all four algorithms on the vector, time and print the results.
    //3. Repeat for all lines in problem file.
    while(!input.eof()){

        problemLength=0;
        getline(input, line);

        //Find the index of the delimiter, save the number and
        //then remove the value from the line.
        while((pos = line.find(delimiter)) != std::string::npos){
            token = line.substr(0,pos);
            //cout << token << endl;
            problem.push_back(std::atoi(token.c_str()));
            problemLength++;
            line.erase(0,pos+delimiter.length());
        }

        //Due to formatting sometimes the last number won't get read by the
        //above while loop. This checks if there is still a number left in
        //the line and parses it if needed.
        if(line.length() > 0 && (isdigit(line[0]) || (line[0]=='-'))){
           pos=0;
           while(isdigit(line[pos]) || line[pos] =='-'){
               pos=pos+1;
           }
           pos=pos+1;
           token = line.substr(0, pos);
           //cout << token << endl;
           problem.push_back(std::atoi(token.c_str()));
           problemLength++;
        }
        problemNumber++;

        //Print the final vector/problem contents
        output << "Problem " << problemNumber << endl;
        printVector(problem, problemLength, output);
        output << endl;

        /**
            We should be able to just run our algorithms here. We can each
            make a run through the current problem using the array "problem[]".
            The variable problemLength has the current number of items in problem[].
            We can each make a run with our algorithm, time them, then save the results
            as we go to a file or data structure. This should be a good skeleton to work from.
        **/

        //**************************Algorithm 1 Run Area************************
  	output << "Algorithm 1 Problem " << problemNumber << endl;
        clock_t timeStart = clock();
        Resultpair alg1Results = basicenum(problem, problemLength);
        float alg1_elapsed = (float)(clock() - timeStart) / CLOCKS_PER_SEC);
        printVector(alg1Results.array, alg1Results.array.size(), output);
        output << "MSS Sum: " << alg1Results.sum << endl
               << "MSS Time: " << fixed << setprecision(10) << alg1_elapsed << endl << endl;

        //**************************Algorithm 1 Run Area************************


        //**************************Algorithm 2 Run Area************************
        output << "Algorithm 2 Problem " << problemNumber << endl;
        clock_t tStart = clock();
        Resultpair alg2Results = BetterEnum(problem, problemLength);
        float alg2_elapsed = (float)(clock() - tStart) / CLOCKS_PER_SEC);
        printVector(alg2Results.array, alg2Results.array.size(), output);
        output << "MSS Sum: " << alg2Results.sum << endl
               << "MSS Time: " << fixed << setprecision(10) << alg2_elapsed << endl << endl;
        //**************************Algorithm 2 Run Area************************


        //**************************Algorithm 3 Run Area************************

		output << "Algorithm 1 Problem " << problemNumber << endl;
		clock_t alg3_begin = clock();
		ResultPair alg3Results = mssDivConq(problem);
		clock_t alg3_end = clock();
		float alg3_elapsed = (float)(alg3_end - alg3_begin) / CLOCKS_PER_SEC;
		printVector(alg3Results.array, alg3Results.array.size(), output);
		output << "MSS Sum: " << alg3Results.sum << endl
			<< "MSS Time: " << fixed << setprecision(10) << alg3_elapsed << endl << endl;

        //**************************Algorithm 3 Run Area************************


        //**************************Algorithm 4 (Linear) Run Area************************
        output << "Algorithm 4 Problem " << problemNumber << endl;
        clock_t alg4_begin = clock();
        ResultPair alg4Results = mssLinear(problem, problemLength);
        clock_t alg4_end = clock();
        float alg4_elapsed = (float)(alg4_end - alg4_begin) / CLOCKS_PER_SEC;
        printVector(alg4Results.array, alg4Results.array.size(), output);
        output << "MSS Sum: " << alg4Results.sum << endl
               << "MSS Time: " << fixed << setprecision(10) << alg4_elapsed << endl << endl;
        cout << fixed << setprecision(10) << alg4_elapsed << endl;
        //**************************Algorithm 4 (Linear) Run Area************************


        //clear the vector
        problem.clear();
    }
    input.close();
    output.close();
    return 0;
}

//Print Vector Contents
void printVector(std::vector<int> vec, int vecLength, std::ofstream & outfile){
    for(int i=0;i<vecLength;i++){
            outfile << vec[i];
            if(i+1 == vecLength){
                outfile << endl;
            }
            else{
                outfile << ", ";
            }

     }
}

//basic enum
ResultPair basicenum(int * array, int length)
{
     int max[3] = { array[0], 0, 0 };
     int temp = 0;
     for (int i = 0; i < length; i++)
     {
	for (int j = i + 1; j < length; j++)
	{
	     temp = subarraysum(array, i, j);
	     if (max[0] < temp)
	     {
		max[0] = temp;
		max[1] = i;
		max[2] = j;
	     }
          }
     }
	int Nlength = max[2] - max[1] + 1;

	int * aresults = new int[Nlength];

	for (int i = max[1], y = 0; i < max[2] && y < Nlength; i++, y++)
	{
		aresults[y] = array[i];
	}
    ResultPair results;
    results.array = aresults;
    results.sum = max[0];
    return results;
}

//Algorithm 2 BetterEnum
ResultPair BetterEnum(int * array, int length)
{
    int max[3] = {array[0], 0, 0}
    int previous = 0;
   	for (int i = 0; i < length; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			if (j == i+1)
			{
				previous = subarraysum(array, i, j);
				if (max[0] < previous) {
					max[0] = previous;
					max[1] = i;
					max[2] = j;
				}
			}
			else
			{
				previous += array[j];
				if (max[0] < previous)
				{
					max[0] = previous;
					max[1] = i;
					max[2] = j;
				}
			}
		}
	}
    int Nlength = max[2] - max[1] + 1;

	int * aresults = new int[Nlength];

	for (int i = max[1], y = 0; i < max[2] && y < Nlength; i++, y++)
	{
		aresults[y] = array[i];
	}
    ResultPair results;
    results.array = aresults;
    results.sum = max[0];
    return results;
}

int subarraysum (int * array, int start, int end)
{
    int sum = 0;
	for (int i = start; i <= end; i++)
	{
		sum += array[i];
	}
	return sum;
}

//Algorithm 1 Enumeration
ResultPair mssEnum(std::vector<int> array, int length){

	int currMax;
	int end = 0;
	int start = 0;
	ResultPair result;
	result.sum = array[0];

	//Search the array for every possible sized sub array
	for (int subSize = 1; subSize <= length; subSize++){

		for (int i = 0; i < length; i += 1){

			currMax = 0;

			//Only calculate if the subarray is within bounds
			if (i + subSize <= length){

				//Calculate the size of the current subarray
				for (int j = 0; j < subSize; j++){

					currMax += array[i + j];
				}

				if (currMax > result.sum){

					result.sum = currMax;
					start = i;
					end = i + subSize - 1;
				}
			}
		}
	}

	//Populate result pair array
	for (int i = start; i <= end; i++)
		result.array.push_back(array[i]);

	return result;

}

//Algorithm 3 divide and conquer
ResultPair mssDivConq(std::vector<int> array){

	int low = 0;
	int high = array.size()-1;

	return mssDivConq(array, low, high);
}

ResultPair mssDivConq(std::vector<int> array, int & start, int & end){

	int interStart = 0;			//Start of intermediate subarray
	int interEnd = 0;			//End of intermeduiate subarray
	int localStart, localEnd;
	int maxSum;
	int currMax;
	ResultPair result;

	if (start == end){

		result.sum = array[start];
		result.array.push_back(array[start]);
	}
	else{

		int splitPoint = (end + start) / 2;

		//Generate the split points for the two subarray halves
		int firstStart = start;
		int firstEnd = splitPoint;
		int secondStart = splitPoint + 1;
		int secondEnd = end;

		ResultPair maxFirst = mssDivConq(array, firstStart, firstEnd);
		ResultPair maxSecond = mssDivConq(array, secondStart, secondEnd);

		interStart = firstStart;
		interEnd = firstEnd;
		localStart = firstStart;
		maxSum = array[firstStart];
		currMax = array[firstStart];

		//Check for a maximum intermediate subarray using Kadane's algorithm
		for (int i = firstStart + 1; i <= secondEnd; i++){
			localEnd = i;
			if (currMax > 0){
				currMax = currMax + array[i];
			}
			else{
				localStart = i;
				currMax = array[i];
			}

			if (currMax > maxSum){
				maxSum = currMax;
				interStart = localStart;
				interEnd = localEnd;
			}
		}

		for (int i = interStart; i <= interEnd; i++)
			result.array.push_back(array[i]);

		result.sum = maxSum;
		start = interStart;
		end = interEnd;

	}

	return result;
}


//Algorithm 4 Linear
ResultPair mssLinear(std::vector<int> array, int length){
    int maxSum = array[0];
    int currMax = array[0];
    int low = 0;
    int high = 0;
    int endLow = 0;
    int endHigh = 0;

    for(int i=1; i<length; i++){
        endHigh = i;
        if(currMax > 0){
            currMax = currMax + array[i];
        }
        else{
            endLow = i;
            currMax = array[i];
        }

        if(currMax > maxSum){
            maxSum = currMax;
            low = endLow;
            high = endHigh;
        }
    }

    std::vector<int> mssArray;
    for(int j=low; j<(high+1); j++){
        mssArray.push_back(array[j]);
    }
    ResultPair results;
    results.array = mssArray;
    results.sum = maxSum;

    return results;
}

