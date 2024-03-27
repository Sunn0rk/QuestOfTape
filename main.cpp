#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cassert>

using namespace std;

struct Config
{
    DWORD ReadingDelay;
    DWORD RecordingDelay;
    DWORD RotateTapeDelay;
    int option;
};

void loadConfig(Config &config)
{
    ifstream fin("config.txt");
    string line;
    while (getline(fin, line))
    {
        istringstream sin(line.substr(line.find("=") + 1));
        if ((int)line.find("ReadingDelay") != -1)
            sin >> config.ReadingDelay;
        else if ((int)line.find("RecordingDelay") != -1)
            sin >> config.RecordingDelay;
        else if ((int)line.find("RotateTapeDelay") != -1)
            sin >> config.RotateTapeDelay;
        else if ((int)line.find("Option") != -1)
            sin >> config.option;
    }
}

class IFTape
{
public:
    int virtual Reading() = 0;
    int virtual GetLength() = 0;
    int virtual GetLocation() = 0;
    void virtual Recording(int value) = 0;
    void virtual Rotate(int i) = 0;
    void virtual Increase(int i) = 0;
};

class Tape : public IFTape
{

public:
    int length;
    int *TapeValue;
    int location;
    DWORD ReadingDelay;
    DWORD RecordingDelay;
    DWORD RotateTapeDelay;
    string TapeName;

    Tape(int mlength, int mlocation)
    {
        this->length = mlength;
        TapeValue = new int[length];
        this->location = mlocation;
    };

    void Initialization(int *arr)
    {
        for (int i = 0; i < length; i++)
            TapeValue[i] = arr[i];
    };

    void Delete()
    {
        delete[] TapeValue;
    };

    int Reading() override
    {
        Sleep(ReadingDelay);
        int value = TapeValue[location];
        return value;
    };

    void Recording(int value) override
    {
        Sleep(RecordingDelay);
        TapeValue[location] = value;
    };

    void Rotate(int i) override
    {
        Sleep(RotateTapeDelay);
        location = location + i;
    };

    void Increase(int i) override
    {
        length = length + i;
    };

    int GetLength() override
    {
        return length;
    };
    int GetLocation() override
    {
        return location;
    };
};

class Operator
{
public:
    int mode = 1;
    int min = -2147483647;
    int count;

    void Sort(IFTape &EnteringTape, IFTape &RecordingTape, IFTape &TemporaryMainTape, IFTape &TemporarySupportTape)
    {

        TemporarySupportTape.Recording(EnteringTape.Reading());
        do
        {
            TemporaryMainTape.Recording(2147483647);
            for (int i = 0; i < EnteringTape.GetLength(); i++)
            {
                if (EnteringTape.Reading() <= TemporaryMainTape.Reading() && EnteringTape.Reading() > min)
                {
                    TemporaryMainTape.Increase(1);
                    TemporaryMainTape.Rotate(1);
                    TemporaryMainTape.Recording(EnteringTape.Reading());
                }
                if (EnteringTape.Reading() >= TemporarySupportTape.Reading())
                {
                    TemporarySupportTape.Increase(1);
                    TemporarySupportTape.Rotate(1);
                    TemporarySupportTape.Recording(EnteringTape.Reading());
                }
                EnteringTape.Rotate(mode);
            }
            mode = mode * (-1);
            EnteringTape.Rotate(mode);
            min = TemporaryMainTape.Reading();
            count = 0;
            while (TemporaryMainTape.Reading() == min)
            {
                RecordingTape.Recording(TemporaryMainTape.Reading());
                RecordingTape.Rotate(1);
                TemporaryMainTape.Rotate(-1);
                count++;
            }
            for (int i = 0; i < count; i++)
            {
                TemporaryMainTape.Rotate(1);
            }

        } while (TemporaryMainTape.Reading() != TemporarySupportTape.Reading());
    }
};

void Assert(int option, DWORD ReadingDelay, DWORD RecordingDelay, DWORD RotateTapeDelay)
{
    int N = 0;
    int *mas = new int[N];
    switch (option)
    {
    case 1:
    {
        const int Length = 10;
        int ATape[Length]{1, 4, 2, 7, 8, 3, 0, 5, 9, 6};
        for (int i = 0; i < Length; i++)
        {
            N++;
            mas[i] = ATape[i];
        }
        break;
    }
    case 2:
    {
        const int Length = 20;
        int ATape[Length]{-53, 7, -91, 96, -71, 58, 100, 48, -43, -2, 36, -44, -7, -93, 15, -43, -69, 20, -29, 61};
        for (int i = 0; i < Length; i++)
        {
            N++;
            mas[i] = ATape[i];
        }
        break;
    }
    case 3:
    {
        const int Length = 40;
        int ATape[Length]{-914, 137, -514, -570, -951, 163, -139, -710, -710, -327, 68, 493, 889, 275, -270, 45, -952, -898, -443, 427, -178, 373, -283, -630, -551, 519, 75, 37, -375, -93, 542, 133, -77, -669, -611, -848, -746, -504, 937, 300};
        for (int i = 0; i < Length; i++)
        {
            N++;
            mas[i] = ATape[i];
        }
        break;
    }
    }

    Operator Oper;
    cout << N << endl;
    Tape EnteringTape(N, 0);
    Tape RecordingTape(N, 0);
    Tape TemporaryMainTape(1, 0);
    Tape TemporarySupportTape(1, 0);

    EnteringTape.Initialization(mas);

    EnteringTape.ReadingDelay = RecordingTape.ReadingDelay = TemporaryMainTape.ReadingDelay = TemporarySupportTape.ReadingDelay = ReadingDelay;
    EnteringTape.RecordingDelay = RecordingTape.RecordingDelay = TemporaryMainTape.RecordingDelay = TemporarySupportTape.RecordingDelay = RecordingDelay;
    EnteringTape.RotateTapeDelay = RecordingTape.RotateTapeDelay = TemporaryMainTape.RotateTapeDelay = TemporarySupportTape.RotateTapeDelay = RotateTapeDelay;

    Oper.Sort(EnteringTape, RecordingTape, TemporaryMainTape, TemporarySupportTape);

    for(int i =0; i<N; i++)
    cout << RecordingTape.TapeValue[i]<<"  ";
    cout << endl;
    delete [] mas;
}

int main()
{
    Config config;
    loadConfig(config);
    
    const int N = 40;
    int ATape[N]{-915, 137, -514, -570, -951, 163, -139, -710, -710, -327, 68, 493, 889, 275, -270, 45, -952, -898, -443, 427, -178, 373, -283, -630, -551, 519, 75, 37, -375, -93, 542, 133, -77, -669, -611, -848, -746, -504, 937, 300};

    Operator Oper;

    Tape EnteringTape(N, 0);
    Tape RecordingTape(N, 0);
    Tape TemporaryMainTape(1, 0);
    Tape TemporarySupportTape(1, 0);

    EnteringTape.Initialization(ATape);

    EnteringTape.ReadingDelay = RecordingTape.ReadingDelay = TemporaryMainTape.ReadingDelay = TemporarySupportTape.ReadingDelay = config.ReadingDelay;
    EnteringTape.RecordingDelay = RecordingTape.RecordingDelay = TemporaryMainTape.RecordingDelay = TemporarySupportTape.RecordingDelay = config.RecordingDelay;
    EnteringTape.RotateTapeDelay = RecordingTape.RotateTapeDelay = TemporaryMainTape.RotateTapeDelay = TemporarySupportTape.RotateTapeDelay = config.RotateTapeDelay;

    Oper.Sort(EnteringTape, RecordingTape, TemporaryMainTape, TemporarySupportTape);

    for(int i =0; i<N; i++)
    cout << RecordingTape.TapeValue[i]<<"  ";
    cout << endl;

    return 0;
}