#define _UNICODE
#include <windows.h>

#include <stdio.h>

#include <winioctl.h>


// Format the Win32 system error code to string

void ErrorMessage(DWORD dwCode);



int wmain(int argc, WCHAR** argv)

{

    HANDLE hVolume;

    LPCWSTR lpDrive = L"\\\\.\\E:";

    // {0} ~ ZeroMemory()

    PNTFS_VOLUME_DATA_BUFFER ntfsVolData = { 0 };

    // NTFS_EXTENDED_VOLUME_DATA versionMajMin = {0};

    BOOL bDioControl = FALSE;

    DWORD dwWritten = 0;



    hVolume = CreateFile(lpDrive,

        GENERIC_READ | GENERIC_WRITE,

        FILE_SHARE_READ | FILE_SHARE_WRITE,

        NULL,

        OPEN_EXISTING,

        0,

        NULL);



    if (hVolume == INVALID_HANDLE_VALUE)

    {

        wprintf(L"CreateFile() failed!\n");

        ErrorMessage(GetLastError());

        if (CloseHandle(hVolume) != 0)

            wprintf(L"hVolume handle was closed successfully!\n");

        else

        {

            wprintf(L"Failed to close hVolume handle!\n");

            ErrorMessage(GetLastError());

        }

        exit(1);

    }

    else

        wprintf(L"CreateFile() is pretty fine!\n");



    ntfsVolData = (PNTFS_VOLUME_DATA_BUFFER)malloc(sizeof(NTFS_VOLUME_DATA_BUFFER) + sizeof(NTFS_EXTENDED_VOLUME_DATA));



    if (ntfsVolData == NULL)

        wprintf(L"Insufficient memory!\n");

    else

        wprintf(L"Memory allocated successfully!\n");



    // a call to FSCTL_GET_NTFS_VOLUME_DATA returns the structure NTFS_VOLUME_DATA_BUFFER

    bDioControl = DeviceIoControl(hVolume, FSCTL_GET_NTFS_VOLUME_DATA, NULL, 0, ntfsVolData,

        sizeof(NTFS_VOLUME_DATA_BUFFER) + sizeof(NTFS_EXTENDED_VOLUME_DATA), &dwWritten, NULL);



    // Failed or pending

    if (bDioControl == 0)

    {

        wprintf(L"DeviceIoControl() failed!\n");

        ErrorMessage(GetLastError());

        if (CloseHandle(hVolume) != 0)

            wprintf(L"hVolume handle was closed successfully!\n");

        else

        {

            wprintf(L"Failed to close hVolume handle!\n");

            ErrorMessage(GetLastError());

        }

        exit(1);

    }

    else

        wprintf(L"DeviceIoControl() is working...\n\n");



    wprintf(L"Volume Serial Number : 0X % .8X % .8X\n", ntfsVolData->VolumeSerialNumber.HighPart, ntfsVolData->VolumeSerialNumber.LowPart);

    wprintf(L"The number of bytes in a cluster : % u\n", ntfsVolData->BytesPerCluster);

    wprintf(L"The number of bytes in a file record segment : % u\n", ntfsVolData->BytesPerFileRecordSegment);

    wprintf(L"The number of bytes in a sector : % u\n", ntfsVolData->BytesPerSector);

    wprintf(L"The number of clusters in a file record segment : % u\n", ntfsVolData->ClustersPerFileRecordSegment);

    wprintf(L"The number of free clusters in the specified volume : % u\n", ntfsVolData->FreeClusters);

    wprintf(L"The starting logical cluster number of the master file table :' 0X % .8X % .8X\n", ntfsVolData->MftStartLcn.HighPart, ntfsVolData->MftStartLcn.LowPart);

    wprintf(L"The starting logical cluster number of the master file table mirror : 0X % .8X % .8X\n", ntfsVolData->Mft2StartLcn.HighPart, ntfsVolData->Mft2StartLcn.LowPart);

    wprintf(L"The length of the master file table, in bytes : % u\n", ntfsVolData->MftValidDataLength);

    wprintf(L"The starting logical cluster number of the master file table zone : 0X % .8X % .8X\n", ntfsVolData->MftZoneStart.HighPart, ntfsVolData->MftZoneStart.LowPart);

    wprintf(L"The ending logical cluster number of the master file table zone : 0X % .8X % .8X\n", ntfsVolData->MftZoneEnd.HighPart, ntfsVolData->MftZoneEnd.LowPart);

    wprintf(L"The number of sectors : % u\n", ntfsVolData->NumberSectors);

    wprintf(L"Total Clusters(used and free) : % u\n", ntfsVolData->TotalClusters);

    wprintf(L"The number of reserved clusters : % u\n", ntfsVolData->TotalReserved);



    // To extract this info the buffer must be large enough, however...FAILED!

    //wprintf(LByte returns: %u\n, versionMajMin.ByteCount);

    //wprintf(LMajor version: %u\n, versionMajMin.MajorVersion);

    //wprintf(LMinor version: %u\n, versionMajMin.MinorVersion);



    if (CloseHandle(hVolume) != 0)

        wprintf(L"\nhVolume handle was closed successfully!\n");

    else

    {

        wprintf(L"\nFailed to close hVolume handle!\n");

        ErrorMessage(GetLastError());

    }



    // free up the allocated memory by malloc()

    free(ntfsVolData);



    return 0;

}



// Accessory function converting the GetLastError() code

// to a meaningful string

void ErrorMessage(DWORD dwCode)

{

    // get the error code...

    DWORD dwErrCode = dwCode;

    DWORD dwNumChar;



    LPWSTR szErrString = NULL;  // will be allocated and filled by FormatMessage



    dwNumChar = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |

        FORMAT_MESSAGE_FROM_SYSTEM, // use windows internal message table

        0,       // 0 since source is internal message table

        dwErrCode, // this is the error code number

        0,       // auto-determine language to use

        (LPWSTR)&szErrString, // the message

        0,                 // min size for buffer

        0);               // since getting message from system tables

    if (dwNumChar == 0)

        wprintf(L"FormatMessage() failed, error % u\n", GetLastError());

    //else

    //    wprintf(LFormatMessage() should be fine!\n);



    wprintf(L"Error code % u:\n % s\n", dwErrCode, szErrString);



    // This buffer used by FormatMessage()

    if (LocalFree(szErrString) != NULL)

        wprintf(L"Failed to free up the buffer, error % u\n", GetLastError());

    //else

    //    wprintf(LBuffer has been freed\n);

}