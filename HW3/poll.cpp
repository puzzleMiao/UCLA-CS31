#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;


bool isValidUppercaseStateCode(string stateCode);  // check whether the statecodes are valid
bool isValidPollString(string pollData);           // fuction1 in project3
bool isValidStateForecast(string stateForecast);   // check whether the stateforecast are valid
void makeStrUpper(string& pollData);               // change all letters to uppercase
int countSeats(string pollData, char party, int& seatCount);   // function2 in project3

//test function.

int main()
{
    assert(isValidPollString("CT5D,NY9R16D1I,VT,ne3r00D"));
    assert(!isValidPollString("ZT5D,NY9R16D1I,VT,ne3r00D"));
    int seats;
    seats = -999;    // so we can detect whether countSeats sets seats
    assert(countSeats("CT5D,NY9R16D1I,VT,ne3r00D", 'd', seats) == 0  &&  seats == 21);
    seats = -999;    // so we can detect whether countSeats changes seats
    assert(countSeats("CT5D,NY9R16D1I,VT,ne3r00D", '%', seats) == 2  &&  seats == -999);
    assert(isValidPollString("MA9D,KS4R")  &&  isValidPollString("KS4R,MA9D"));
    assert(isValidPollString("MA9D,,KS4R") == isValidPollString("KS4R,,MA9D"));

    cerr << "All tests succeeded" << endl;


}


//*************************************
//  Fuction: isValidPollString
//*************************************

bool isValidPollString(string pollData)
{
    makeStrUpper(pollData);
    
    if (pollData == "")
        return true;
    
    for (int i = 0; i < pollData.size(); i++) {
        if (!isalpha(pollData.at(i)) && !isdigit(pollData.at(i)) && pollData.at(i) != ',')   // only number, letter and ',' are allowed in pollData
            return false;
    }
    
    int i = 0;
    pollData += ',';                                                                        // make easier for the while loop
    while (i != pollData.size())
    {
        string temp = "";
        while(pollData.at(i) != ',')
        {
            temp += pollData.at(i);
            i++;
        }
        if (!isValidStateForecast(temp))
        {
            return false;
        }
        else
        {
            i++;
        }
    }
    return true;
}

//*************************************
//  Fuction: countSeats
//*************************************

int countSeats(string pollData, char party, int& seatCount)
{
    // chenck if polldata are valid.
    if (!isValidPollString(pollData))
    {
        return 1;
    }
    
    if (!isalpha(party))
    {
        return 2;
    }
    
    else if (pollData.size() <= 2)
    {
        seatCount = 0;    // sets seatTally = 0 when function returns 0
        return 0;
    }
    
    pollData += ',';
    makeStrUpper(pollData);
    party = toupper(party);
    string tempSeatsString;
    seatCount = 0;
    
    
    for (int x = 3; x != pollData.size(); x++)
    {
        if(pollData.at(x) == party)
        {
            if(isdigit(pollData.at(x-1)))
            {
                if(isdigit(pollData.at(x-2)))
                {
                    seatCount += stoi(pollData.substr(x - 2, 2));
                }
                else
                {
                    seatCount += stoi(pollData.substr(x - 1, 1));
                }
            }
        }
    }
    
    return 0;
}

//*************************************
//  Fuction: makeStrUpper
//*************************************

void makeStrUpper(string& pollData)
{
    for(int i = 0; i != pollData.size(); i++)
    {
        pollData.at(i) = toupper(pollData.at(i));
    }
}

//*************************************
//  Fuction: isValidStateForecast
//*************************************

bool isValidStateForecast(string tempStr)
{
    if (!(isValidUppercaseStateCode(tempStr.substr(0, 2))))
        return false;
    else if (tempStr.size() == 2)                                                                    // if string is just a state code, return true
        return true;

    int k = 2;                                                                                       // first two char are state code
    while (k < tempStr.size())
    {
        string partySeats = "";
        while (isdigit(tempStr[k]) && k != tempStr.size() - 1 && partySeats.size() < 2)              // check digits
        {
            partySeats += tempStr[k];
            k++;
        }

        if (!(isalpha(tempStr[k])) || (isalpha(tempStr[k]) && !isdigit(tempStr[k - 1])))             // checks for cases where the char following digits
            return false;                                                                            // is non-alphabetical or where no digits separate
        else if (partySeats == "" && k != tempStr.size() - 1)                                        // two alphabetical characters
        {
            k++;
            continue;
        }
        else
        {
            k++;
            continue;
        }
    }
    return true;
}


//*************************************
//  Fuction: isValidUppercaseStateCode
//*************************************

bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
        "AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
        "LA.ME.MA.MD.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
        "OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
            stateCode.find('.') == string::npos  &&  // no '.' in stateCode
            codes.find(stateCode) != string::npos);  // match found
}
