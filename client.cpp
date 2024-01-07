#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <mmreg.h>
#include <Windows.h>
#include <mmsystem.h>

#define Message(msg) MessageBoxA(GetConsoleWindow(), msg, "Client Contact - Error", MB_OK)
#define SERVER_IP "127.0.0.1" // Change this to the server's IP address
#define PORT 6868

using namespace std;

const int NUMPTS = 96000 * 0.1; // 0.1 seconds
int sampleRate = 48000;
float waveIn[NUMPTS];  // changed to float to match the format

int main()
{
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);

    if (wsOk != 0)
    {
        Message("Can't initialize winsock! Quitting");
        cout << "Can't initialize winsock! Quitting" << endl
             << endl;
        return 1;
    }

    // Create a socket
    SOCKET out = socket(AF_INET, SOCK_STREAM, 0);
    if (out == INVALID_SOCKET)
    {
        Message("Can't create a socket! Quitting");
        cout << "Can't create a socket! Quitting" << endl
             << endl;
        return 1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    hint.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (connect(out, (struct sockaddr *)&hint, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
        cout << "connect(out, (struct sockaddr *)&hint, sizeof(struct sockaddr_in)) == SOCKET_ERROR" << endl;
        Sleep(500);
    }

    // Set up audio capture
    WAVEFORMATEX pFormat;
    pFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    pFormat.nChannels = 1;                 // 1 = mono, 2 = stereo
    pFormat.nSamplesPerSec = sampleRate;   // 48000
    pFormat.wBitsPerSample = 32;           // 32 for IEEE float
    pFormat.nAvgBytesPerSec = sampleRate * (pFormat.nChannels * pFormat.wBitsPerSample / 8);
    pFormat.nBlockAlign = pFormat.nChannels * (pFormat.wBitsPerSample / 8);
    pFormat.cbSize = 0;

    MMRESULT result;
    HWAVEIN hWaveIn;
    WAVEHDR WaveInHdr;

    result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
    if (result)
    {
        char fault[256];
        waveInGetErrorTextA(result, fault, 256);
        MessageBoxA(GetConsoleWindow(), fault, "Failed to open waveform input device.", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    while (true)
    {
        // Set up and prepare header for input
        WaveInHdr.lpData = (LPSTR)waveIn;  // use waveIn array
        WaveInHdr.dwBufferLength = NUMPTS * sizeof(float);  // corrected buffer length to match format
        WaveInHdr.dwBytesRecorded = 0;
        WaveInHdr.dwUser = 0L;
        WaveInHdr.dwFlags = 0L;
        WaveInHdr.dwLoops = 0L;

        waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

        // Insert a wave input buffer
        result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
        if (result)
        {
            MessageBoxW(GetConsoleWindow(), L"Failed to read block from device", NULL, MB_OK | MB_ICONEXCLAMATION);
            return 1;
        }

        // Commence sampling input
        result = waveInStart(hWaveIn);
        if (result)
        {
            MessageBoxW(GetConsoleWindow(), L"Failed to start recording", NULL, MB_OK | MB_ICONEXCLAMATION);
            return 1;
        }

        // Wait until finished recording
        while (waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING)
        {
        }

        // Send audio data to the server
        if (send(out, (char *)waveIn, NUMPTS * sizeof(float), 0) <= 0)  // corrected send length and cast
        {
            cout << "Failed to send audio data." << endl;
            break;
        }
    }

    waveInClose(hWaveIn);

    // Close the socket
    closesocket(out);

    // Cleanup winsock
    WSACleanup();

    return 0;
}
