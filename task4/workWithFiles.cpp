#include "includes.cpp"


class workWithDB{
public:
    static void writeToFile(string fileName,string line)
    {
        ofstream fileIN;
        fileIN.open(fileName,ios_base::app);
        if(!fileIN.is_open()) {
            cout << "File " << fileName << " does not work to write"<<endl;
            return;
        }
        fileIN<<line<<endl;
        fileIN.close();
    }

    static bool makeIfstream(string fileName, ifstream &fileOUT)
    {
        fileOUT.open(fileName,ios_base::out);
        if(!fileOUT.is_open()) {
            //cout << "File " << fileName << " does not work to read"<<endl;
            return false;
        }
        return true;
    }

    static string readFromFile(ifstream &tempFileOUT)
    {
        string outLine = "";
        getline(tempFileOUT,outLine);
        return outLine;
    }

    static int countOfLinesInFile(string fileName)
    {
        ifstream orderOUT;
        int tempIterator = 0;
        if(makeIfstream(fileName, orderOUT)) {
            string lineOUT = "Message";
            while (lineOUT.size() > 0) {
                lineOUT = readFromFile(orderOUT);
                if (lineOUT.size() > 0) {
                    tempIterator++;
                }
            }
            orderOUT.close();
            return tempIterator;
        }
        return 0;
    }

    static void changeLineInAFile(string fileName, int id, string newLine)
    {
        vector<string> jsons;
        ifstream orderOUT;
        int tempIterator = 0;
        if(makeIfstream(fileName, orderOUT)) {
            string lineOUT = "Message";
            while (lineOUT.size() > 0) {
                lineOUT = readFromFile(orderOUT);
                if (lineOUT.size() > 0) {
                    if(tempIterator != id)
                        jsons.insert(jsons.begin(),lineOUT);
                    else
                        jsons.insert(jsons.begin(),newLine);
                    tempIterator++;
                }
            }
            orderOUT.close();
            ofstream orderIN;
            orderIN.open(fileName,std::ios::trunc);
            int countOfLines = jsons.size();
            for(int f = 0;f<countOfLines;f++) {
                string outValue = jsons.back();
                jsons.pop_back();
                orderIN << outValue << endl;
            }
            orderIN.close();
        }
    }

    static void deleteLineInAFile(string fileName, int id)
    {
        vector<string> jsons;
        ifstream orderOUT;
        int tempIterator = 0;
        if(makeIfstream(fileName, orderOUT)) {
            string lineOUT = "Message";
            while (lineOUT.size() > 0) {
                lineOUT = readFromFile(orderOUT);
                if (lineOUT.size() > 0) {
                    if(tempIterator != id)
                        jsons.insert(jsons.begin(),lineOUT);
                    else {
                        json tempJson = json::parse(lineOUT);
                        tempJson["deleted"] = true;
                        jsons.insert(jsons.begin(), tempJson.dump());
                    }
                    tempIterator++;
                }
            }
            orderOUT.close();
            ofstream orderIN;
            orderIN.open(fileName,std::ios::trunc);
            int countOfLines = jsons.size();
            for(int f = 0;f<countOfLines;f++) {
                string outValue = jsons.back();
                jsons.pop_back();
                orderIN << outValue << endl;
            }
            orderIN.close();
        }
    }

};