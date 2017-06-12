#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class NaiveBayes
{
private:
	fstream trainingfile;
	fstream testingfile;
	int numberFeatures;
	int numberClasses;
	int N;
	map<string, map<pair<string, string>, float> > pTable;
	map<string, float> pClass;	
	vector<string> featureLabels;
	vector<vector<string>> trainingSet;

public:
	NaiveBayes()
	{
		this->N=0;
		this->numberClasses = 0;
		this->numberFeatures = 0;
	}

	void loadTraining(string filename)
	{
		fstream file;
		file.open(filename.c_str());
		string line;
		string word;

		getline( file, line );
		stringstream ss0;
		ss0 << line;
		while(ss0 >> word)
		{
			featureLabels.push_back(word);
			pTable[word];
			++numberFeatures;
		}

		while(getline( file, line ))
		{			 
			stringstream ss;
			ss << line;
			vector<string> temp;
			for(int i=0 ; i<=numberFeatures ; ++i)
			{
				ss >> word;
				temp.push_back(word);
				if(i==numberFeatures)
				{
					if(pClass.find(word) != pClass.end())
						pClass[word] += 1;
					else
						pClass[word] = 1;
				}
			}
			this->trainingSet.push_back(temp);
			++this->N;
		}		

		for(auto & row : trainingSet)
		{
			string c = row[numberFeatures];
			for(int i=0 ; i<row.size()-1 ; ++i)
			{
				if( pTable[featureLabels[i]].find(make_pair(row[i],c)) != pTable[featureLabels[i]].end() )
					pTable[featureLabels[i]][make_pair(row[i],c)] += 1;
				else
					pTable[featureLabels[i]][make_pair(row[i],c)] = 1;
			}
		}

		for(auto & f : pTable)
		{
			cout << f.first << endl;

			for(auto & c : pClass)
			{
				for(auto & l : f.second)
				{					
					if(f.second.find(make_pair(l.first.first,c.first)) == f.second.end())
						f.second[make_pair(l.first.first,c.first)] = .0;
				}
			}
		
			string tempLabel = f.second.begin()->first.first;
			float total = .0;
			for(auto & l : f.second)
			{
				if(l.first.first == tempLabel)
				{
					total += l.second;
				}
				else
				{
					f.second[make_pair(tempLabel, "_total")] = total;
					total = l.second;
					tempLabel = l.first.first;
				}
			}
			f.second[make_pair(tempLabel, "_total")] = total;

			for(auto & l : f.second)
			{
				if(l.first.second == "_total")
					l.second /= N;
				else	
					l.second = l.second/pClass[l.first.second];
				cout << "   " << l.first.first << "/" << l.first.second << " = " << l.second << endl;
			}
		}


		for(auto & c : pClass)
		{
			c.second /= N;
			cout << c.first << " = " << c.second << endl;
		}

		this->numberClasses = pClass.size();
	}

	void loadTesting(string filename)
	{
		fstream file;
		file.open(filename.c_str());
		string line;
		string word;		

		while(getline( file, line ))
		{			 
			stringstream ss;
			ss << line;			

			vector<string> temp;

			float pT = 1.0;
			for(int i=0 ; i<numberFeatures ; ++i)
			{
				ss >> word;
				temp.push_back(word);
				pT *= pTable[featureLabels[i]][make_pair(word, "_total")];
			}						

			float max = 0.0;
			float pC;
			string label;
			for(auto & c : pClass)
			{				
				pC = 1;
				for(int i=0 ; i<numberFeatures ; ++i)
				{
					pC *= pTable[featureLabels[i]][make_pair(temp[i],c.first)];
				}
				pC *= c.second;
				pC /= pT;

				if(pC > max)
				{
					max = pC;
					label = c.first;
				}
			}

			cout << "Class " << label << " with " << max << endl;
		}
	}

};

int main()
{
	NaiveBayes NB;
	NB.loadTraining("car.data");
	NB.loadTesting("car-prueba.data");
	return 0;
}